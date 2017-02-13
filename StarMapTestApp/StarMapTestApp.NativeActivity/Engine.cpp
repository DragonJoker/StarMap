#include "Engine.h"

#include <array>
#include <chrono>
#include <string>

#include <android/configuration.h>
#include <android/window.h>

#include <AndroidUtils/AndroidApp.h>
#include <AndroidUtils/FontLoader.h>
#include <AndroidUtils/Log.h>
#include <AndroidUtils/AndroidUtils.h>

#include <StarMapLib/CsvReader.h>
#include <StarMapLib/XmlReader.h>

#define ALLOW_OPENGL_ES_2 1

#ifndef EGL_OPENGL_ES3_BIT
#	define EGL_OPENGL_ES3_BIT 0x00000040
#endif

//*****************************************************************************

Engine::Engine( android_app * state )
	: m_app{ state }
	, m_tapDetector{ state->config }
	, m_doubleTapDetector{ state->config }
	, m_moveDetector{ state->config }
	, m_dblMoveDetector{ state->config }
	, m_events{ m_onScreenTap
		, m_onScreenDoubleTap
		, m_onScreenSingleMove
		, m_onScreenDoubleMove }
	, m_starmap{ m_events }
{
	std::string const appName = "StarMapLibTestApp";
	m_cout = new render::LogStreambuf< utils::InfoLogStreambufTraits >( appName
		, std::cout );
	m_cerr = new render::LogStreambuf< utils::ErrorLogStreambufTraits >( appName
		, std::cerr );
	m_clog = new render::LogStreambuf< utils::DebugLogStreambufTraits >( appName
		, std::clog );

	// Préparation de la surveillance de l'accéléromètre
	auto sensorManager = ASensorManager_getInstance();
	m_accelerometer = ASensorManager_getDefaultSensor( sensorManager
		, ASENSOR_TYPE_ACCELEROMETER );
	m_gyroscope = ASensorManager_getDefaultSensor( sensorManager
		, ASENSOR_TYPE_GYROSCOPE );
	m_sensorEventQueue = ASensorManager_createEventQueue( sensorManager
		, state->looper
		, LOOPER_ID_USER
		, nullptr
		, nullptr );


	if ( state->savedState )
	{
		// Un état enregistré précédent est utilisé pour procéder à la restauration.
		assert( state->savedStateSize == sizeof( render::CameraState ) );
		m_starmap.restore( *reinterpret_cast< render::CameraState * >( state->savedState ) );
	}
	else
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

		auto engine = reinterpret_cast< utils::AndroidApp * >( m_app->userData );
		starmap::loadStarsFromXml( m_starmap
			, engine->getFileTextContent( "stars.xml", true ) );
		starmap::loadConstellationsFromXml( m_starmap
			, engine->getFileTextContent( "constellations.xml", true ) );
	}

	ANativeActivity_setWindowFlags( m_app->activity
		, AWINDOW_FLAG_FULLSCREEN | AWINDOW_FLAG_KEEP_SCREEN_ON
		, 0 );
}

Engine::~Engine()
{
	delete m_cout;
	delete m_cerr;
	delete m_clog;
}

void Engine::handleUserEvents()
{
	ASensorEvent event;

	while ( ASensorEventQueue_getEvents( m_sensorEventQueue
		, &event
		, 1 ) > 0 )
	{
		switch ( event.sensor )
		{
		case ASENSOR_TYPE_ACCELEROMETER:
			if ( m_accelerometer )
			{
				//std::cout << "accelerometer: x=" << event.acceleration.x
				//	<< " y=" << event.acceleration.y
				//	<< " z=" << event.acceleration.z
				//	<< std::endl;
			}
			break;

		case ASENSOR_TYPE_GYROSCOPE:
			if ( m_gyroscope )
			{
				//m_state.setVelocity( { event.vector.x, event.vector.y } );
			}
			break;
		}
	}
}

void Engine::cleanup()
{
	onTermWindow();
}

void Engine::update()
{
	if ( m_animating )
	{
		// Événements terminés ; dessin de la prochaine image d'animation suivante.
		// Le dessin étant limité à la fréquence de mise à jour de l'écran,
		// aucune temporisation n'est nécessaire.
		doDrawFrame();
	}
}

int32_t Engine::handleInput( android_app * app, AInputEvent * event )
{
	int32_t ret = 0;
	Engine * engine = reinterpret_cast< Engine * >( app->userData );

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
	Engine * engine = reinterpret_cast< Engine * >( app->userData );

	switch ( cmd )
	{
	case APP_CMD_INIT_WINDOW:
		// La fenêtre est affichée : opération de préparation.
		engine->onInitWindow();
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
		engine->onSaveState();
		break;
	}
}

void Engine::onSaveState()
{
	auto state = new render::CameraState;
	m_starmap.save( *state );
	m_app->savedState = state;
	m_app->savedStateSize = sizeof( render::CameraState );
}

void Engine::onInitWindow()
{
	if ( m_app->window )
	{
		if ( !doInitialiseDisplay() )
		{
			doDrawFrame();
			m_animating = true;
		}
	}
}

void Engine::onUpdateConfig()
{
	if ( m_app->window
		&& ( m_width != ANativeWindow_getWidth( m_app->window )
			|| m_height != ANativeWindow_getHeight( m_app->window ) ) )
	{
		handleCommand( m_app, APP_CMD_TERM_WINDOW );
		handleCommand( m_app, APP_CMD_INIT_WINDOW );
	}
}

void Engine::onTermWindow()
{
	if ( m_display != EGL_NO_DISPLAY )
	{
		eglMakeCurrent( m_display
			, EGL_NO_SURFACE
			, EGL_NO_SURFACE
			, EGL_NO_CONTEXT );

		m_starmap.cleanup();

		if ( m_context != EGL_NO_CONTEXT )
		{
			eglDestroyContext( m_display, m_context );
		}
		if ( m_surface != EGL_NO_SURFACE )
		{
			eglDestroySurface( m_display, m_surface );
		}

		eglTerminate( m_display );
	}

	m_animating = false;
	m_display = EGL_NO_DISPLAY;
	m_context = EGL_NO_CONTEXT;
	m_surface = EGL_NO_SURFACE;
}

void Engine::onGainedFocus()
{
	if ( m_accelerometer )
	{
		ASensorEventQueue_enableSensor( m_sensorEventQueue
			, m_accelerometer );
		// L'objectif est de 60 évènements par seconde.
		ASensorEventQueue_setEventRate( m_sensorEventQueue
			, m_accelerometer
			, ( 1000L / 60 ) * 1000 );
	}

	if ( m_gyroscope )
	{
		ASensorEventQueue_enableSensor( m_sensorEventQueue
			, m_gyroscope );
		// L'objectif est de 60 évènements par seconde.
		ASensorEventQueue_setEventRate( m_sensorEventQueue
			, m_gyroscope
			, ( 1000L / 60 ) * 1000 );
	}

	m_animating = true;
}

void Engine::onLostFocus()
{
	if ( m_accelerometer )
	{
		ASensorEventQueue_disableSensor( m_sensorEventQueue
			, m_accelerometer );
	}

	if ( m_gyroscope )
	{
		ASensorEventQueue_disableSensor( m_sensorEventQueue
			, m_gyroscope );
	}

	// Arrêt également de l'animation.
	m_animating = false;
	//doDrawFrame();
}

int Engine::onSingleTouchEvent( AInputEvent * event )
{
	int ret{ 0 };

	if ( m_doubleTapDetector.detect( event ) == utils::ResultType::eDoubleTap )
	{
		m_onScreenDoubleTap( m_doubleTapDetector.result() );
		ret = 1;
	}
	else if ( m_tapDetector.detect( event ) == utils::ResultType::eTap )
	{
		m_onScreenTap( m_tapDetector.result() );
		ret = 1;
	}
	else if ( m_moveDetector.detect( event ) == utils::ResultType::eMove )
	{
		m_onScreenSingleMove( m_moveDetector.result() );
		ret = 1;
	}

	return ret;
}

int Engine::onMultiTouchEvent( AInputEvent * event )
{
	int ret{ 0 };

	if ( m_dblMoveDetector.detect( event ) == utils::ResultType::eMove )
	{
		m_onScreenDoubleMove( m_dblMoveDetector.result()
			, m_dblMoveDetector.distanceOffset() );
		ret = 1;
	}

	return ret;
}

int Engine::doInitialiseDisplay()
{
	// Initialisation d'OpenGL ES et EGL
	EGLDisplay display = eglGetDisplay( EGL_DEFAULT_DISPLAY );
	EGLint major, minor;
	eglInitialize( display, &major, &minor );

	/*
	* Ici, les attributs de la configuration désirée sont spécifiés.
	* Un composant EGLConfig avec au moins 8 bits par couleur
	* compatible avec les fenêtres à l'écran est sélectionné ci-dessous.
	*/
	EGLint configAttribs[]
	{
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT | EGL_OPENGL_ES2_BIT,
		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_ALPHA_SIZE, 8,
		EGL_DEPTH_SIZE, 8,
		EGL_STENCIL_SIZE, 8,
		EGL_NONE
	};
	EGLint numConfigs{ 0 };
	EGLConfig config{ 0 };

	/*
	* Ici, l'application choisit la configuration désirée. Cet
	* exemple illustre un processus de sélection très simplifié dans lequel le premier EGLConfig
	* correspondant aux critères est sélectionné.
	*/
	eglChooseConfig( display
		, configAttribs
		, &config
		, 1
		, &numConfigs );
	int ret = -1;

#if ALLOW_OPENGL_ES_2

	if ( !config )
	{
		configAttribs[1] = EGL_OPENGL_ES2_BIT;
		eglChooseConfig( display
			, configAttribs
			, &config
			, 1
			, &numConfigs );
	}

#endif

	if ( config )
	{
		/*
		* EGL_NATIVE_VISUAL_ID est un attribut d'EGLConfig dont
		* l'acceptation par ANativeWindow_setBuffersGeometry() est garantie.
		* Dès qu'un EGLConfig est choisi, il est possible de reconfigurer en toute sécurité les
		* mémoires tampons ANativeWindow pour les faire correspondre à l'aide d'EGL_NATIVE_VISUAL_ID.
		*/
		EGLint format;
		eglGetConfigAttrib( display, config, EGL_NATIVE_VISUAL_ID, &format );

		ANativeWindow_setBuffersGeometry( m_app->window, 0, 0, format );

		auto surface = eglCreateWindowSurface( display
			, config
			, m_app->window
			, nullptr );

		EGLint contextAttribs[]
		{
			EGL_CONTEXT_CLIENT_VERSION, 3,
			EGL_NONE
		};
		auto context = eglCreateContext( display
			, config
			, nullptr
			, contextAttribs );

#if ALLOW_OPENGL_ES_2

		if ( !context )
		{
			contextAttribs[1] = 2;
			context = eglCreateContext( display
				, config
				, nullptr
				, contextAttribs );
		}

#endif

		if ( context )
		{
			if ( eglMakeCurrent( display
				, surface
				, surface
				, context ) == EGL_FALSE )
			{
				std::cerr << "Unable to eglMakeCurrent" << std::endl;
			}
			else
			{
				EGLint w, h;
				eglQuerySurface( display, surface, EGL_WIDTH, &w );
				eglQuerySurface( display, surface, EGL_HEIGHT, &h );

				m_display = display;
				m_context = context;
				m_surface = surface;
				m_width = std::max( w, h );
				m_height = std::min( h, w );

				// Initialisation de l'état GL.
				glCheckError( glEnable, GL_CULL_FACE );
				glCheckError( glEnable, GL_TEXTURE_2D );
				glCheckError( glFrontFace, GL_CCW );

				auto engine = reinterpret_cast< utils::AndroidApp * >( m_app->userData );
				auto content = engine->getFileBinaryContent( "arial.ttf", true );
				std::string dataPath{ engine->setFileBinaryContent( content, "arial.ttf" ) };

				if ( !dataPath.empty() )
				{
					utils::FontLoader loader{ dataPath };
					m_starmap.initialise( glm::ivec2{ m_width, m_height }
						, engine->getFileBinaryContent( "halo.bmp", true )
						, loader );
					ret = 0;
				}
			}
		}
	}

	return ret;
}

void Engine::doDrawFrame()
{
	if ( m_display )
	{
		m_starmap.beginFrame();
		m_starmap.drawFrame();
		eglSwapBuffers( m_display, m_surface );
		m_starmap.endFrame();
	}
}

std::string Engine::getFileTextContent( std::string const & fileName
	, bool fromResource )const
{
	auto asset = AAssetManager_open( m_app->activity->assetManager
		, fileName.c_str()
		, AASSET_MODE_STREAMING );
	std::string content;

	if ( asset )
	{
		std::array< char, 1024 > buffer;
		int read = 0;

		while ( ( read = AAsset_read( asset
			, buffer.data()
			, buffer.size() ) ) > 0 )
		{
			content += std::string{ buffer.data(), buffer.data() + read };
		}

		AAsset_close( asset );
	}

	return content;
}

std::vector< uint8_t > Engine::getFileBinaryContent( std::string const & fileName
	, bool fromResource )const
{
	auto asset = AAssetManager_open( m_app->activity->assetManager
		, fileName.c_str()
		, AASSET_MODE_STREAMING );
	std::vector< uint8_t > content;

	if ( asset )
	{
		std::array< uint8_t, 1024 > buffer;
		int read = 0;

		while ( ( read = AAsset_read( asset
			, buffer.data()
			, buffer.size() ) ) > 0 )
		{
			content.insert( content.end()
				, buffer.data()
				, buffer.data() + read );
		}

		AAsset_close( asset );
	}

	return content;
}

std::string Engine::setFileTextContent( std::string const & content
	, std::string const & fileName )const
{
	std::string dataPath = std::string{ m_app->activity->internalDataPath }
	+"/" + fileName;
	FILE * fp = fopen( dataPath.c_str(), "w" );
	bool result{ false };

	if ( fp )
	{
		result = fwrite( content.data(), 1, content.size(), fp ) == content.size();
		fclose( fp );
	}

	if ( !result )
	{
		dataPath.clear();
	}

	return dataPath;
}

std::string Engine::setFileBinaryContent( std::vector< uint8_t > const & content
	, std::string const & fileName )const
{
	std::string dataPath = std::string{ m_app->activity->internalDataPath }
	+"/" + fileName;
	FILE * fp = fopen( dataPath.c_str(), "wb" );
	bool result{ false };

	if ( fp )
	{
		result = fwrite( content.data(), 1, content.size(), fp ) == content.size();
		fclose( fp );
	}

	if ( !result )
	{
		dataPath.clear();
	}

	return dataPath;
}
