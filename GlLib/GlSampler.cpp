#include "pch.h"
#include "GlSampler.h"

namespace gl
{
	//*************************************************************************

	namespace
	{
		GLenum doGetWrapMode( WrapMode mode )
		{
			switch ( mode )
			{
			case WrapMode::eClampToEdge:
				return GL_CLAMP_TO_EDGE;

			case WrapMode::eMirroredRepeat:
				return GL_MIRRORED_REPEAT;

			case WrapMode::eRepeat:
				return GL_REPEAT;

			default:
				assert( false && "Unsupported wrap mode" );
				return 0;
			}
		}

		GLenum doGetMinFilter( MinFilter filter )
		{
			switch ( filter )
			{
			case MinFilter::eNearest:
				return GL_NEAREST;

			case MinFilter::eLinear:
				return GL_LINEAR;

			case MinFilter::eNearestMipmapNearest:
				return GL_NEAREST_MIPMAP_NEAREST;

			case MinFilter::eLinearMipmapNearest:
				return GL_LINEAR_MIPMAP_NEAREST;

			case MinFilter::eNearestMipmapLinear:
				return GL_NEAREST_MIPMAP_LINEAR;

			case MinFilter::eLinearMipmapLinear:
				return GL_LINEAR_MIPMAP_LINEAR;

			default:
				assert( false && "Unsupported minification filter" );
				return 0;
			}
		}

		GLenum doGetMagFilter( MagFilter filter )
		{
			switch ( filter )
			{
			case MagFilter::eNearest:
				return GL_NEAREST;

			case MagFilter::eLinear:
				return GL_LINEAR;

			default:
				assert( false && "Unsupported magnification filter" );
				return 0;
			}
		}
	}

	//*************************************************************************

	Sampler::Sampler( WrapMode wrapS
		, WrapMode wrapT
		, MinFilter minFilter
		, MagFilter magFilter )noexcept
		: m_wrapS{ doGetWrapMode( wrapS ) }
		, m_wrapT{ doGetWrapMode( wrapT ) }
		, m_minFilter{ doGetMinFilter( minFilter ) }
		, m_magFilter{ doGetMagFilter( magFilter ) }
	{
	}

	Sampler::~Sampler()noexcept
	{
	}

	void Sampler::bind( uint32_t unit )const noexcept
	{
		glCheckError( glTexParameteri
			, GL_TEXTURE_2D
			, GL_TEXTURE_WRAP_S
			, m_wrapS );
		glCheckError( glTexParameteri
			, GL_TEXTURE_2D
			, GL_TEXTURE_WRAP_T
			, m_wrapT );
		glCheckError( glTexParameteri
			, GL_TEXTURE_2D
			, GL_TEXTURE_MIN_FILTER
			, m_minFilter );
		glCheckError( glTexParameteri
			, GL_TEXTURE_2D
			, GL_TEXTURE_MAG_FILTER
			, m_magFilter );
	}

	void Sampler::unbind( uint32_t unit )const noexcept
	{
	}

	//*************************************************************************

	SamplerObject::SamplerObject( WrapMode wrapS
		, WrapMode wrapT
		, MinFilter minFilter
		, MagFilter magFilter )noexcept
		: Sampler{ wrapS, wrapT, minFilter, magFilter }
	{
		glCheckError( glGenSamplers, 1, &m_name );
		glCheckError( glSamplerParameteri
			, m_name
			, GL_TEXTURE_WRAP_S
			, GL_CLAMP_TO_EDGE );
		glCheckError( glSamplerParameteri
			, m_name
			, GL_TEXTURE_WRAP_T
			, GL_CLAMP_TO_EDGE );
		glCheckError( glSamplerParameteri
			, m_name
			, GL_TEXTURE_MIN_FILTER
			, GL_LINEAR );
		glCheckError( glSamplerParameteri
			, m_name
			, GL_TEXTURE_MAG_FILTER
			, GL_LINEAR );
	}

	SamplerObject::~SamplerObject()noexcept
	{
		if ( m_name != GL_INVALID_INDEX )
		{
			glCheckError( glDeleteSamplers, 1, &m_name );
		}
	}

	void SamplerObject::bind( uint32_t unit )const noexcept
	{
		assert( m_name != GL_INVALID_INDEX );
		glCheckError( glBindSampler, unit, m_name );
	}

	void SamplerObject::unbind( uint32_t unit )const noexcept
	{
		assert( m_name != GL_INVALID_INDEX );
		glCheckError( glBindSampler, unit, 0 );
	}

	//*************************************************************************
}
