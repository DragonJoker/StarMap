#include "OverlayRenderer.h"

#include <GlLib/GlAttribute.h>
#include <GlLib/GlBuffer.h>
#include <GlLib/GlUniform.h>

#include "BorderPanelOverlay.h"
#include "ElementsList.h"
#include "Material.h"
#include "PanelOverlay.h"
#include "TextOverlay.h"
#include "UberShader.h"
#include "Viewport.h"

namespace render
{
	//*************************************************************************

	namespace
	{
		void doFillBuffers( TextOverlay::QuadArray::const_iterator begin
			, uint32_t count
			, uint32_t & offset
			, gl::Buffer< TextOverlay::Quad > & buffer )
		{
			buffer.bind();
			buffer.upload( offset, count, &( *begin ) );
			buffer.unbind();
			offset += count;
		}

		std::vector< OverlayPtr > doSortPerZIndex( OverlayList const & overlays
			, gl::IVec2 const & size )
		{
			std::vector< OverlayPtr > result;

			for ( auto & overlay : overlays )
			{
				auto & ovLeftTop = overlay.second->position();
				auto ovRightBottom = ovLeftTop + overlay.second->size();

				if ( overlay.second->visible()
					&& ovLeftTop.x < size.x
					&& ovLeftTop.y < size.y
					&& ovRightBottom.x > 0
					&& ovRightBottom.y > 0 )
				{
					auto it = std::lower_bound( std::begin( result )
						, std::end( result )
						, overlay.second->index()
						, []( OverlayPtr const & res, uint32_t index )
					{
						return res->index() <= index;
					} );

					if ( it == std::end( result ) )
					{
						result.push_back( overlay.second );
					}
					else
					{
						result.insert( it, overlay.second );
					}
				}
			}

			return result;
		}


		void doBindMaterial( OverlayNode const & node
			, Material const & material )
		{
			node.m_colour->value( { material.ambient(), material.opacity() } );

			if ( material.hasDiffuseMap()
				&& node.m_mapColour->valid() )
			{
				node.m_mapColour->value( 0 );
				node.m_mapColour->bind();
				material.diffuseMap().bind( 0 );
			}

			if ( material.hasOpacityMap()
				&& node.m_mapOpacity->valid() )
			{
				node.m_mapOpacity->value( 1 );
				node.m_mapOpacity->bind();
				material.opacityMap().bind( 1 );
			}
		}

		void doUnbindMaterial( OverlayNode const & node
			, Material const & material )
		{
			if ( material.hasOpacityMap()
				&& node.m_mapOpacity->valid() )
			{
				material.opacityMap().unbind( 1 );
			}

			if ( material.hasDiffuseMap()
				&& node.m_mapColour->valid() )
			{
				material.diffuseMap().unbind( 0 );
			}
		}	}

	//*************************************************************************

	OverlayNode::OverlayNode( bool text )
		: m_program{ UberShader::createShaderProgram( RenderType::eScene
			, text ? TextureFlag::eOpacity : TextureFlag::eNone
			, text ? OpacityType::eAlphaTest : OpacityType::eAlphaBlend
			, text ? ObjectType::eTextOverlay : ObjectType::ePanelOverlay ) }
		, m_overlayUbo{ "Overlay", 0u, *m_program }
		, m_mpUniform{ &m_overlayUbo.createUniform< gl::Mat4 >( "mtxMP" ) }
		, m_colour{ &m_overlayUbo.createUniform< gl::RgbaColour >( "colour" ) }
		, m_mapColour{ gl::makeUniform< int >( "mapColour", *m_program ) }
		, m_mapOpacity{ gl::makeUniform< int >( "maOpacity", *m_program ) }
	{
		m_overlayUbo.initialise();
	}

	//*************************************************************************

	OverlayRenderer::OverlayRenderer( uint32_t maxCharsPerBuffer )
		: m_panelNode{ false }
		, m_textNode{ true }
		, m_pipeline{ true, false, true, true }
		, m_panelBuffer
		{
			gl::makeBuffer( gl::BufferTarget::eArrayBuffer
				, std::vector< TextOverlay::Quad >( 1u ) ),
			m_panelNode.m_program->createAttribute< gl::Vec2 >( "position"
				, sizeof( TextOverlay::Vertex )
				, offsetof( TextOverlay::Vertex, coords ) ),
			m_panelNode.m_program->createAttribute< gl::Vec2 >( "texture"
				, sizeof( TextOverlay::Vertex )
				, offsetof( TextOverlay::Vertex, text ) )
		}
		, m_borderBuffer
		{
			gl::makeBuffer( gl::BufferTarget::eArrayBuffer
				, std::vector< TextOverlay::Quad >( 8u ) ),
			m_panelNode.m_program->createAttribute< gl::Vec2 >( "position"
				, sizeof( TextOverlay::Vertex )
				, offsetof( TextOverlay::Vertex, coords ) ),
			m_panelNode.m_program->createAttribute< gl::Vec2 >( "texture"
				, sizeof( TextOverlay::Vertex )
				, offsetof( TextOverlay::Vertex, text ) )
		}
		, m_maxCharsPerBuffer{ maxCharsPerBuffer }
		, m_viewport{ { 100, 100 } }
	{
		doCreateTextBuffer();
	}

	OverlayRenderer::~OverlayRenderer()
	{
	}

	void OverlayRenderer::draw( OverlayList const & overlays )
	{
		auto & size = m_viewport.size();
		auto sorted = doSortPerZIndex( overlays, size );

		for ( auto & overlay : sorted )
		{
			overlay->render( *this );
		}
	}

	void OverlayRenderer::beginRender( gl::IVec2 const & size )
	{
		if ( m_viewport.size() != size )
		{
			m_sizeChanged = true;
			m_viewport.resize( size );
			m_viewport.ortho( 0.0f
				, float( size.x )
				, 0.0f
				, float( size.y )
				, 0.0f
				, 1000.0f );
			m_transform = m_viewport.transform();
		}

		m_pipeline.apply();
	}

	void OverlayRenderer::endRender()
	{
		m_sizeChanged = false;
	}

	void OverlayRenderer::drawPanel( PanelOverlay const & overlay )
	{
		m_panelNode.m_program->bind();
		m_panelBuffer.m_vbo->bind();
		m_panelBuffer.m_vbo->upload( 0u, 1u, overlay.panelVertex().data() );
		m_panelBuffer.m_vbo->unbind();
		doDrawBuffer( m_panelBuffer
			, 1u
			, overlay.transform()
			, overlay.material()
			, m_panelNode );
		m_panelNode.m_program->unbind();
	}

	void OverlayRenderer::drawBorderPanel( BorderPanelOverlay const & overlay )
	{
		m_panelNode.m_program->bind();
		m_panelBuffer.m_vbo->bind();
		m_panelBuffer.m_vbo->upload( 0u, 1u, overlay.panelVertex().data() );
		m_panelBuffer.m_vbo->unbind();
		doDrawBuffer( m_panelBuffer
			, 1u
			, overlay.transform()
			, overlay.material()
			, m_panelNode );
		m_borderBuffer.m_vbo->bind();
		m_borderBuffer.m_vbo->upload( 0u, 8u, overlay.borderVertex().data() );
		m_borderBuffer.m_vbo->unbind();
		doDrawBuffer( m_borderBuffer
			, 8u
			, overlay.transform()
			, overlay.borderMaterial()
			, m_panelNode );
		m_panelNode.m_program->unbind();
	}

	void OverlayRenderer::drawText( TextOverlay const & overlay )
	{
		uint32_t offset{ 0u };
		auto quads = overlay.textVertex();
		uint32_t count = uint32_t( quads.size() );
		auto it = quads.cbegin();
		uint32_t index{ 0u };
		std::vector< VertexBuffer const * > buffers;

		while ( count > m_maxCharsPerBuffer )
		{
			buffers.push_back( &doFillTextPart( count
				, offset
				, it
				, index ) );
			count -= m_maxCharsPerBuffer;
		}

		if ( count > 0 )
		{
			buffers.push_back( &doFillTextPart( count
				, offset
				, it
				, index ) );
		}

		count = uint32_t( quads.size() );
		m_textNode.m_program->bind();

		for ( auto & buffer : buffers )
		{
			doDrawBuffer( *buffer
				, std::min( count, m_maxCharsPerBuffer )
				, overlay.transform()
				, overlay.material()
				, m_textNode );
			count -= m_maxCharsPerBuffer;
		}

		m_textNode.m_program->unbind();
	}

	OverlayRenderer::VertexBuffer const & OverlayRenderer::doCreateTextBuffer()
	{
		auto buffer = std::make_unique< VertexBuffer >();
		buffer->m_vbo = gl::makeBuffer( gl::BufferTarget::eArrayBuffer
			, std::vector< TextOverlay::Quad >( m_maxCharsPerBuffer ) );
		buffer->m_position = m_textNode.m_program->createAttribute< gl::Vec2 >( "position"
			, sizeof( TextOverlay::Vertex )
			, offsetof( TextOverlay::Vertex, coords ) );
		buffer->m_texture = m_textNode.m_program->createAttribute< gl::Vec2 >( "texture"
			, sizeof( TextOverlay::Vertex )
			, offsetof( TextOverlay::Vertex, text ) );
		m_textBuffers.push_back( std::move( buffer ) );
		return *m_textBuffers.back();
	}

	void OverlayRenderer::doDrawBuffer( VertexBuffer const & buffer
		, uint32_t count
		, gl::Mat4 const & transform
		, Material const & material
		, OverlayNode const & node )
	{
		buffer.m_vbo->bind();
		buffer.m_position->bind();
		node.m_mpUniform->value( m_transform * transform );
		doBindMaterial( node, material );
		node.m_overlayUbo.bind( 0u );
		glCheckError( glDrawArrays, GL_TRIANGLES, 0, count * 6 );
		doUnbindMaterial( node, material );
		buffer.m_position->unbind();
		buffer.m_vbo->unbind();
	}

	OverlayRenderer::VertexBuffer const & OverlayRenderer::doFillTextPart
		( uint32_t count
		, uint32_t & offset
		, TextOverlay::QuadArray::const_iterator & it
		, uint32_t & index )
	{
		OverlayRenderer::VertexBuffer const * buffer{ nullptr };

		if ( offset + count > m_maxCharsPerBuffer )
		{
			buffer = &doCreateTextBuffer();
			offset = 0u;
			++index;
		}
		else
		{
			buffer = m_textBuffers[index].get();
		}

		auto & vertexBuffer = *buffer->m_vbo;
		count = std::min( count, m_maxCharsPerBuffer );
		doFillBuffers( it, count, offset, vertexBuffer );
		it += count;

		return *buffer;
	}
}
