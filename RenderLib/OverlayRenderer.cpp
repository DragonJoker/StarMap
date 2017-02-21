#include "OverlayRenderer.h"

#include <GlLib/GlAttribute.h>
#include <GlLib/GlBuffer.h>
#include <GlLib/GlUniform.h>

#include "BorderPanelOverlay.h"
#include "ElementsList.h"
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
	}

	//*************************************************************************

	OverlayRenderer::OverlayProgram::OverlayProgram( bool text )
		: m_program{ UberShader::createShaderProgram( RenderType::eScene
			, text ? TextureFlag::eOpacity : TextureFlag::eNone
			, text ? OpacityType::eAlphaTest : OpacityType::eAlphaBlend
			, text ? ObjectType::eTextOverlay : ObjectType::ePanelOverlay ) }
		, m_mpUniform{ gl::makeUniform< gl::Matrix4x4 >( "mtxMP", *m_program ) }
		, m_map{ text ? gl::makeUniform< int >( "mapText", *m_program ) : nullptr }
		, m_colour{ gl::makeUniform< gl::RgbaColour >( "colour", *m_program ) }
	{
	}

	//*************************************************************************

	OverlayRenderer::OverlayRenderer( uint32_t maxCharsPerBuffer )
		: m_panelProgram{ false }
		, m_textProgram{ true }
		, m_pipeline{ true, false, true, true }
		, m_panelBuffer
		{
			gl::makeBuffer( gl::BufferTarget::eArrayBuffer
				, std::vector< TextOverlay::Quad >( 1u ) ),
			m_panelProgram.m_program->createAttribute< gl::Vec2 >( "position"
				, sizeof( TextOverlay::Vertex )
				, offsetof( TextOverlay::Vertex, coords ) ),
			m_panelProgram.m_program->createAttribute< gl::Vec2 >( "texture"
				, sizeof( TextOverlay::Vertex )
				, offsetof( TextOverlay::Vertex, text ) )
		}
		, m_borderBuffer
		{
			gl::makeBuffer( gl::BufferTarget::eArrayBuffer
				, std::vector< TextOverlay::Quad >( 8u ) ),
			m_panelProgram.m_program->createAttribute< gl::Vec2 >( "position"
				, sizeof( TextOverlay::Vertex )
				, offsetof( TextOverlay::Vertex, coords ) ),
			m_panelProgram.m_program->createAttribute< gl::Vec2 >( "texture"
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
		m_panelProgram.m_program->bind();
		m_panelBuffer.m_vbo->bind();
		m_panelBuffer.m_vbo->upload( 0u, 1u, overlay.panelVertex().data() );
		m_panelBuffer.m_vbo->unbind();
		doDrawBuffer( m_panelBuffer
			, 1u
			, overlay.transform()
			, overlay.colour() );
		m_panelProgram.m_program->unbind();
	}

	void OverlayRenderer::drawBorderPanel( BorderPanelOverlay const & overlay )
	{
		m_panelProgram.m_program->bind();
		m_panelBuffer.m_vbo->bind();
		m_panelBuffer.m_vbo->upload( 0u, 1u, overlay.panelVertex().data() );
		m_panelBuffer.m_vbo->unbind();
		doDrawBuffer( m_panelBuffer
			, 1u
			, overlay.transform()
			, overlay.colour() );
		m_borderBuffer.m_vbo->bind();
		m_borderBuffer.m_vbo->upload( 0u, 8u, overlay.borderVertex().data() );
		m_borderBuffer.m_vbo->unbind();
		doDrawBuffer( m_borderBuffer
			, 8u
			, overlay.transform()
			, overlay.borderColour() );
		m_panelProgram.m_program->unbind();
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
		m_textProgram.m_program->bind();

		for ( auto & buffer : buffers )
		{
			doDrawBuffer( *buffer
				, std::min( count, m_maxCharsPerBuffer )
				, overlay.transform()
				, overlay.colour()
				, overlay.fontTexture().texture() );
			count -= m_maxCharsPerBuffer;
		}

		m_textProgram.m_program->unbind();
	}

	OverlayRenderer::VertexBuffer const & OverlayRenderer::doCreateTextBuffer()
	{
		auto buffer = std::make_unique< VertexBuffer >();
		buffer->m_vbo = gl::makeBuffer( gl::BufferTarget::eArrayBuffer
			, std::vector< TextOverlay::Quad >( m_maxCharsPerBuffer ) );
		buffer->m_position = m_textProgram.m_program->createAttribute< gl::Vec2 >( "position"
			, sizeof( TextOverlay::Vertex )
			, offsetof( TextOverlay::Vertex, coords ) );
		buffer->m_texture = m_textProgram.m_program->createAttribute< gl::Vec2 >( "texture"
			, sizeof( TextOverlay::Vertex )
			, offsetof( TextOverlay::Vertex, text ) );
		m_textBuffers.push_back( std::move( buffer ) );
		return *m_textBuffers.back();
	}

	void OverlayRenderer::doDrawBuffer( VertexBuffer const & buffer
		, uint32_t count
		, gl::Matrix4x4 const & transform
		, gl::RgbaColour const & colour )
	{
		buffer.m_vbo->bind();
		buffer.m_position->bind();
		m_panelProgram.m_mpUniform->value( m_transform * transform );
		m_panelProgram.m_colour->value( colour );
		m_panelProgram.m_mpUniform->bind();
		m_panelProgram.m_colour->bind();
		glCheckError( glDrawArrays, GL_TRIANGLES, 0, count * 6 );
		buffer.m_position->unbind();
		buffer.m_vbo->unbind();
	}

	void OverlayRenderer::doDrawBuffer( VertexBuffer const & buffer
		, uint32_t count
		, gl::Matrix4x4 const & transform
		, gl::RgbaColour const & colour
		, Texture const & texture )
	{
		buffer.m_vbo->bind();
		buffer.m_position->bind();
		buffer.m_texture->bind();
		m_textProgram.m_mpUniform->value( m_transform * transform );
		m_textProgram.m_colour->value( colour );
		m_textProgram.m_map->value( 0 );
		m_textProgram.m_mpUniform->bind();
		m_textProgram.m_colour->bind();
		m_textProgram.m_map->bind();
		texture.bind( 0 );
		glCheckError( glDrawArrays, GL_TRIANGLES, 0, count * 6 );
		texture.unbind( 0 );
		buffer.m_texture->unbind();
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
