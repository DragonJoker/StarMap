#include "GlRenderBuffer.h"

namespace gl
{
	namespace
	{
		GLenum doGetFormat( PixelFormat format )
		{
			switch ( format )
			{
			case PixelFormat::eRGB565:
				return GL_RGB565;

			case PixelFormat::eRGBA5551:
				return GL_RGB5_A1;

			case PixelFormat::eRGBA4444:
				return GL_RGBA4;

			case PixelFormat::eD16:
				return GL_DEPTH_COMPONENT16;

			case PixelFormat::eS8:
				return GL_STENCIL_INDEX8;

			default:
				assert( false && "Unsupported pixel format for render buffer" );
				return 0;
			}
		}
	}

	RenderBuffer::RenderBuffer( PixelFormat format
		, Size2D const & size )noexcept
		: m_format{ doGetFormat( format ) }
		, m_size{ size }
	{
		glCheckError( glGenRenderbuffers, 1, &m_name );
		glCheckError( glBindRenderbuffer, GL_RENDERBUFFER, m_name );
		glCheckError( glRenderbufferStorage
			, GL_RENDERBUFFER
			, m_format
			, m_size.x
			, m_size.y );
		glCheckError( glBindRenderbuffer, GL_RENDERBUFFER, 0 );
	}

	RenderBuffer::~RenderBuffer()noexcept
	{
		glCheckError( glDeleteRenderbuffers, 1, &m_name );
	}
}
