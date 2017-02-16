/**
*\file
*	AndroidApp.h
*\author
*	Sylvain Doremus
*/
#ifndef ___DesktopUtils_AndroidApp_HPP___
#define ___DesktopUtils_AndroidApp_HPP___
#pragma once

#include "AndroidWindow.h"
#include "GestureDetector.h"

#include <android/native_activity.h>

namespace utils
{
	/**
	*\brief
	*	Fonction de création de la fenêtre.
	*\param[in] app
	*	L'application Android.
	*\param[in] window
	*	La fenêtre Android native.
	*\param[in] state
	*	L'état sauvegardé dans l'application (au cas où la fenêtre a été
	*	détruite à cause d'une mise en background de l'application).
	*\return
	*	La fenêtre créée.
	*/
	using CreateWindowFunc = std::function< AndroidWindowPtr
		( AndroidApp const & app
		, ANativeWindow * window
		, render::ByteArray const & state ) >;
	/**
	*\brief
	*	Application Android de base.
	*\remarks
	*	Crée la fenêtre à partir de la fonction de création donnée.\n
	*	Faire de cette manière permet de ne pas avoir à créer une classe
	*	dérivée de AndroidApp dans les applications l'utilisant.
	*/
	class AndroidApp
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] activity
		*	L'activité native.
		*\param[in] config
		*	La configuration actuelle.
		*\param[in] looper
		*	La boucle d'évènements.
		*\param[in] sensorQueueIndex
		*	L'indice de la file de capteurs à écouter.
		*\param[in] createWindow
		*	Fonction de création de la fenêtre.
		*/
		AndroidApp( ANativeActivity * activity
			, AConfiguration * config
			, ALooper * looper
			, int sensorQueueIndex
			, CreateWindowFunc createWindow );
		/**
		*\brief
		*	Destructeur.
		*/
		~AndroidApp();
		/**
		*\brief
		*	Traitement des évènements utilisateur.
		*/
		void handleUserEvents();
		/**
		*\brief
		*	Traitement d'un évènement d'entrée.
		*\param[in] event
		*	L'évènement à traiter.
		*\return
		*	1 si l'évènement a été traité, 0 sinon.
		*/
		int32_t handleInput( AInputEvent * event );
		/**
		*\brief
		*	Ferme la fenêtre.
		*/
		void cleanup();
		/**
		*\brief
		*	Met à jour les données et dessine une frame.
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
		*	Le contenu du fichier, vide si non trouvé.
		*/
		std::string getFileTextContent( std::string const & fileName
			, bool fromResource )const;
		/**
		*\brief
		*	Charge le contenu d'un fichier binaire dans un render::ByteArray.
		*\param[in] fileName
		*	Le nom du fichier binaire.
		*\param[in] fromResource
		*	Précise si le fichier se trouve dans les ressources, ou sur le
		*	système de fichier.
		*\return
		*	Le contenu du fichier, vide si non trouvé.
		*/
		render::ByteArray getFileBinaryContent( std::string const & fileName
			, bool fromResource )const;
		/**
		*\brief
		*	Ecrit un texte dans un fichier du système de fichiers.
		*\param[in] content
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
		*	Ecrit un tableau d'octets dans un fichier du système de fichiers.
		*\param[in] content
		*	Le contenu du fichier.
		*\param[in] fileName
		*	Le nom du fichier binaire.
		*\return
		*	Le chemin d'accès au fichier en cas de succès, vide sinon.
		*/
		std::string setFileBinaryContent( render::ByteArray const & content
			, std::string const & fileName )const;
		/**
		*\return
		*	Sauvegarde les données de l'application.
		*\param[out] state
		*	Reçoit l'état de l'application.
		*\param[out] stateSize
		*	Reçoit la taille de l'état sauvegardé.
		*/
		void onSave( void *& state, size_t & stateSize );
		/**
		*\brief
		*	Initialisation de la fenêtre.
		*\param[in] window
		*	La fenêtre Android native.
		*\param[in] state
		*	L'état de l'application au moment de sa sauvegarde.
		*\param[in] stateSize
		*	La taille de l'état sauvegardé.
		*/
		void onInitWindow( ANativeWindow * window
			, void * state
			, size_t stateSize );
		/**
		*\brief
		*	Met à jour la configuration de la fenêtre.
		*/
		void onUpdateConfig();
		/**
		*\brief
		*	Destruction de la fenêtre.
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
		/**
		*\return
		*	\p true si l'application est active.
		*/
		inline bool animating()const noexcept
		{
			return m_animating;
		}

	private:
		//! L'activité native.
		ANativeActivity * m_activity;
		//! Fonction de création de la fenêtre.
		CreateWindowFunc createWindow;
		//! L'accéléromètre
		ASensor const * m_accelerometer{ nullptr };
		//! Le gyroscope.
		ASensor const * m_gyroscope{ nullptr };
		//! La file d'évènements de capteurs.
		ASensorEventQueue * m_sensorEventQueue{ nullptr };
		//! Le streambuf par log pour cout.
		std::streambuf * m_cout;
		//! Le streambuf par log pour cerr.
		std::streambuf * m_cerr;
		//! Le streambuf par log pour clog.
		std::streambuf * m_clog;
		//! Le détecteur de tap sur l'écran.
		utils::TapDetector m_tapDetector;
		//! Le détecteur de double tap sur l'écran.
		utils::DoubleTapDetector m_doubleTapDetector;
		//! Le détecteur de déplacement d'un curseur sur l'écran.
		utils::MoveDetector m_moveDetector;
		//! Le détecteur de déplacement de deux curseurs sur l'écran.
		utils::DoubleMoveDetector m_dblMoveDetector;
		//! Dit que l'application est active.
		bool m_animating{ false };
		//! La fenêtre de l'application
		AndroidWindowPtr m_window;
		//! L'état sauvegardé de la fenêtre.
		render::ByteArray m_savedState;
	};
}

#endif
