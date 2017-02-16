#include "AndroidWindow.h"

#include "AndroidApp.h"
#include "FontLoader.h"
#include "ObjLoader.h"
#include "Log.h"

#include <array>
#include <string>

#include <android/configuration.h>
#include <android/window.h>

#include <RenderLib/Billboard.h>
#include <RenderLib/CameraState.h>
#include <RenderLib/PolyLine.h>
#include <RenderLib/RenderWindow.h>

#include <GlLib/glm/gtc/matrix_transform.hpp>

#define ALLOW_OPENGL_ES_2 1

#ifndef EGL_OPENGL_ES3_BIT
#	define EGL_OPENGL_ES3_BIT 0x00000040
#endif

namespace utils
{
	AndroidWindow::AndroidWindow( AndroidApp const & parent
		, ANativeWindow * window
		, render::ByteArray const & state )
		: m_window{ window }
		, m_parent{ parent }
		, m_state{ state }
	{
	}

	AndroidWindow::~AndroidWindow()
	{
	}

	void AndroidWindow::initialise()
	{
		EGLConfig config{ 0 };
		EGLDisplay display = doInitialiseEGL( config );

		if ( config && display )
		{
			EGLSurface surface;
			auto context = doCreateContext( display, config, surface );

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
					m_size =
					{
						std::max( w, h ),
						std::min( h, w )
					};

					// Initialisation de l'état GL.
					if ( !gl::OpenGL::isInitialised() )
					{
						gl::OpenGL::initialise();
					}

					glCheckError( glEnable, GL_TEXTURE_2D );
					glCheckError( glFrontFace, GL_CCW );
					
					onCreate();

					if ( !m_state.empty() )
					{
						onRestore( m_state );
						m_state.clear();
					}

					draw();
				}
			}
		}
	}

	void AndroidWindow::cleanup()
	{
		if ( m_display != EGL_NO_DISPLAY )
		{
			eglMakeCurrent( m_display
				, EGL_NO_SURFACE
				, EGL_NO_SURFACE
				, EGL_NO_CONTEXT );
			onDestroy();

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

		m_display = EGL_NO_DISPLAY;
		m_context = EGL_NO_CONTEXT;
		m_surface = EGL_NO_SURFACE;
	}

	void AndroidWindow::draw()
	{
		if ( m_display )
		{
			onDraw();
		}
	}

	void AndroidWindow::updateConfig()
	{
		if ( m_size.x != ANativeWindow_getWidth( m_window )
				|| m_size.y != ANativeWindow_getHeight( m_window ) )
		{
			cleanup();
			initialise();
		}
	}

	EGLDisplay AndroidWindow::doInitialiseEGL( EGLConfig & config )
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

		return display;
	}

	EGLContext AndroidWindow::doCreateContext( EGLDisplay display
		, EGLConfig config
		, EGLSurface & surface )
	{
		/*
		* EGL_NATIVE_VISUAL_ID est un attribut d'EGLConfig dont
		* l'acceptation par ANativeWindow_setBuffersGeometry() est garantie.
		* Dès qu'un EGLConfig est choisi, il est possible de reconfigurer en toute sécurité les
		* mémoires tampons ANativeWindow pour les faire correspondre à l'aide d'EGL_NATIVE_VISUAL_ID.
		*/
		EGLint format;
		eglGetConfigAttrib( display, config, EGL_NATIVE_VISUAL_ID, &format );

		ANativeWindow_setBuffersGeometry( m_window, 0, 0, format );

		surface = eglCreateWindowSurface( display
			, config
			, m_window
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

		return context;
	}

	render::FontLoaderPtr AndroidWindow::doCreateFontLoader( std::string const & path )
	{
		render::FontLoaderPtr result;
		auto content = m_parent.getFileBinaryContent( "arial.ttf", true );
		std::string dataPath = m_parent.setFileBinaryContent( content
			, "/arial.ttf" );

		if ( !dataPath.empty() )
		{
			result = std::make_unique< utils::FontLoader >( dataPath );
		}

		return result;
	}

	render::FontTexturePtr AndroidWindow::doCreateFontTexture( std::string const & path
		, uint32_t height )
	{
		render::FontTexturePtr result;
		auto content = m_parent.getFileBinaryContent( "arial.ttf", true );
		std::string dataPath = m_parent.setFileBinaryContent( content
			, "/arial.ttf" );

		if ( !dataPath.empty() )
		{
			render::FontPtr font = std::make_unique< render::Font >( "Arial", 32u );
			utils::FontLoader loader{ dataPath };
			render::loadFont( loader, *font );
			result = std::make_unique< render::FontTexture >
				( std::move( font ) );
		}

		return result;
	}
}
