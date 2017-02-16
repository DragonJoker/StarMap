/**
*\file
*	AndroidUtilsPrerequisites.h
*\author
*	Sylvain Doremus
*/
#ifndef ___AndroidUtils_GestureDetector_HPP___
#define ___AndroidUtils_GestureDetector_HPP___
#pragma once

#include "AndroidUtilsPrerequisites.h"

#include <android/sensor.h>
#include <android/log.h>
#include <android/configuration.h>
#include <android/native_activity.h>
#include <android/native_window_jni.h>

namespace utils
{
	/**
	*\brief
	*	Type de résultat possible pour les évènements écran.
	*/
	enum class ResultType
	{
		//! Aucun évènement écran correspondant.
		eNone,
		//! Evènement écran de type appui.
		eDown,
		//! Evènement écran de type déplacement.
		eMove,
		//! Evènement écran de type tap.
		eTap,
		//! Evènement écran de type double tap.
		eDoubleTap,
	};
	/**
	*\brief
	*	Classe de base pour les évènements écran.
	*/
	class GestureDetector
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] config
		*	La configuration de l'écran.
		*/
		GestureDetector( AConfiguration * config )noexcept;
		/**
		*\brief
		*	Effectue la détection de l'évènement souhaité, à partir de
		*	l'évènement donné.
		*\param[in] event
		*	Les informations de l'évènement enregistré.
		*\return
		*	L'évènement détecté.
		*/
		virtual ResultType detect( const AInputEvent * event ) = 0;
		/**
		*\return
		*	Le résultat de l'évènement détecté, dépend du type d'évènement
		*	souhaité.
		*/
		inline gl::Vector2D const & result()const noexcept
		{
			return m_result;
		}

	protected:
		//! Le facteur de DPI.
		float m_dpFactor{ 1.0f };
		//! Le résultat de l'évènement, au cas où il a été détecté.
		gl::Vector2D m_result;
	};
	/**
	*\brief
	*	Détecte les évènements de type "clic" sur l'écran.
	*\remarks
	*	result() renvoie la position du "clic".
	*/
	class TapDetector
		: public GestureDetector
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] config
		*	La configuration de l'écran.
		*/
		TapDetector( AConfiguration * config )noexcept;
		/**
		*\brief
		*	Effectue la détection du "clic", à partir de l'évènement donné.
		*\param[in] event
		*	Les informations de l'évènement enregistré.
		*\return
		*	ResultType::eNone ou ResultType::eTap.
		*/
		ResultType detect( const AInputEvent * event )override;

	private:
		//! L'identifiant du pointeur appuyé.
		int32_t m_downPointerID;
		//! La position du curseur lorsque l'appui a été détecté.
		gl::Vector2D m_down;
	};
	/**
	*\brief
	*	Détecte les évènements de type "double clic" sur l'écran.
	*\remarks
	*	result() renvoie la position du "double clic".
	*/
	class DoubleTapDetector
		: public GestureDetector
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] config
		*	La configuration de l'écran.
		*/
		DoubleTapDetector( AConfiguration * config )noexcept;
		/**
		*\brief
		*	Effectue la détection du "double clic", à partir de
		*	l'évènement donné.
		*\param[in] event
		*	Les informations de l'évènement enregistré.
		*\return
		*	ResultType::eNone ou ResultType::eDoubleTap.
		*/
		ResultType detect( const AInputEvent * event )override;

	private:
		//! Le détecteur de tap.
		TapDetector m_tapDetector;
		//! L'index de temps lors du tap.
		int64_t m_lastTapTime;
		//! La position lors du tap.
		gl::Vector2D m_lastTap;
	};
	/**
	*\brief
	*	Détecte les évènements de type déplacement, sur un curseur.
	*\remarks
	*	result() renvoie la valeur du déplacement.\n
	*	Permet de choisir le curseur que l'on souhaite tracker.
	*/
	class MoveDetector
		: public GestureDetector
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] config
		*	La configuration de l'écran.
		*\param[in] index
		*	L'indice du curseur tracké.
		*/
		MoveDetector( AConfiguration * config, uint32_t index = 0u )noexcept;
		/**
		*\brief
		*	Effectue la détection du déplacement, à partir de
		*	l'évènement donné.
		*\param[in] event
		*	Les informations de l'évènement enregistré.
		*\return
		*	ResultType::eNone ou ResultType::eMove.
		*/
		ResultType detect( const AInputEvent * event )override;
		/**
		*\return
		*	La position finale du curseur tracké.
		*/
		inline gl::Vector2D const & position()const noexcept
		{
			return m_down;
		}

	private:
		//! L'identifiant du pointeur tracké.
		int32_t m_downPointerID;
		//! La position du curseur lorsque l'appui a été détecté.
		gl::Vector2D m_down;
		//! L'indice du pointeur tracké.
		uint32_t m_index;
	};
	/**
	*\brief
	*	Détecte les évènements de type déplacement, sur deux curseurs.
	*\remarks
	*	result() renvoie la différence de position entre les deux curseurs.
	*/
	class DoubleMoveDetector
		: public GestureDetector
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] config
		*	La configuration de l'écran.
		*/
		DoubleMoveDetector( AConfiguration * config )noexcept;
		/**
		*\brief
		*	Effectue la détection du déplacement, à partir de
		*	l'évènement donné.
		*\param[in] event
		*	Les informations de l'évènement enregistré.
		*\return
		*	ResultType::eNone ou ResultType::eMove.
		*/
		ResultType detect( const AInputEvent * event )override;
		/**
		*\return
		*	La différence de distance enregistrée entre les déplacements.
		*/
		inline float distanceOffset()const noexcept
		{
			return m_offset;
		}

	private:
		//! Le premier détecteur de mouvements.
		MoveDetector m_main;
		//! Le second détecteur de mouvements.
		MoveDetector m_secondary;
		//! La différence de position entre les curseur lorsque l'appui a été détecté.
		gl::Vector2D m_down;
		//! La distance entre les curseurs lorsque l'appui a été détecté.
		float m_distance{ 0.0f };
		//! La différence de distance entre les curseurs.
		float m_offset{ 0.0f };
	};
}

#endif
