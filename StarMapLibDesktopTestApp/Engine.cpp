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

	//glm::vec3 colours[]
	//{
	//	glm::vec3{ 1.0, 1.0, 1.0 },
	//	glm::vec3{ 0.7, 1.0, 1.0 },
	//	glm::vec3{ 1.0, 0.7, 1.0 },
	//	glm::vec3{ 1.0, 1.0, 0.7 },
	//	glm::vec3{ 0.7, 0.7, 1.0 },
	//	glm::vec3{ 1.0, 0.7, 0.7 },
	//	glm::vec3{ 0.7, 1.0, 0.7 },
	//	glm::vec3{ 0.8, 1.0, 1.0 },
	//	glm::vec3{ 1.0, 0.8, 1.0 },
	//	glm::vec3{ 1.0, 1.0, 0.8 },
	//	glm::vec3{ 0.8, 0.8, 1.0 },
	//	glm::vec3{ 1.0, 0.8, 0.8 },
	//	glm::vec3{ 0.8, 1.0, 0.8 },
	//	glm::vec3{ 0.9, 1.0, 1.0 },
	//	glm::vec3{ 1.0, 0.9, 1.0 },
	//	glm::vec3{ 1.0, 1.0, 0.9 },
	//	glm::vec3{ 0.9, 0.9, 1.0 },
	//	glm::vec3{ 1.0, 0.9, 0.9 },
	//	glm::vec3{ 0.9, 1.0, 0.9 },
	//};

	//std::mt19937 engine{ uint32_t( std::chrono::system_clock::now().time_since_epoch().count() ) };
	//std::uniform_real_distribution< float > fdistribution{ float( -M_PI ) / 2, float( M_PI ) / 2 };
	//std::uniform_real_distribution< float > mdistribution{ -50.0f, 50.0f };
	//std::uniform_int_distribution< uint32_t > uidistribution{ 0u, uint32_t( ( sizeof( colours ) / sizeof( glm::vec3 ) ) - 1 ) };

	//auto randf = [&engine, &fdistribution]()
	//{
	//	return fdistribution( engine );
	//};

	//auto randm = [&engine, &mdistribution]()
	//{
	//	return mdistribution( engine ) + 1.0f;
	//};

	//auto randui = [&engine, &uidistribution]()
	//{
	//	return uidistribution( engine );
	//};

	//for ( uint32_t i = 0; i < 5000; ++i )
	//{
	//	m_starmap.add( { "Coin"
	//		, randm()
	//		,{ randf(), randf() }
	//	,colours[randui()] } );
	//}

	try
	{
		auto stars = starmap::loadFromCsv( utils::getFileTextContent( "stars.csv" ) );
		m_starmap.add( stars );
	}
	catch ( std::exception & exc )
	{
		std::cerr << "Csv loading failed: " << exc.what() << std::endl;
	}
}

Engine::~Engine()
{
	delete m_cout;
	delete m_cerr;
	delete m_clog;
}

void Engine::onUpdate()
{
	m_starmap.beginFrame();
	m_starmap.drawFrame();
	doSwapBuffers();
	m_starmap.endFrame();
}

void Engine::onPaint()
{
	onUpdate();
}

void Engine::onCreate()
{
	std::string dataPath{ doGetExecutableDir() + "\\arial.ttf" };
	uint32_t size = 32;
	m_starmap.initialise( getSize()
		, utils::getFileBinaryContent( "halo.bmp" )
		, utils::FontLoader{ dataPath, size } );
}

void Engine::onDestroy()
{
	m_starmap.cleanup();
}

void Engine::onResize( glm::ivec2 const & event )
{
	m_starmap.resize( event );
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
