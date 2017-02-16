/**
*\file
*	LogarithmicRange.h
*\author
*	Sylvain Doremus
*/
#ifndef ___DesktopUtils_LogarithmicRange_HPP___
#define ___DesktopUtils_LogarithmicRange_HPP___
#pragma once

#include "Range.h"

#include <cmath>
#include <limits>

namespace render
{
	/**
	*\brief
	*	Classe d'intervalle logarithmique de valeurs.
	*\remarks
	*	Implémente l'équation y = a * exp( b * x );
	*/
	template< typename T >
	class LogarithmicRange
		: public Range< T >
	{
	private:
		double doComputeB()
		{
			auto min = m_minOffset + Range< T >::min();
			auto max = m_minOffset + Range< T >::max();
			return log( max / min ) / ( max - min );
		}

		double doComputeA()
		{
			auto max = m_minOffset + Range< T >::max();
			return max / exp( max * m_b );
		}

	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] min, max
		*	Les bornes de l'intervalle.
		*/
		inline LogarithmicRange( T const & min, T const & max )noexcept
			: Range< T >{ min, max }
			, m_minOffset{ Range< T >::min() > T{ 0 }
				? T{ 0 }
				: std::abs( Range< T >::min() ) + 0.1f }
			, m_b{ doComputeB() }
			, m_a{ doComputeA() }
		{
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
		T logValue( T const & value )const noexcept
		{
			return T( ( log( value + m_minOffset ) - log( m_a ) ) / m_b ) - m_minOffset;
		}

	private:
		//! L'offset pour mettre la borne inférieure à 0.
		T m_minOffset;
		//! La valeur b de l'équation.
		double m_b;
		//! La valeur a de l'équation.
		double m_a;
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
	inline LogarithmicRange< T > makeLogarithmicRange( T const & min, T const & max )noexcept
	{
		return LogarithmicRange< T >( min, max );
	}
}

#endif
