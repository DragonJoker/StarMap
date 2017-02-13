#include "Engine.h"

#include <AndroidUtils/AndroidApp.h>
#include <AndroidUtils/FontLoader.h>

#include <StarMapLib/XmlReader.h>

//*****************************************************************************

Window::Window( utils::AndroidApp const & parent
	, ANativeWindow * window
	, void * saved
	, size_t savedSize )
	: utils::AndroidWindow{ parent, window, saved, savedSize }
	, m_events{ m_onScreenTap
		, m_onScreenDoubleTap
		, m_onScreenSingleMove
		, m_onScreenDoubleMove }
	, m_starmap{ m_events }
{
	if ( !saved )
	{
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

		//std::mt19937 engine( std::chrono::system_clock::now().time_since_epoch().count() );
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
		//		,colours[randui()] } );
		//}

		starmap::loadStarsFromXml( m_starmap
			, m_parent.getFileTextContent( "stars.xml", true ) );
		starmap::loadConstellationsFromXml( m_starmap
			, m_parent.getFileTextContent( "constellations.xml", true ) );
	}
}

Window::~Window()
{
}

void Window::onCreate()
{
	auto loader = doCreateFontLoader( "arial.ttf" );

	if ( loader )
	{
		m_starmap.initialise( m_size
			, m_parent.getFileBinaryContent( "halo.bmp", true )
			, *loader );
	}
}

void Window::onDestroy()
{
	m_starmap.cleanup();
}

void Window::onDraw()
{
	m_starmap.beginFrame();
	m_starmap.drawFrame();
	eglSwapBuffers( m_display, m_surface );
	m_starmap.endFrame();
}

void Window::onSave( void * state, size_t & stateSize )
{
	auto camState = new render::CameraState;
	m_starmap.save( *camState);
	state = camState;
	stateSize = sizeof( render::CameraState );
}

void Window::onRestore( void const * state, size_t stateSize )
{
	assert( stateSize == sizeof( render::CameraState ) );
	m_starmap.restore( *reinterpret_cast< render::CameraState const * >( state ) );
}

void Window::onSingleTap( glm::ivec2 const & position )
{
	m_onScreenTap( position );
}

void Window::onDoubleTap( glm::ivec2 const & position )
{
	m_onScreenDoubleTap( position );
}

void Window::onSingleMove( glm::ivec2 const & position )
{
	m_onScreenSingleMove( position );
}

void Window::onDoubleMove( glm::ivec2 const & posDiff, int distDiff )
{
	m_onScreenDoubleMove( posDiff, distDiff );
}

//*****************************************************************************

int32_t Engine::handleInput( android_app * app, AInputEvent * event )
{
	int32_t ret = 0;
	auto engine = reinterpret_cast< utils::AndroidApp * >( app->userData );

	switch ( AInputEvent_getType( event ) )
	{
	case AINPUT_EVENT_TYPE_MOTION:
		if ( AMotionEvent_getPointerCount( event ) == 1 )
		{
			ret = engine->onSingleTouchEvent( event );
		}
		else
		{
			ret = engine->onMultiTouchEvent( event );
		}
		break;
	}

	return ret;
}

void Engine::handleCommand( android_app * app, int32_t cmd )
{
	auto engine = reinterpret_cast< utils::AndroidApp * >( app->userData );

	switch ( cmd )
	{
	case APP_CMD_INIT_WINDOW:
		// La fenêtre est affichée : opération de préparation.
		engine->onInitWindow( app->window
			, app->savedState
			, app->savedStateSize );
		break;

	case APP_CMD_TERM_WINDOW:
		// La fenêtre est masquée ou fermée : opération de nettoyage.
		engine->onTermWindow();
		break;

	case APP_CMD_GAINED_FOCUS:
		// Quand l'application obtient le focus, la surveillance de l'accéléromètre est démarrée.
		engine->onGainedFocus();
		break;

	case APP_CMD_LOST_FOCUS:
		// Quand l'application perd le focus, la surveillance de l'accéléromètre est arrêtée.
		// Cela évite de décharger la batterie quand elle n'est pas utilisée.
		engine->onLostFocus();
		break;

	case APP_CMD_CONFIG_CHANGED:
		engine->onUpdateConfig();
		break;

	case APP_CMD_SAVE_STATE:
		// Le système demande d'enregistrer l'état actuel. Cette opération est effectuée.
		engine->onSave( app->savedState, app->savedStateSize );
		break;
	}
}

//*****************************************************************************
