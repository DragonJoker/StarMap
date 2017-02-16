/**
*\file
*	MsWindow.h
*\author
*	Sylvain Doremus
*/
#ifndef ___DesktopUtils_MsWindow_HPP___
#define ___DesktopUtils_MsWindow_HPP___
#pragma once

#include "DesktopUtilsPrerequisites.h"

#include <Windows.h>

namespace utils
{
	class MsWindow
	{
	public:
		MsWindow();
		/**
		*\brief
		*	Crée la fenêtre.
		*/
		void create( HINSTANCE instance
			, std::string const & className
			, int iconResourceID
			, int smallIconResourceID
			, std::string const & title );
		/**
		*\return
		*	Les dimensions de la fenêtre.
		*/
		inline gl::Size2D size()const
		{
			return m_size;
		}

	private:
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
		*	Mise à jour du contenu.
		*/
		virtual void onDraw() = 0;
		/**
		*\brief
		*	Redimensionnement de la fenêtre.
		*/
		virtual void onResize( gl::Size2D const & event ) = 0;
		/**
		*\brief
		*	D�placement de la souris.
		*/
		virtual void onMouseMove( utils::MouseEvent const & event ) = 0;
		/**
		*\brief
		*	Bouton gauche de la souris cliqué.
		*/
		virtual void onLButtonDown( utils::MouseEvent const & event ) = 0;
		/**
		*\brief
		*	Bouton gauche de la souris cliqué.
		*/
		virtual void onLButtonClick( utils::MouseEvent const & event ) = 0;
		/**
		*\brief
		*	Bouton gauche de la souris double cliqué.
		*/
		virtual void onLButtonDblClick( utils::MouseEvent const & event ) = 0;
		/**
		*\brief
		*	Un évènement de molette.
		*/
		virtual void onMouseWheel( utils::MouseEvent const & event ) = 0;

	protected:
		/**
		*\brief
		*	Echange les tampons du double buffering.
		*/
		void doSwapBuffers();

	private:
		void doCreate();
		void doDestroy();
		void doResize( gl::Size2D const & size );
		bool doPrepareDC( HDC hdc );
		HGLRC doCreateContext( HDC hdc );
		void doRegisterClass( HINSTANCE hInstance
			, std::string const & className
			, int iconResourceID
			, int smallIconResourceID );
		LRESULT processMessage( UINT message
			, WPARAM wParam
			, LPARAM lParam );
		static LRESULT CALLBACK WndProc( HWND hWnd
			, UINT message
			, WPARAM wParam
			, LPARAM lParam );
		static void doRegisterInstance( MsWindow * window );
		static void doUnregisterInstance( MsWindow * window );
		static MsWindow * doGetInstance( HWND hWnd );

	private:
		HWND m_hwnd{ nullptr };
		HDC m_hdc{ NULL };
		HGLRC m_context{ NULL };
		gl::Size2D m_size;
		int m_timer{ -1 };
		static std::map< HWND, MsWindow * > sm_instances;
	};
}

#endif
