/**
*\file
*	AndroidWindow.h
*\author
*	Sylvain Doremus
*/
#ifndef ___DesktopUtils_AndroidWindow_HPP___
#define ___DesktopUtils_AndroidWindow_HPP___
#pragma once

#include "AndroidUtilsPrerequisites.h"

#include <RenderLib/Debug.h>

#include <android/native_activity.h>

namespace utils
{
	/**
	*\brief
	*	Classe de base d'une fenêtre Android.
	*\remarks
	*	Seule classe à implémenter dans les applications.
	*/
	class AndroidWindow
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] parent
		*	L'application Android.
		*\param[in] window
		*	La fenêtre Android native.
		*\param[in] state
		*	L'état sauvegardé dans l'application (au cas où la fenêtre a été
		*	détruite à cause d'une mise en background de l'application).
		*/
		AndroidWindow( AndroidApp const & parent
			, ANativeWindow * window
			, render::ByteArray const & state );
		/**
		*\brief
		*	Destructeur.
		*/
		virtual ~AndroidWindow();
		/**
		*\brief
		*	Initialise les données de la fenêtre.
		*/
		void initialise();
		/**
		*\brief
		*	Nettoie les données de la fenêtre.
		*/
		void cleanup();
		/**
		*\brief
		*	Met à jour les données de frame et dessine une frame.
		*/
		void draw();
		/**
		*\brief
		*	Met à jour la configuration de la fenêtre.
		*/
		void updateConfig();
		/**
		*\brief
		*	Création de la fenêtre.
		*/
		virtual void onCreate() = 0;
		/**
		*\brief
		*	Destruction de la fenêtre.
		*/
		virtual void onDestroy() = 0;
		/**
		*\brief
		*	Met à jour les données de frame et dessine une frame.
		*/
		virtual void onDraw() = 0;
		/**
		*\return
		*	Sauvegarde les données de la fenêtre.
		*\param[out] state
		*	Reçoit l'état de la fenêtre.
		*/
		virtual void onSave( render::ByteArray & state ) = 0;
		/**
		*\return
		*	Restaure les données de la fenêtre.
		*\param[in] state
		*	Contient l'état de la fenêtre au moment de la sauvegarde.
		*/
		virtual void onRestore( render::ByteArray const & state ) = 0;
		/**
		*\brief
		*	Gestion d'un évènement de type tap.
		*\param[in] position
		*	La position du curseur.
		*/
		virtual void onSingleTap( gl::Position2D const & position ) = 0;
		/**
		*\brief
		*	Gestion d'un évènement de type double tap.
		*\param[in] position
		*	La position du curseur.
		*/
		virtual void onDoubleTap( gl::Position2D const & position ) = 0;
		/**
		*\brief
		*	Gestion d'un évènement de type déplacement avec un curseur.
		*\param[in] diff
		*	La différence de position du curseur..
		*/
		virtual void onSingleMove( gl::Offset2D const & diff ) = 0;
		/**
		*\brief
		*	Gestion d'un évènement de type déplacement avec deux curseurs.
		*\param[in] posDiff
		*	La différence de position des curseurs.
		*\param[in] distDiff
		*	La différence de distance entre les curseur.
		*/
		virtual void onDoubleMove( gl::Position2D const & posDiff, int distDiff ) = 0;
		/**
		*\return
		*	Les dimensions de la fenêtre.
		*/
		inline gl::Size2D const & dimensions()const noexcept
		{
			return m_size;
		}

	protected:
		/**
		*\brief
		*	Crée un loader de police.
		*\param[in] path
		*	Le chemin d'accès au fichier de la police.
		*\return
		*	Le loader de police.
		*/
		render::FontLoaderPtr doCreateFontLoader( std::string const & path );
		/**
		*\brief
		*	Crée texture de police.
		*\param[in] path
		*	Le chemin d'accès au fichier de la police.
		*\param[in] height
		*	La hauteur voulue pour les caractères.
		*\return
		*	La texture de police.
		*/
		render::FontTexturePtr doCreateFontTexture( std::string const & path
			, uint32_t height );

	private:
		/**
		*\brief
		*	Initialise EGL.
		*\param[out] config
		*	Reçoit la configuration sélectionnée.
		*\return
		*	L'écran.
		*/
		EGLDisplay doInitialiseEGL( EGLConfig & config );
		/**
		*\brief
		*	Crée le contexte OpenGL.
		*\param[in] display
		*	L'écran.
		*\param[in] config
		*	La configuration sélectionnée.
		*\param[out] surface
		*	Reçoit la surface EGL.
		*/
		EGLContext doCreateContext( EGLDisplay display
			, EGLConfig config
			, EGLSurface & surface );

	protected:
		//! La fenêtre Android native.
		ANativeWindow * m_window;
		//! L'application Android.
		AndroidApp const & m_parent;
		//! L'état sauvegardé dans l'application.
		render::ByteArray m_state;
		//! L'écran EGL.
		EGLDisplay m_display{ EGL_NO_DISPLAY };
		//! La surface EGL de rendu.
		EGLSurface m_surface{ EGL_NO_SURFACE };
		//! Le contexte de rendu OpenGL.
		EGLContext m_context{ EGL_NO_CONTEXT };
		//! Les dimensions de la fenêtre.
		gl::Size2D m_size{ 0 };
	};
}

#endif
