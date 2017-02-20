#include "Engine.h"

#include <AndroidUtils/AndroidApp.h>
#include <AndroidUtils/FontLoader.h>

#include <StarMapLib/CsvReader.h>
#include <StarMapLib/XmlReader.h>

//*****************************************************************************

Window::Window( utils::AndroidApp const & parent
	, ANativeWindow * window
	, render::ByteArray const & state )
	: utils::AndroidWindow{ parent, window, state }
	, m_events{ m_onScreenTap
		, m_onScreenDoubleTap
		, m_onScreenSingleMove
		, m_onScreenDoubleMove }
	, m_starmap{ m_events, 5u }
{
	starmap::loadStarsFromXml( m_starmap
		, m_parent.getFileTextContent( "stars.xml", true ) );
	starmap::loadConstellationsFromXml( m_starmap
		, m_parent.getFileTextContent( "constellations.xml", true ) );
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

void Window::onSave( render::ByteArray & state )
{
	state.resize( sizeof( render::CameraState ) );
	m_starmap.save( *reinterpret_cast< render::CameraState * >( state.data() ) );
}

void Window::onRestore( render::ByteArray const & state )
{
	assert( state.size() == sizeof( render::CameraState ) );
	m_starmap.restore( *reinterpret_cast< render::CameraState const * >( state.data() ) );
}

void Window::onSingleTap( gl::Position2D const & position )
{
	m_onScreenTap( position );
}

void Window::onDoubleTap( gl::Position2D const & position )
{
	m_onScreenDoubleTap( position );
}

void Window::onSingleMove( gl::Position2D const & position )
{
	m_onScreenSingleMove( position );
}

void Window::onDoubleMove( gl::Position2D const & posDiff, int distDiff )
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
