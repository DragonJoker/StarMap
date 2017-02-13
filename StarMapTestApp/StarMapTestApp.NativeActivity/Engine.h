#pragma once

#include <StarMapLib/ScreenEvents.h>
#include <StarMapLib/StarMap.h>

#include <AndroidUtils/GestureDetector.h>

class Engine
{
public:
	Engine( android_app * state );
	~Engine();
	/**
	*\brief
	*	Traitement des évènements utilisateur.
	*/
	void handleUserEvents();
	/**
	*\brief
	*	Destruction du contexte EGL actuellement associé à l'affichage.
	*/
	void cleanup();
	/**
	*\brief
	*	Met à jour les données de frame et dessine une frame.
	*/
	void update();
	/**
	*\brief
	*	Charge le contenu d'un fichier texte dans un std::string.
	*\param[in] fileName
	*	Le nom du fichier texte.
	*\param[in] fromResource
	*	Précise si le fichier se trouve dans les ressources, ou sur le
	*	système de fichier.
	*\return
	*	Le contenu du fichier, vide si non trouvé
	*/
	std::string getFileTextContent( std::string const & fileName
		, bool fromResource )const;
	/**
	*\brief
	*	Charge le contenu d'un fichier binaire dans un std::vector< uint8_t >.
	*\param[in] fileName
	*	Le nom du fichier binaire.
	*\param[in] fromResource
	*	Précise si le fichier se trouve dans les ressources, ou sur le
	*	système de fichier.
	*\return
	*	Le contenu du fichier, vide si non trouvé
	*/
	std::vector< uint8_t > getFileBinaryContent( std::string const & fileName
		, bool fromResource )const;
	/**
	*\brief
	*	Ecrit un std::string contenu d'un fichier texte des assets dans un std::string.
	*\param[content]
	*	Le contenu du fichier.
	*\param[in] fileName
	*	Le nom du fichier texte.
	*\return
	*	Le chemin d'accès au fichier en cas de succès, vide sinon.
	*/
	std::string setFileTextContent( std::string const & content
		, std::string const & fileName )const;
	/**
	*\brief
	*	Charge le contenu d'un fichier binaire des assets dans un std::vector< uint8_t >.
	*\param[content]
	*	Le contenu du fichier.
	*\param[in] fileName
	*	Le nom du fichier binaire.
	*\return
	*	Le chemin d'accès au fichier en cas de succès, vide sinon.
	*/
	std::string setFileBinaryContent( std::vector< uint8_t > const & content
		, std::string const & fileName )const;
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

	inline bool isAnimating()const
	{
		return m_animating;
	}

private:
	/**
	*\brief
	*	Enregistre l'état courant.
	*/
	void onSaveState();
	/**
	*\brief
	*	Initialisation de la fenêtre.
	*/
	void onInitWindow();
	/**
	*\brief
	*	Met à jour la configuration de la fenêtre.
	*/
	void onUpdateConfig();
	/**
	*\brief
	*	Destruction du contexte EGL actuellement associé à l'affichage.
	*/
	void onTermWindow();
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
	/**
	*\brief
	*	Gestion d'un évènement de type toucher unique.
	*/
	int onSingleTouchEvent( AInputEvent * event );
	/**
	*\brief
	*	Gestion d'un évènement de type toucher multiple.
	*/
	int onMultiTouchEvent( AInputEvent * event );

private:
	/**
	*\brief
	*	Initialisation d'un contexte EGL pour l'affichage en cours.
	*/
	int doInitialiseDisplay();
	/**
	*\brief
	*	Uniquement l'image actuelle dans l'affichage.
	*/
	void doDrawFrame();
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
	android_app * m_app{ nullptr };

	ASensor const * m_accelerometer{ nullptr };
	ASensor const * m_gyroscope{ nullptr };
	ASensorEventQueue * m_sensorEventQueue{ nullptr };

	std::streambuf * m_cout;
	std::streambuf * m_cerr;
	std::streambuf * m_clog;

	bool m_animating{ false };
	EGLDisplay m_display{ EGL_NO_DISPLAY };
	EGLSurface m_surface{ EGL_NO_SURFACE };
	EGLContext m_context{ EGL_NO_CONTEXT };
	int32_t m_width{ 0 };
	int32_t m_height{ 0 };

	utils::TapDetector m_tapDetector;
	utils::DoubleTapDetector m_doubleTapDetector;
	utils::MoveDetector m_moveDetector;
	utils::DoubleMoveDetector m_dblMoveDetector;

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
