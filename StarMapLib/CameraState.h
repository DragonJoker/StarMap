/**
*\file
*	CameraState.h
*\author
*	Sylvain Doremus
*/
#ifndef ___StarMapLib_CameraState_HPP___
#define ___StarMapLib_CameraState_HPP___
#pragma once

#include "StarMapLibPrerequisites.h"

namespace starmap
{
/**
*\brief
*	Données de déplacement de la caméra.
*/
	struct CameraState
	{
	public:
		/**
		*\brief
		*	Réinitialise l'état.
		*/
		void reset();
		/**
		*\brief
		*	Met à jour le zoom et l'angle en fonction des vitesses.
		*/
		void update();
		/**
		*\brief
		*	Définit la vitesse de rotation de la caméra.
		*\param[in] value
		*	La nouvelle valeur.
		*/
		inline void setVelocity( glm::vec2 const & value ) noexcept
		{
			m_velocity = value;
		}
		/**
		*\brief
		*	Définit la vitesse de zoom de la caméra.
		*\param[in] value
		*	La nouvelle valeur.
		*/
		inline void setZoomVelocity( float value ) noexcept
		{
			m_zoomVelocity = value;
		}
		/**
		*\return
		*	L'angle de rotation de la caméra, sur les axes X et Y.
		*/
		inline glm::vec2 const & getAngle()const noexcept
		{
			return m_angle;
		}
		/**
		*\return
		*	La valeur actuelle du zoom.
		*/
		inline float getZoom()const noexcept
		{
			return m_zoom;
		}

	private:
		glm::vec2 m_velocity{ 0, 0 };
		glm::vec2 m_angle{ 0, 0 };
		float m_zoomVelocity{ 0 };
		float m_zoom{ 0 };
	};
}
#endif
