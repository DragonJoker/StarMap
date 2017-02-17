/**
*\file
*	Vec3.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlLib_Vec3_HPP___
#define ___GlLib_Vec3_HPP___
#pragma once

#include "Vec2.h"

namespace gl
{
	/**
	*\brief
	*	Point à 3 composantes.
	*/
	template< typename T >
	class Vec3T
	{
	public:
		/**
		*\brief
		*	Constructeur sans initialisation.
		*/
		Vec3T( NoInit const & )noexcept;
		/**
		*\brief
		*	Constructeur par défaut.
		*/
		constexpr Vec3T()noexcept;
		/**
		*\brief
		*	Constructeur par copie.
		*\param[in] rhs
		*	Le point à copier.
		*/
		Vec3T( Vec3T< T > const & rhs ) = default;
		/**
		*\brief
		*	Opérateur d'affectation par copie.
		*\param[in] rhs
		*	Le point à copier.
		*\return
		*	Une référence sur ce point.
		*/
		Vec3T & operator=( Vec3T< T > const & rhs ) = default;
		/**
		*\brief
		*	Constructeur.
		*\param[in] x, y
		*	Les valeurs des composantes.
		*/
		template< typename X
			, typename Y
			, typename Z >
		constexpr Vec3T( X const & x
			, Y const & y
			, Z const & z )noexcept;
		/**
		*\brief
		*	Constructeur de conversion.
		*\param[in] rhs
		*	Le point à convertir.
		*/
		template< typename U >
		Vec3T( Vec3T< U > const & rhs )noexcept;
		/**
		*\brief
		*	Opérateur d'affectation par conversion.
		*\param[in] rhs
		*	Le point à convertir.
		*\return
		*	Une référence sur ce point.
		*/
		template< typename U >
		Vec3T & operator=( Vec3T< U > const & rhs )noexcept;
		/**
		*\brief
		*	Opérateur d'indexation.
		*\param[in] index
		*	L'indice de la composante à récupérer.
		*\return
		*	Une référence sur la composante.
		*/
		T & operator[]( size_t index )noexcept;
		/**
		*\brief
		*	Opérateur d'indexation.
		*\param[in] index
		*	L'indice de la composante à récupérer.
		*\return
		*	Une référence sur la composante.
		*/
		T const & operator[]( size_t index )const noexcept;
		/**
		*\name Opérateurs arithmétiques membres.
		*/
		/**@{*/
		inline Vec3T & operator-()noexcept;
		template< typename U >
		inline Vec3T & operator+=( Vec3T< U > const & rhs )noexcept;
		template< typename U >
		inline Vec3T & operator-=( Vec3T< U > const & rhs )noexcept;
		template< typename U >
		inline Vec3T & operator*=( Vec3T< U > const & rhs )noexcept;
		template< typename U >
		inline Vec3T & operator+=( U const & rhs )noexcept;
		template< typename U >
		inline Vec3T & operator-=( U const & rhs )noexcept;
		template< typename U >
		inline Vec3T & operator*=( U const & rhs )noexcept;
		template< typename U >
		inline Vec3T & operator/=( U const & rhs )noexcept;
		/**@}*/

	public:
		//! Les composantes du point.
		union
		{
			T data[3];
			struct
			{
				T x;
				T y;
				T z;
			};
			struct
			{
				T r;
				T g;
				T b;
			};
		};
	};
	/**
	*\brief
	*	Produit scalaire de 2 points.
	*\param[in] lhs, rhs
	*	Les points.
	*\return
	*	La produit scalaire.
	*/
	template< typename T >
	T dot( Vec3T< T > const & lhs, Vec3T< T > const & rhs )noexcept;
	/**
	*\brief
	*	Produit vectoriel de 2 points.
	*\param[in] lhs, rhs
	*	Les points.
	*\return
	*	La produit vectoriel.
	*/
	template< typename T >
	Vec3T< T > cross( Vec3T< T > const & lhs, Vec3T< T > const & rhs )noexcept;
	/**
	*\brief
	*	Donne la norme d'un point.
	*\param[in] vec
	*	Le point.
	*\return
	*	La norme.
	*/
	template< typename T >
	T length( Vec3T< T > const & vec )noexcept;
	/**
	*\brief
	*	Donne la distance entre 2 points.
	*\param[in] lhs, rhs
	*	Les points.
	*\return
	*	La distance.
	*/
	template< typename T >
	T distance( Vec3T< T > const & lhs, Vec3T< T > const & rhs )noexcept;
	/**
	*\brief
	*	Normalise un point (divise chaque composante par la norme du point.
	*\param[in] vec
	*	Le point à normaliser.
	*\return
	*	Le point normalisé.
	*/
	template< typename T >
	Vec3T< T > normalize( Vec3T< T > const & vec )noexcept;
	/**
	*\name Opérateurs logiques.
	*/
	/**@{*/
	template< typename T >
	inline bool operator==( Vec3T< T > const & lhs
		, Vec3T< T > const & rhs )noexcept;
	template< typename T >
	inline bool operator!=( Vec3T< T > const & lhs
		, Vec3T< T > const & rhs )noexcept;
	/**@}*/
	/**
	*\name Opérateurs arithmétiques.
	*/
	/**@{*/
	template< typename T, typename U >
	inline Vec3T< T > operator+( Vec3T< T > const & lhs
		, Vec3T< U > const & rhs )noexcept;
	template< typename T, typename U >
	inline Vec3T< T > operator-( Vec3T< T > const & lhs
		, Vec3T< U > const & rhs )noexcept;
	template< typename T, typename U >
	inline Vec3T< T > operator*( Vec3T< T > const & lhs
		, Vec3T< U > const & rhs )noexcept;
	template< typename T, typename U >
	inline Vec3T< T > operator+( Vec3T< T > const & lhs
		, U const & rhs )noexcept;
	template< typename T, typename U >
	inline Vec3T< T > operator-( Vec3T< T > const & lhs
		, U const & rhs )noexcept;
	template< typename T, typename U >
	inline Vec3T< T > operator*( Vec3T< T > const & lhs
		, U const & rhs )noexcept;
	template< typename T, typename U >
	inline Vec3T< T > operator/( Vec3T< T > const & lhs
		, U const & rhs )noexcept;
	/**@}*/
}

#include "Vec3.inl"

#endif
