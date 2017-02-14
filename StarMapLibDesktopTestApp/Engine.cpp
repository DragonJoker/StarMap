#include "stdafx.h"

#include "Engine.h"

#include <array>
#include <chrono>
#include <random>
#include <string>

#include <DesktopUtils/FontLoader.h>
#include <DesktopUtils/ObjLoader.h>
#include <DesktopUtils/Log.h>

#include <StarMapLib/CsvReader.h>
#include <StarMapLib/XmlReader.h>

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

Window::Window()
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

	try
	{
		starmap::loadStarsFromXml( m_starmap
			, utils::getFileTextContent( "stars.xml" ) );
		starmap::loadConstellationsFromXml( m_starmap
			, utils::getFileTextContent( "constellations.xml" ) );
	}
	catch ( std::exception & exc )
	{
		std::cerr << "Csv loading failed: " << exc.what() << std::endl;
	}
}

Window::~Window()
{
	delete m_cout;
	delete m_cerr;
	delete m_clog;
}

void Window::onDraw()
{
	m_starmap.beginFrame();
	m_starmap.drawFrame();
	doSwapBuffers();
	m_starmap.endFrame();
}

void Window::onCreate()
{
	std::string dataPath{ doGetExecutableDir() + "\\arial.ttf" };
	m_starmap.initialise( size()
		, utils::getFileBinaryContent( "halo.bmp" )
		, utils::FontLoader{ dataPath } );
}

void Window::onDestroy()
{
	m_starmap.cleanup();
}

void Window::onResize( gl::Size2D const & event )
{
	m_starmap.resize( event );
}

void Window::onMouseMove( utils::MouseEvent const & event )
{
	if ( event.ldown )
	{
		auto diff = event.position - m_mouse;
		m_onScreenSingleMove( diff );
		m_mouse = event.position;
	}
}

void Window::onLButtonDown( utils::MouseEvent const & event )
{
	m_mouse = event.position;
}

void Window::onLButtonClick( utils::MouseEvent const & event )
{
	m_onScreenTap( event.position );
}

void Window::onLButtonDblClick( utils::MouseEvent const & event )
{
	m_onScreenDoubleTap( event.position );
}

void Window::onMouseWheel( utils::MouseEvent const & event )
{
	m_onScreenDoubleMove( event.position, -float( event.delta ) );
}
