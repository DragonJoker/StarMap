#include "pch.h"
#include "OpenGL.h"
#include "GlDebug.h"

#include "GlSampler.h"

namespace gl
{
#if !defined( NDEBUG )

	Debug OpenGL::m_debug;

#endif

	FeatureLevel OpenGL::m_level{ FeatureLevel::eUndefined };
	std::string OpenGL::m_version;

	bool OpenGL::initialise()
	{
		auto string = glGetString( GL_VERSION );
		m_version = reinterpret_cast< char const * >( string );
		std::clog << "OpenGL:" << std::endl;
		std::clog << "    Version: " << m_version << std::endl;
		assert( !m_version.empty() );

		if ( m_version.find( "OpenGL ES 3" ) != std::string::npos )
		{
			m_level = FeatureLevel::eGLES3;
		}
		else if ( m_version.find( "OpenGL ES 2" ) != std::string::npos )
		{
			m_level = FeatureLevel::eGLES2;
		}
		else
		{
			// Regular OpenGL.
			double dversion{ 0u };
			std::stringstream stream( m_version );
			stream >> dversion;
			int iversion = int( dversion * 10 );

			if ( iversion > 30 )
			{
				m_level = FeatureLevel::eGLES3;
			}
			else
			{
				m_level = FeatureLevel::eGLES2;
			}

			m_version = "OpenGL " + m_version;
		}

		string = glGetString( GL_VENDOR );
		auto vendor = reinterpret_cast< char const * >( string );
		std::clog << "    Vendor: " << vendor << std::endl;

		string = glGetString( GL_RENDERER );
		auto renderer = reinterpret_cast< char const * >( string );
		std::clog << "    Renderer: " << renderer << std::endl;

		auto result = m_level != FeatureLevel::eUndefined;

#if _WIN32

		glewExperimental = GL_TRUE;
		auto err = glewInit();

		if ( err != GLEW_NO_ERROR )
		{
			result = false;
			std::cerr << "GLEW initialisation failed: " << glewGetErrorString( err ) << std::endl;
		}

#endif
#if !defined( NDEBUG )

		m_debug.initialise();

#endif

		return result;
	}

	bool OpenGL::hasInstancing()noexcept
	{
		return checkSupport( FeatureLevel::eGLES3 );
	}

	SamplerPtr OpenGL::createSampler( WrapMode wrapS
		, WrapMode wrapT
		, MinFilter minFilter
		, MagFilter magFilter )
	{
		if ( m_level >= FeatureLevel::eGLES3 )
		{
			return std::make_unique< SamplerObject >( wrapS
				, wrapT
				, minFilter
				, magFilter );
		}
		else
		{
			return std::make_unique< Sampler >( wrapS
				, wrapT
				, minFilter
				, magFilter );
		}
	}
}
