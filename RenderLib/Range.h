/**
*\file
*	Range.h
*\author
*	Sylvain Doremus
*/
#ifndef ___DesktopUtils_Range_HPP___
#define ___DesktopUtils_Range_HPP___
#pragma once

#include "RenderLibPrerequisites.h"

#include <cmath>
#include <limits>

namespace render
{
	/**
	*\brief
	*	Classe d'intervalle de valeurs.
	*/
	template< typename T >
	class Range
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] min, max
		*	Les bornes de l'intervalle.
		*/
		inline Range( T const & min, T const & max )noexcept
			: m_min{ std::min( min, max ) }
			, m_max{ std::max( min, max ) }
		{
			assert( m_min != m_max );
		}
		/**
		*\brief
		*	Remet une valeur dans l'intervalle.
		*\param[in] value
		*	La valeur.
		*\return
		*	\p m_min si la valeur y est inférieure.\n
		*	\p m_max si la valeur y est supérieure.\n
		*	\p value sinon.
		*/
		T const & clamp( T const & value )const noexcept
		{
			if ( value < m_min )
			{
				return m_min;
			}

			if ( value > m_max )
			{
				return m_max;
			}

			return value;
		}
		/**
		*\brief
		*	Retourne le pourcentage correspondant à la valeur donnée.
		*\param[in] value
		*	La valeur.
		*\return
		*	0.0 si \p value est égal à ou inférieur à \p m_min.\n
		*	1.0 si \p value est égal à ou supérieur à \p m_max.\n
		*	Une pourcentage allant de 0.0 à 1.0, selon que la valeur est plus
		*	proche de \p m_min ou de \p m_max.
		*/
		float percent( T const & value )const noexcept
		{
			return float( m_min - clamp( value ) ) / float( m_min - m_max );
		}
		/**
		*\brief
		*	Retourne le pourcentage correspondant à la valeur donnée.
		*\param[in] value
		*	La valeur.
		*\return
		*	0.0 si \p value est égal à ou supérieur à \p m_max.\n
		*	1.0 si \p value est égal à ou inférieur à \p m_min.\n
		*	Une pourcentage allant de 0.0 à 1.0, selon que la valeur est plus
		*	proche de \p m_max ou de \p m_min.
		*/
		float invpercent( T const & value )const noexcept
		{
			return 1.0f - percent( value );
		}
		/**
		*\brief
		*	Retourne la valeur correspondant au pourcentage donné.
		*\param[in] percent
		*	Le pourcentage.
		*\return
		*	\p m_min si \p percent vaut 0.0.\n
		*	\p m_max si \p percent vaut 1.0.\n
		*	Une valeur comprise entre \p m_min et \p m_max.
		*/
		T value( float const & percent )const noexcept
		{
			return T{ m_min + percent * float( m_max - m_min ) };
		}
		/**
		*\return
		*	La borne minimale.
		*/
		inline T const & min()const noexcept
		{
			return m_min;
		}
		/**
		*\return
		*	La borne maximale.
		*/
		inline T const & max()const noexcept
		{
			return m_max;
		}

	private:
		//! La borne inférieure.
		T m_min;
		//! La borne supérieure.
		T m_max;
	};
	/**
	*\brief
	*	Fonction d'aide à la construction d'un intervalle.
	*\param[in] min, max
	*	Les bornes de l'intervalle.
	*\return
	*	L'intervalle créé.
	*/
	template< typename T >
	inline Range< T > makeRange( T const & min, T const & max )noexcept
	{
		return Range< T >( min, max );
	}
}

#endif
