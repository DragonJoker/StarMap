#pragma once

#include <RenderLib/CameraState.h>

#include <StarMapLib/ScreenEvents.h>
#include <StarMapLib/StarMap.h>

#include <DesktopUtils/MsWindow.h>

#include <Windows.h>
#include <GL/glew.h>

class Engine
	: public utils::MsWindow
{
public:
	Engine();
	~Engine();

public:
	/**
	*\brief
	*	Mise à jour du contenu.
	*/
	void onUpdate( HDC hdc )override;
	/**
	*\brief
	*	Dessin de la fenêtre.
	*/
	void onPaint( HDC hdc )override;
	/**
	*\brief
	*	Création de la fenêtre.
	*/
	void onCreate()override;
	/**
	*\brief
	*	Destruction de la fenêtre.
	*/
	void onDestroy()override;
	/**
	*\brief
	*	Initialisation de la fenêtre.
	*/
	void onResize( glm::ivec2 const & event )override;
	/**
	*\brief
	*	Déplacement de la souris.
	*/
	void onMouseMove( utils::MouseEvent const & event )override;
	/**
	*\brief
	*	Bouton gauche de la souris cliqué.
	*/
	void onLButtonDown( utils::MouseEvent const & event )override;
	/**
	*\brief
	*	Bouton gauche de la souris cliqué.
	*/
	void onLButtonClick( utils::MouseEvent const & event )override;
	/**
	*\brief
	*	Bouton gauche de la souris double cliqué.
	*/
	void onLButtonDblClick( utils::MouseEvent const & event )override;
	/**
	*\brief
	*	Un évènement de molette.
	*/
	void onMouseWheel( utils::MouseEvent const & event )override;
	/**
	*\brief
	*	Démarre la surveillance de l'accéléromètre.
	*/
	void onGainedFocus();
	/**
	*\brief
	*	Stoppe la surveillance de l'accéléromètre.
	*/
	void onLostFocus();

private:
	/**
	*\brief
	*	Initialisation d'un contexte EGL pour l'affichage en cours.
	*/
	int doInitialiseDisplay();
	/**
	*\brief
	*	Initialisation des éléments 3D affichés.
	*/
	void doInitialise3DElements();
	/**
	*\brief
	*	Destruction des éléments 3D affichés.
	*/
	void doCleanup3DElements();
	/**
	*\brief
	*	Initialise la texture de police.
	*/
	void doInitialiseFontTexture();

private:
	std::streambuf * m_cout;
	std::streambuf * m_cerr;
	std::streambuf * m_clog;

	bool m_animating{ false };
	HDC m_hdc{ NULL };
	HGLRC m_context{ NULL };
	glm::ivec2 m_size;

	glm::ivec2 m_mouse;
	render::CameraState m_handler;

	//! Le signal émis lorsque l'on "clique" sur l'écran.
	starmap::OnScreenTap m_onScreenTap;
	//! Le signal émis lorsque l'on "double clique" sur l'écran.
	starmap::OnScreenDoubleTap m_onScreenDoubleTap;
	//! Le signal émis lorsque l'on effectue un déplacement avec un doigt sur l'écran.
	starmap::OnScreenSingleMove m_onScreenSingleMove;
	//! Le signal émis lorsque l'on effectue un déplacement avec deux doigts sur l'écran.
	starmap::OnScreenDoubleMove m_onScreenDoubleMove;
	//! Les évènements écran.
	starmap::ScreenEvents m_events;
	//! La carte des étoiles.
	starmap::StarMap m_starmap;
};
