#include "OverlayRenderer.h"

#include <GlLib/GlAttribute.h>
#include <GlLib/GlBuffer.h>
#include <GlLib/GlUniform.h>

#include "ElementsList.h"
#include "UberShader.h"
#include "Viewport.h"

namespace render
{
	namespace
	{
		void doFillBuffers( TextOverlay::VertexArray::const_iterator begin
			, uint32_t count
			, uint32_t & offset
			, gl::Buffer< TextOverlay::Vertex > & buffer )
		{
			buffer.bind();
			buffer.upload( offset, count, &( *begin ) );
			buffer.unbind();
			offset += count;
		}
	}

	OverlayRenderer::OverlayRenderer( uint32_t maxCharsPerBuffer )
		: m_program{ UberShader::createShaderProgram( RenderType::eScene
			, TextureFlag::eNone
			, OpacityType::eOpaque
			, ObjectType::eOverlay ) }
		, m_pipeline{ true, false, true, true }
		, m_mpUniform{ gl::makeUniform< gl::Matrix4x4 >( "mtxMP", *m_program ) }
		, m_mapText{ gl::makeUniform< int >( "mapText", *m_program ) }
		, m_colour{ gl::makeUniform< gl::RgbaColour >( "colour", *m_program ) }
		, m_maxCharsPerBuffer{ maxCharsPerBuffer }
		, m_viewport{ { 100, 100 } }
	{
		doCreateTextBuffer();
	}

	OverlayRenderer::~OverlayRenderer()
	{
	}

	void OverlayRenderer::draw( TextOverlayList const & overlays )
	{
		auto & size = m_viewport.size();

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
				doDraw( *overlay.second );
			}
		}
	}

	void OverlayRenderer::beginRender( gl::Size2D const & size )
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

	void OverlayRenderer::doDraw( TextOverlay const & overlay )
	{
		uint32_t offset{ 0u };
		auto vertices = overlay.textVertex();
		uint32_t count = uint32_t( vertices.size() );
		auto it = vertices.cbegin();
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

		count = uint32_t( vertices.size() );
		m_program->bind();

		for ( auto & buffer : buffers )
		{
			doDrawBuffer( *buffer
				, std::min( count, m_maxCharsPerBuffer )
				, overlay.transform()
				, overlay.colour()
				, overlay.fontTexture().texture() );
			count -= m_maxCharsPerBuffer;
		}

		m_program->unbind();
	}

	OverlayRenderer::VertexBuffer const & OverlayRenderer::doCreateTextBuffer()
	{
		VertexBuffer buffer
		{
			gl::makeBuffer( gl::BufferTarget::eArrayBuffer
				, std::vector< TextOverlay::Vertex >( m_maxCharsPerBuffer ) ),
			m_program->createAttribute< gl::Vector2D >( "position"
				, sizeof( TextOverlay::Vertex )
				, offsetof( TextOverlay::Vertex, coords ) ),
			m_program->createAttribute< gl::Vector2D >( "texture"
				, sizeof( TextOverlay::Vertex )
				, offsetof( TextOverlay::Vertex, text ) ),
		};
		m_textBuffers.push_back( std::move( buffer ) );
		return m_textBuffers.back();
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
		m_mpUniform->value( m_transform * transform );
		m_colour->value( colour );
		m_mapText->value( 0 );
		m_mpUniform->bind();
		m_colour->bind();
		m_mapText->bind();
		texture.bind( 0 );
		glCheckError( glDrawArrays, GL_TRIANGLES, 0, count );
		texture.unbind( 0 );
		buffer.m_texture->unbind();
		buffer.m_position->unbind();
		buffer.m_vbo->unbind();
	}

	OverlayRenderer::VertexBuffer const & OverlayRenderer::doFillTextPart
		( uint32_t count
		, uint32_t & offset
		, TextOverlay::VertexArray::const_iterator & it
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
			buffer = &m_textBuffers[index];
		}

		auto & vertexBuffer = *buffer->m_vbo;
		count = std::min( count, m_maxCharsPerBuffer );
		doFillBuffers( it, count, offset, vertexBuffer );
		it += count;

		return *buffer;
	}
}
