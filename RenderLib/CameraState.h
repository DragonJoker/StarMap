/**
*\file
*	CameraState.h
*\author
*	Sylvain Doremus
*/
#ifndef ___DesktopUtils_CameraState_HPP___
#define ___DesktopUtils_CameraState_HPP___
#pragma once

#include "Angle.h"
#include "RangedValue.h"

namespace render
{
	/**
	*\brief
	*	Classe de gestion des déplacements de la caméra.
	*/
	class CameraState
	{
	public:
		/**
		*\brief
		*	Remet les vitesses et angles à 0.
		*/
		void reset();
		/**
		*\brief
		*	Met à jour l'angle et le zoom en fonction des vitesses.
		*	
		*/
		void update();
		/**
		*\brief
		*	Définit les angles d'ouverture minimal et maximal.
		*\param[in] fovyMin, fovyMax
		*	Les bornes.
		*/
		inline void zoomBounds( Angle const & fovyMin
			, Angle const & fovyMax )noexcept
		{
			m_fovy.updateRange( makeRange( fovyMin, fovyMax ) );
		}
		/**
		*\return
		*	Les bornes du zoom.
		*/
		inline Range< Angle > const & zoomBounds()const noexcept
		{
			return m_fovy.range();
		}
		/**
		*\brief
		*	Définit la vitesse de rotation de la caméra.
		*param[in] value
		*	La nouvelle valeur.
		*/
		inline void velocity( gl::Vector2D const & value )noexcept
		{
			m_velocityX = Degrees{ value.x };
			m_velocityY = Degrees{ value.y };
		}
		/**
		*\brief
		*	Définit la vitesse de zoom de la caméra.
		*param[in] value
		*	La nouvelle valeur.
		*/
		inline void zoomVelocity( float value )noexcept
		{
			m_zoomVelocity = Degrees{ value };
		}
		/**
		*\return
		*	Les angles voulus pour l'orientation de la caméra.
		*/
		inline gl::Vector2D angle()const noexcept
		{
			return{ m_angleX.value(), m_angleY.value() };
		}
		/**
		*\return
		*	La valeur du zoom.
		*/
		inline Angle const & zoom()const noexcept
		{
			return m_fovy.value();
		}

	private:
		//! L'intervalle d'angles de rotation.
		Range< Radians > m_angleRange
		{
			Radians{ float( -Pi ) / 6.0f },
			Radians{ float( Pi ) / 6.0f }
		};
		//! La rotation sur l'axe X.
		RangedValue< Radians > m_angleX
		{
			0.0_radians,
			m_angleRange
		};
		//! La rotation sur l'axe Y.
		RangedValue< Radians > m_angleY
		{
			0.0_radians,
			m_angleRange
		};
		//! L'intervalle de vitesse de rotation.
		Range< Degrees > m_velocityRange
		{
			-5.0_degrees,
			5.0_degrees
		};
		//! La vitesse de rotation sur l'axe X.
		RangedValue< Degrees > m_velocityX
		{
			0.0_degrees,
			m_velocityRange
		};
		//! La vitesse de rotation sur l'axe Y.
		RangedValue< Degrees > m_velocityY
		{
			0.0_degrees,
			m_velocityRange
		};
		//! La vitesse de zoom.
		RangedValue< Degrees > m_zoomVelocity
		{
			0.0_degrees,
			makeRange( -2.0_degrees, 2.0_degrees )
		};
		//! Le FovY utilisé pour le zoom, borné dans son intervalle.
		RangedValue< Angle > m_fovy
		{
			Angle{ 45.0_degrees },
			makeRange( Angle{ 0.1_degrees }, Angle{ 90.0_degrees } )
		};
	};
}

#endif
