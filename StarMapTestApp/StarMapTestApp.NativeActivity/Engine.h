#pragma once

#include <StarMapLib/ScreenEvents.h>
#include <StarMapLib/StarMap.h>

#include <AndroidUtils/AndroidWindow.h>

class Window
	: public utils::AndroidWindow
{
public:
	Window( utils::AndroidApp const & parent
		, ANativeWindow * window
		, render::ByteArray const & state );
	~Window();

private:
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
	*	Met à jour les données de frame et dessine une frame.
	*/
	void onDraw()override;
	/**
	*\return
	*	Sauvegarde les données de l'application.
	*/
	void onSave( render::ByteArray & state )override;
	/**
	*\return
	*	Restaure les données de l'application.
	*/
	void onRestore( render::ByteArray const & state )override;
	/**
	*\brief
	*	Gestion d'un évènement de type tap.
	*/
	void onSingleTap( gl::Position2D const & position )override;
	/**
	*\brief
	*	Gestion d'un évènement de type double tap.
	*/
	void onDoubleTap( gl::Position2D const & position )override;
	/**
	*\brief
	*	Gestion d'un évènement de type déplacement avec un pointeur.
	*/
	void onSingleMove( gl::Position2D const & position )override;
	/**
	*\brief
	*	Gestion d'un évènement de type déplacement avec deux pointeur.
	*/
	void onDoubleMove( gl::Position2D const & posDiff, int distDiff )override;

private:
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

class Engine
{
public:
	/**
	*\brief
	*	Traitement de l'évènement d'entrée suivant.
	*/
	static int32_t handleInput( android_app * app, AInputEvent * event );
	/**
	*\brief
	*	Traitement de la commande principale suivante.
	*/
	static void handleCommand( android_app * app, int32_t cmd );
};
