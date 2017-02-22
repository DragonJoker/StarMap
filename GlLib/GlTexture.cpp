#include "pch.h"
#include "GlTexture.h"

namespace gl
{
	Texture::Texture( PixelFormat format
		, IVec2 const & size )noexcept
		: Texture{ format, size, ByteArray{} }
	{
	}

	Texture::Texture( PixelFormat format
		, IVec2 const & size
		, ByteArray const & data )noexcept
		: m_type{ glType( format ) }
		, m_format{ glFormat( format ) }
		, m_size{ size }
	{
		glCheckError( glGenTextures, 1, &m_name );
		glCheckError( glBindTexture, GL_TEXTURE_2D, m_name );
		glCheckError( glTexImage2D
			, GL_TEXTURE_2D
			, 0
			, m_format
			, m_size.x
			, m_size.y
			, 0
			, m_format
			, m_type
			, data.empty() ? nullptr : data.data() );
		glCheckError( glBindTexture, GL_TEXTURE_2D, 0 );
	}

	Texture::~Texture()noexcept
	{
		glCheckError( glDeleteTextures, 1, &m_name );
	}

	void Texture::bind( uint32_t unit )const noexcept
	{
		GlLib_DebugAssertFalse( m_bound );
		assert( m_name != GL_INVALID_INDEX );
		glCheckError( glActiveTexture, GL_TEXTURE0 + unit );
		glCheckError( glBindTexture, GL_TEXTURE_2D, m_name );
		GlLib_DebugSetTrue( m_bound );
	}

	void Texture::unbind( uint32_t unit )const noexcept
	{
		GlLib_DebugAssertTrue( m_bound );
		glCheckError( glActiveTexture, GL_TEXTURE0 + unit );
		glCheckError( glBindTexture, GL_TEXTURE_2D, 0u );
		GlLib_DebugSetFalse( m_bound );
	}

	void Texture::generateMipmaps()const noexcept
	{
		GlLib_DebugAssertTrue( m_bound );
		glCheckError( glGenerateMipmap, GL_TEXTURE_2D );
	}

	void Texture::upload( uint32_t xoffset
		, uint32_t yoffset
		, uint32_t width
		, uint32_t height
		, uint8_t const * data )const noexcept
	{
		GlLib_DebugAssertTrue( m_bound );
		assert( xoffset + width <= uint32_t( m_size.x ) );
		assert( yoffset + height <= uint32_t( m_size.y ) );
		glCheckError( glTexSubImage2D
			, GL_TEXTURE_2D
			, 0
			, xoffset
			, yoffset
			, width
			, height
			, m_format
			, m_type
			, data );
	}
}
