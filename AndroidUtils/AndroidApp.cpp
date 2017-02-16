#include "AndroidApp.h"

#include "FontLoader.h"
#include "ObjLoader.h"
#include "Log.h"

#include <array>
#include <string>

#include <android/configuration.h>
#include <android/window.h>

#include <RenderLib/Billboard.h>
#include <RenderLib/PolyLine.h>

#include <GlLib/glm/gtc/matrix_transform.hpp>

#define ALLOW_OPENGL_ES_2 1

#ifndef EGL_OPENGL_ES3_BIT
#	define EGL_OPENGL_ES3_BIT 0x00000040
#endif

namespace utils
{
	AndroidApp::AndroidApp( ANativeActivity * activity
		, AConfiguration * config
		, ALooper * looper
		, int sensorIndex
		, CreateWindowFunc createWindow )
		: m_activity{ activity }
		, createWindow{ createWindow }
		, m_tapDetector{ config }
		, m_doubleTapDetector{ config }
		, m_moveDetector{ config }
		, m_dblMoveDetector{ config }
	{
		std::string const appName = "RenderLibTestApp";
		m_cout = new render::LogStreambuf< utils::InfoLogStreambufTraits >( appName
			, std::cout );
		m_cerr = new render::LogStreambuf< utils::ErrorLogStreambufTraits >( appName
			, std::cerr );
		m_clog = new render::LogStreambuf< utils::DebugLogStreambufTraits >( appName
			, std::clog );

		// Pr�paration de la surveillance de l'acc�l�rom�tre
		auto sensorManager = ASensorManager_getInstance();
		m_accelerometer = ASensorManager_getDefaultSensor( sensorManager
			, ASENSOR_TYPE_ACCELEROMETER );
		m_gyroscope = ASensorManager_getDefaultSensor( sensorManager
			, ASENSOR_TYPE_GYROSCOPE );
		m_sensorEventQueue = ASensorManager_createEventQueue( sensorManager
			, looper
			, sensorIndex
			, nullptr
			, nullptr );

		ANativeActivity_setWindowFlags( m_activity
			, AWINDOW_FLAG_FULLSCREEN | AWINDOW_FLAG_KEEP_SCREEN_ON
			, 0 );
	}

	AndroidApp::~AndroidApp()
	{
		delete m_cout;
		delete m_cerr;
		delete m_clog;
	}

	void AndroidApp::handleUserEvents()
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
				}
				break;

			case ASENSOR_TYPE_GYROSCOPE:
				if ( m_gyroscope )
				{
				}
				break;
			}
		}
	}

	int32_t AndroidApp::handleInput( AInputEvent * event )
	{
		int32_t ret = 0;

		switch ( AInputEvent_getType( event ) )
		{
		case AINPUT_EVENT_TYPE_MOTION:
			if ( AMotionEvent_getPointerCount( event ) == 1 )
			{
				ret = onSingleTouchEvent( event );
			}
			else
			{
				ret = onMultiTouchEvent( event );
			}
			break;
		}

		return ret;
	}

	void AndroidApp::cleanup()
	{
		onTermWindow();
	}

	void AndroidApp::update()
	{
		if ( m_window && m_animating )
		{
			m_window->draw();
		}
	}

	std::string AndroidApp::getFileTextContent( std::string const & fileName
		, bool fromResource )const
	{
		auto asset = AAssetManager_open( m_activity->assetManager
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

	render::ByteArray AndroidApp::getFileBinaryContent( std::string const & fileName
		, bool fromResource )const
	{
		auto asset = AAssetManager_open( m_activity->assetManager
			, fileName.c_str()
			, AASSET_MODE_STREAMING );
		render::ByteArray content;

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

	std::string AndroidApp::setFileTextContent( std::string const & content
		, std::string const & fileName )const
	{
		std::string dataPath = std::string{ m_activity->internalDataPath }
			+ "/" + fileName;
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

	std::string AndroidApp::setFileBinaryContent( render::ByteArray const & content
		, std::string const & fileName )const
	{
		std::string dataPath = std::string{ m_activity->internalDataPath }
			+ "/" + fileName;
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

	void AndroidApp::onSave( void *& state, size_t & stateSize )
	{
		if ( !m_savedState.empty() )
		{
			stateSize = m_savedState.size();
			state = malloc(stateSize);
			std::memcpy( state, m_savedState.data(), stateSize );
		}
	}

	void AndroidApp::onInitWindow( ANativeWindow * window
		, void * state
		, size_t stateSize )
	{
		if ( stateSize && state )
		{
			m_savedState.resize( stateSize );
			std::memcpy( m_savedState.data(), state, stateSize );
		}

		m_window = createWindow( *this
			, window
			, m_savedState );
		m_window->initialise();
		ANativeActivity_setWindowFlags( m_activity
			, AWINDOW_FLAG_FULLSCREEN | AWINDOW_FLAG_KEEP_SCREEN_ON
			, 0 );
		m_animating = true;
	}

	void AndroidApp::onUpdateConfig()
	{
		if ( m_window )
		{
			m_window->updateConfig();
		}
	}

	void AndroidApp::onTermWindow()
	{
		if ( m_window )
		{
			m_window->onSave( m_savedState );
		}

		m_animating = false;
		m_window.reset();
	}

	void AndroidApp::onGainedFocus()
	{
		if ( m_accelerometer )
		{
			ASensorEventQueue_enableSensor( m_sensorEventQueue
				, m_accelerometer );
			// L'objectif est de 60 �v�nements par seconde.
			ASensorEventQueue_setEventRate( m_sensorEventQueue
				, m_accelerometer
				, ( 1000L / 60 ) * 1000 );
		}

		if ( m_gyroscope )
		{
			ASensorEventQueue_enableSensor( m_sensorEventQueue
				, m_gyroscope );
			// L'objectif est de 60 �v�nements par seconde.
			ASensorEventQueue_setEventRate( m_sensorEventQueue
				, m_gyroscope
				, ( 1000L / 60 ) * 1000 );
		}

		m_animating = true;
	}

	void AndroidApp::onLostFocus()
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

		// Arr�t �galement de l'animation.
		m_animating = false;
	}

	int AndroidApp::onSingleTouchEvent( AInputEvent * event )
	{
		int ret{ 0 };

		if ( m_doubleTapDetector.detect( event ) == utils::ResultType::eDoubleTap )
		{
			if ( m_window )
			{
				m_window->onDoubleTap( m_doubleTapDetector.result() );
			}
			ret = 1;
		}
		else if ( m_tapDetector.detect( event ) == utils::ResultType::eTap )
		{
			if ( m_window )
			{
				m_window->onSingleTap( m_tapDetector.result() );
			}

			ret = 1;
		}
		else if ( m_moveDetector.detect( event ) == utils::ResultType::eMove )
		{
			if ( m_window )
			{
				m_window->onSingleMove( m_moveDetector.result() );
			}
			ret = 1;
		}

		return ret;
	}

	int AndroidApp::onMultiTouchEvent( AInputEvent * event )
	{
		int ret{ 0 };

		if ( m_dblMoveDetector.detect( event ) == utils::ResultType::eMove )
		{
			m_window->onDoubleMove( m_dblMoveDetector.result(), m_dblMoveDetector.distanceOffset() );
			ret = 1;
		}

		return ret;
	}
}
