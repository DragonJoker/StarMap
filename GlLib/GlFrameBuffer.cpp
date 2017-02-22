#include "pch.h"
#include "GlFrameBuffer.h"

#include "GlRenderBuffer.h"
#include "GlTexture.h"

namespace gl
{
	namespace
	{
		void doCheckStatus()
		{
			static std::map < GLenum, std::string > const statusName
			{
				{ GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT, "Incomplete attachment" },
#if defined GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS
				{ GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS, "Incomplete dimensions" },
#endif
				{ GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT, "Missing attachment" },
				{ GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE, "Incomplete multisample" },
				{ GL_FRAMEBUFFER_UNSUPPORTED, "Unsupported" },
			};
			auto status = glCheckFramebufferStatus( GL_FRAMEBUFFER );

			if ( status != GL_FRAMEBUFFER_COMPLETE )
			{
				std::cerr << "Framebuffer attachment error: " << statusName.at( status ) << std::endl;
			}
		}

		GLenum doGetAttachmentPoint( AttachmentPoint point )
		{
			switch ( point )
			{
			case AttachmentPoint::eDepth:
				return GL_DEPTH_ATTACHMENT;

			case AttachmentPoint::eColour0:
				return GL_COLOR_ATTACHMENT0;

			default:
				assert( false && "Unsupported attachment point." );
				return GL_COLOR_ATTACHMENT0;
				break;
			}
		}
	}

	FrameBuffer::FrameBuffer()noexcept
	{
		glCheckError( glGenFramebuffers, 1, &m_name );
	}

	FrameBuffer::~FrameBuffer()noexcept
	{
		glCheckError( glDeleteFramebuffers, 1, &m_name );
	}

	void FrameBuffer::bind()const noexcept
	{
		GlLib_DebugAssertFalse( m_bound );
		assert( m_name != GL_INVALID_INDEX );
		glCheckError( glBindFramebuffer, GL_FRAMEBUFFER, m_name );
		GlLib_DebugSetTrue( m_bound );
	}

	void FrameBuffer::unbind()const noexcept
	{
		GlLib_DebugAssertTrue( m_bound );
		glCheckError( glBindFramebuffer, GL_FRAMEBUFFER, 0u );
		GlLib_DebugSetFalse( m_bound );
	}

	void FrameBuffer::clear( RgbaColour const & colour
		, float depth
		, int stencil )const noexcept
	{
		//GlLib_DebugAssertTrue( m_bound );
		glCheckError( glClearColor, colour.r, colour.g, colour.b, colour.a );
		glCheckError( glClearDepthf, depth );
		glCheckError( glClearStencil, stencil );
		glCheckError( glClear, GL_COLOR_BUFFER_BIT
			| GL_DEPTH_BUFFER_BIT
			| GL_STENCIL_BUFFER_BIT );
	}

	void FrameBuffer::attach( Texture const & texture
		, AttachmentPoint point )const noexcept
	{
		GlLib_DebugAssertTrue( m_bound );

		if ( m_size.x == 0
			&& m_size.y == 0 )
		{
			m_size = texture.dimensions();
		}

		assert( texture.dimensions().x == m_size.x );
		assert( texture.dimensions().y == m_size.y );
		assert( texture.glName() != GL_INVALID_INDEX );
		glCheckError( glFramebufferTexture2D
			,GL_FRAMEBUFFER
			, doGetAttachmentPoint( point )
			, GL_TEXTURE_2D
			, texture.glName(), 0 );
		doCheckStatus();
	}

	void FrameBuffer::detach( Texture const & texture
		, AttachmentPoint point )const noexcept
	{
		GlLib_DebugAssertTrue( m_bound );
		glCheckError( glFramebufferTexture2D
			, GL_FRAMEBUFFER
			, doGetAttachmentPoint( point )
			, GL_TEXTURE_2D
			, 0
			, 0 );
	}

	void FrameBuffer::attach( RenderBuffer const & buffer
		, AttachmentPoint point )const noexcept
	{
		GlLib_DebugAssertTrue( m_bound );

		if ( m_size.x == 0
			&& m_size.y == 0 )
		{
			m_size = buffer.dimensions();
		}

		assert( buffer.dimensions().x == m_size.x );
		assert( buffer.dimensions().y == m_size.y );
		assert( buffer.glName() != GL_INVALID_INDEX );
		glCheckError( glFramebufferRenderbuffer
			, GL_FRAMEBUFFER
			, doGetAttachmentPoint( point )
			, GL_RENDERBUFFER
			, buffer.glName() );
		doCheckStatus();
	}

	void FrameBuffer::detach( RenderBuffer const & buffer
		, AttachmentPoint point )const noexcept
	{
		GlLib_DebugAssertTrue( m_bound );
		glCheckError( glFramebufferRenderbuffer
			, GL_FRAMEBUFFER
			, doGetAttachmentPoint( point )
			, GL_RENDERBUFFER, 0 );
	}

	void FrameBuffer::download( uint32_t xoffset
		, uint32_t yoffset
		, uint32_t width
		, uint32_t height
		, PixelFormat format
		, uint8_t * data )const noexcept
	{
		GlLib_DebugAssertTrue( m_bound );
		assert( xoffset + width <= uint32_t( m_size.x ) );
		assert( yoffset + height <= uint32_t( m_size.y ) );
		glCheckError( glReadPixels, xoffset
			, yoffset
			, width
			, height
			, glFormat( format )
			, glType( format )
			, data );
	}
}
