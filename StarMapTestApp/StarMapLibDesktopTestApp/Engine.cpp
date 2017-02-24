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
#include <StarMapLib/StarMapState.h>
#include <StarMapLib/XmlReader.h>

Window::Window()
	: MsWindow{}
	, m_events{ m_onScreenTap
		, m_onScreenDoubleTap
		, m_onScreenSingleMove
		, m_onScreenDoubleMove }
{
	std::string const appName = "RenderLibTestApp";
	m_cout = new render::LogStreambuf< utils::InfoLogStreambufTraits >( appName
		, std::cout );
	m_cerr = new render::LogStreambuf< utils::ErrorLogStreambufTraits >( appName
		, std::cerr );
	m_clog = new render::LogStreambuf< utils::DebugLogStreambufTraits >( appName
		, std::clog );
}

Window::~Window()
{
	delete m_cout;
	delete m_cerr;
	delete m_clog;
}

void Window::onDraw()
{
	if ( m_starmap )
	{
		m_starmap->beginFrame();
		m_starmap->drawFrame();
		doSwapBuffers();
		m_starmap->endFrame();
	}
}

void Window::onCreate()
{
	std::string dataPath{ "arial.ttf" };
	m_starmap = std::make_unique< starmap::StarMap >( m_events
		, 5u
		, size()
		, utils::FontLoader{ dataPath }
		, utils::getFileBinaryContent( "halo.bmp" ) );

	try
	{
		starmap::loadStarsFromXml( *m_starmap
			, utils::getFileTextContent( "stars.xml" ) );
		starmap::loadConstellationsFromXml( *m_starmap
			, utils::getFileTextContent( "constellations.xml" ) );
		m_starmap->initialise();
	}
	catch ( std::exception & exc )
	{
		std::cerr << "Csv loading failed: " << exc.what() << std::endl;
	}
}

void Window::onDestroy()
{
	m_starmap->cleanup();
}

void Window::onMinimise()
{
	m_starmap->save( m_savedState );
	m_starmap.reset();
}

void Window::onRestore( gl::IVec2 const & event )
{
	std::string dataPath{ "arial.ttf" };
	m_starmap = std::make_unique< starmap::StarMap >( m_events
		, 5u
		, size()
		, utils::FontLoader{ dataPath }
	, utils::getFileBinaryContent( "halo.bmp" ) );
	m_starmap->restore( m_savedState );
	m_starmap->initialise();
}

void Window::onResize( gl::IVec2 const & event )
{
	m_starmap->resize( event );
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
