#include "stdafx.h"

#include "Engine.h"

#include <array>
#include <chrono>
#include <random>
#include <string>

#include <DesktopUtils/FontLoader.h>
#include <DesktopUtils/ObjLoader.h>
#include <DesktopUtils/Log.h>

#include <GL/wglew.h>

//*****************************************************************************

namespace
{
#if !defined( NDEBUG )

	static constexpr int GL_CONTEXT_CREATION_DEFAULT_FLAGS = WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB | WGL_CONTEXT_DEBUG_BIT_ARB;
	static constexpr int GL_CONTEXT_CREATION_DEFAULT_MASK = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;

#else

	static constexpr int GL_CONTEXT_CREATION_DEFAULT_FLAGS = WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB;
	static constexpr int GL_CONTEXT_CREATION_DEFAULT_MASK = WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB;

#endif

	constexpr double M_PI = 3.14159265358979323846;

	std::string doGetExecutableDir()
	{
		std::string ret;
		ret.resize( 1024 );
		DWORD count{ 1023u };
		count = ::GetModuleFileNameA( NULL, &ret[0], count );
		ret.resize( count + 1 );
		ret = ret.substr( 0, ret.find_last_of( "\\" ) );
		return ret;
	}
}

//*****************************************************************************

Engine::Engine()
	: MsWindow{}
	, m_events{ m_onScreenTap
		, m_onScreenDoubleTap
		, m_onScreenSingleMove
		, m_onScreenDoubleMove }
	, m_starmap{ m_events }
{
	std::string const appName = "RenderLibTestApp";
	m_cout = new render::LogStreambuf< utils::InfoLogStreambufTraits >( appName
		, std::cout );
	m_cerr = new render::LogStreambuf< utils::ErrorLogStreambufTraits >( appName
		, std::cerr );
	m_clog = new render::LogStreambuf< utils::DebugLogStreambufTraits >( appName
		, std::clog );

	::SetCurrentDirectoryA( doGetExecutableDir().c_str() );

	glm::vec3 colours[]
	{
		glm::vec3{ 1.0, 1.0, 1.0 },
		glm::vec3{ 0.7, 1.0, 1.0 },
		glm::vec3{ 1.0, 0.7, 1.0 },
		glm::vec3{ 1.0, 1.0, 0.7 },
		glm::vec3{ 0.7, 0.7, 1.0 },
		glm::vec3{ 1.0, 0.7, 0.7 },
		glm::vec3{ 0.7, 1.0, 0.7 },
		glm::vec3{ 0.8, 1.0, 1.0 },
		glm::vec3{ 1.0, 0.8, 1.0 },
		glm::vec3{ 1.0, 1.0, 0.8 },
		glm::vec3{ 0.8, 0.8, 1.0 },
		glm::vec3{ 1.0, 0.8, 0.8 },
		glm::vec3{ 0.8, 1.0, 0.8 },
		glm::vec3{ 0.9, 1.0, 1.0 },
		glm::vec3{ 1.0, 0.9, 1.0 },
		glm::vec3{ 1.0, 1.0, 0.9 },
		glm::vec3{ 0.9, 0.9, 1.0 },
		glm::vec3{ 1.0, 0.9, 0.9 },
		glm::vec3{ 0.9, 1.0, 0.9 },
	};

	std::mt19937 engine( std::chrono::system_clock::now().time_since_epoch().count() );
	std::uniform_real_distribution< float > fdistribution{ float( -M_PI ) / 2, float( M_PI ) / 2 };
	std::uniform_real_distribution< float > mdistribution{ -50.0f, 50.0f };
	std::uniform_int_distribution< uint32_t > uidistribution{ 0u, uint32_t( ( sizeof( colours ) / sizeof( glm::vec3 ) ) - 1 ) };

	auto randf = [&engine, &fdistribution]()
	{
		return fdistribution( engine );
	};

	auto randm = [&engine, &mdistribution]()
	{
		return mdistribution( engine ) + 1.0f;
	};

	auto randui = [&engine, &uidistribution]()
	{
		return uidistribution( engine );
	};

	for ( uint32_t i = 0; i < 5000; ++i )
	{
		m_starmap.add( { "Coin"
			, randm()
			,{ randf(), randf() }
		,colours[randui()] } );
	}
}

Engine::~Engine()
{
	delete m_cout;
	delete m_cerr;
	delete m_clog;
}

void Engine::onUpdate( HDC hdc )
{
	if ( m_animating )
	{
		wglMakeCurrent( hdc, m_context );
		m_starmap.beginFrame();
		m_starmap.drawFrame();
		::SwapBuffers( m_hdc );
		m_starmap.endFrame();
		wglMakeCurrent( hdc, 0 );
	}
}

void Engine::onPaint( HDC hdc )
{
	onUpdate( hdc );
}

void Engine::onCreate()
{
	if ( !doInitialiseDisplay() )
	{
		m_animating = true;
	}
}

void Engine::onDestroy()
{
	if ( m_context != INVALID_HANDLE_VALUE )
	{
		wglMakeCurrent( m_hdc, m_context );
		m_starmap.cleanup();
		wglMakeCurrent( m_hdc, 0 );
		ReleaseDC( getHwnd(), m_hdc );
		wglDeleteContext( m_context );
	}

	m_animating = false;
	m_hdc = NULL;
	m_context = NULL;
}

void Engine::onResize( glm::ivec2 const & event )
{
	if ( m_size != event )
	{
		m_size = event;
		m_starmap.resize( m_size );
	}
}

void Engine::onMouseMove( utils::MouseEvent const & event )
{
	if ( event.ldown )
	{
		auto diff = event.position - m_mouse;
		m_onScreenSingleMove( diff );
		m_mouse = event.position;
	}
}

void Engine::onLButtonDown( utils::MouseEvent const & event )
{
	m_mouse = event.position;
}

void Engine::onLButtonClick( utils::MouseEvent const & event )
{
	m_onScreenTap( event.position );
}

void Engine::onLButtonDblClick( utils::MouseEvent const & event )
{
	m_onScreenDoubleTap( event.position );
}

void Engine::onMouseWheel( utils::MouseEvent const & event )
{
	m_onScreenDoubleMove( event.position, -float( event.delta ) );
}

void Engine::onGainedFocus()
{
	m_animating = true;
}

void Engine::onLostFocus()
{
	m_animating = false;
}

int Engine::doInitialiseDisplay()
{
	// Initialisation d'OpenGL
	m_hdc = ::GetDC( getHwnd() );

	if ( !m_hdc )
	{
		std::cout << "HDC creation failed." << std::endl;
		return -1;
	}

	PIXELFORMATDESCRIPTOR pfd = { 0 };
	pfd.nSize = sizeof( PIXELFORMATDESCRIPTOR );
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.cColorBits = 32;

	auto count = ::ChoosePixelFormat( m_hdc, &pfd );

	if ( !count )
	{
		std::cerr << "Pixel format selection failed." << std::endl;
		return -1;
	}

	if ( ::SetPixelFormat( m_hdc, count, &pfd ) == FALSE )
	{
		std::cerr << "Pixel format selection failed." << std::endl;
		return -1;
	}

	m_context = wglCreateContext( m_hdc );

	if ( !m_context )
	{
		std::cerr << "Context creation failed." << std::endl;
		return -1;
	}

	RECT rect;
	::GetClientRect( getHwnd(), &rect );
	m_size.x = rect.right - rect.left;
	m_size.y = rect.bottom - rect.top;

	wglMakeCurrent( m_hdc, m_context );

	auto err = glewInit();

	if ( err != GLEW_NO_ERROR )
	{
		std::cout << "GLEW initialisation failed: " << glewGetErrorString( err ) << std::endl;
		return err;
	}

	HGLRC( CALLBACK *glCreateContextAttribs )( HDC, HGLRC, int const * );
	glCreateContextAttribs = reinterpret_cast< decltype( glCreateContextAttribs ) >( wglGetProcAddress( "wglCreateContextAttribsARB" ) );

	if ( glCreateContextAttribs )
	{
		HGLRC context = m_context;
		int major = 3;
		int minor = 2;
		std::vector< int > attribList
		{
			WGL_CONTEXT_MAJOR_VERSION_ARB, major,
			WGL_CONTEXT_MINOR_VERSION_ARB, minor,
			WGL_CONTEXT_FLAGS_ARB, GL_CONTEXT_CREATION_DEFAULT_FLAGS,
			WGL_CONTEXT_PROFILE_MASK_ARB, GL_CONTEXT_CREATION_DEFAULT_MASK,
			0
		};
		m_context = glCreateContextAttribs( m_hdc, nullptr, attribList.data() );
		wglMakeCurrent( m_hdc, 0 );
		wglDeleteContext( context );
		wglMakeCurrent( m_hdc, m_context );
	}

	// Initialisation de l'état GL.
	glCheckError( glEnable, GL_CULL_FACE );
	glCheckError( glEnable, GL_TEXTURE_2D );
	glCheckError( glFrontFace, GL_CCW );

	std::string dataPath{ doGetExecutableDir() + "\\arial.ttf" };
	uint32_t size = 32;
	m_starmap.initialise( m_size
		, utils::getFileBinaryContent( "halo.bmp" )
		, utils::FontLoader{ dataPath, size } );

	wglMakeCurrent( m_hdc, 0 );
	return 0;
}
