/**
*\file
*	Quaternion.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlLib_Quaternion_HPP___
#define ___GlLib_Quaternion_HPP___
#pragma once

#include "Mat4.h"

namespace gl
{
	/**
	*\brief
	*	Point à 4 composantes.
	*/
	template< typename T >
	class QuaternionT
	{
	public:
		/**
		*\brief
		*	Constructeur sans initialisation.
		*/
		QuaternionT( NoInit const & )noexcept;
		/**
		*\brief
		*	Constructeur par défaut.
		*/
		constexpr QuaternionT()noexcept;
		/**
		*\brief
		*	Constructeur depuis 2 axes.
		*\param[in] u, v
		*	Les 2 axes.
		*/
		QuaternionT( Vec3T< T > const & u, Vec3T< T > const & v )noexcept;
		/**
		*\brief
		*	Constructeur depuis 3 angles d'Euler.
		*\param[in] euler
		*	Les angles.
		*/
		QuaternionT( Vec3T< T > const & euler )noexcept;
		/**
		*\brief
		*	Constructeur par copie.
		*\param[in] rhs
		*	Le point à copier.
		*/
		QuaternionT( QuaternionT< T > const & rhs ) = default;
		/**
		*\brief
		*	Opérateur d'affectation par copie.
		*\param[in] rhs
		*	Le point à copier.
		*\return
		*	Une référence sur ce point.
		*/
		QuaternionT & operator=( QuaternionT< T > const & rhs ) = default;
		/**
		*\brief
		*	Constructeur.
		*\param[in] w, x, y, z
		*	Les valeurs des composantes.
		*/
		template< typename W
			, typename X
			, typename Y
			, typename Z >
		constexpr QuaternionT( W const & w
			, X const & x
			, Y const & y
			, Z const & z )noexcept;
		/**
		*\brief
		*	Constructeur de conversion.
		*\param[in] rhs
		*	Le point à convertir.
		*/
		template< typename U >
		QuaternionT( QuaternionT< U > const & rhs )noexcept;
		/**
		*\brief
		*	Opérateur d'affectation par conversion.
		*\param[in] rhs
		*	Le point à convertir.
		*\return
		*	Une référence sur ce point.
		*/
		template< typename U >
		QuaternionT & operator=( QuaternionT< U > const & rhs )noexcept;
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
		inline QuaternionT & operator-()noexcept;
		template< typename U >
		inline QuaternionT & operator+=( QuaternionT< U > const & rhs )noexcept;
		template< typename U >
		inline QuaternionT & operator-=( QuaternionT< U > const & rhs )noexcept;
		template< typename U >
		inline QuaternionT & operator*=( QuaternionT< U > const & rhs )noexcept;
		template< typename U >
		inline QuaternionT & operator*=( U const & rhs )noexcept;
		template< typename U >
		inline QuaternionT & operator/=( U const & rhs )noexcept;
		/**@}*/

	public:
		//! Les composantes du point.
		union
		{
			T data[4];
			struct
			{
				T x;
				T y;
				T z;
				T w;
			};
			struct
			{
				T r;
				T g;
				T b;
				T a;
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
	T dot( QuaternionT< T > const & lhs, QuaternionT< T > const & rhs )noexcept;
	/**
	*\brief
	*	Donne la norme d'un point.
	*\param[in] vec
	*	Le point.
	*\return
	*	La norme.
	*/
	template< typename T >
	T length( QuaternionT< T > const & vec )noexcept;
	/**
	*\brief
	*	Donne la distance entre 2 points.
	*\param[in] lhs, rhs
	*	Les points.
	*\return
	*	La distance.
	*/
	template< typename T >
	T distance( QuaternionT< T > const & lhs, QuaternionT< T > const & rhs )noexcept;
	/**
	*\brief
	*	Normalise un point (divise chaque composante par la norme du point.
	*\param[in] vec
	*	Le point à normaliser.
	*\return
	*	Le point normalisé.
	*/
	template< typename T >
	QuaternionT< T > normalize( QuaternionT< T > const & vec )noexcept;
	/**
	*\brief
	*	Effectue une rotation du vecteur \p v par le quaternion \q.
	*\param[in] q
	*	Le quaternion.
	*\param[in] v
	*	Le vecteur à transformer.
	*\return
	*	Le vecteur transformé.
	*/
	template< typename T, typename U >
	inline Vec3T< T > rotate( QuaternionT< T > const & lhs
		, Vec3T< U > const & rhs )noexcept;
	/**
	*\brief
	*	Effectue une rotation du vecteur \p v par le quaternion \q.
	*\param[in] q
	*	Le quaternion.
	*\param[in] v
	*	Le vecteur à transformer.
	*\return
	*	Le vecteur transformé.
	*/
	template< typename T, typename U >
	inline Vec4T< T > rotate( QuaternionT< T > const & lhs
		, Vec4T< U > const & rhs )noexcept;
	/**
	*\brief
	*	Crée une matrice de rotation à partir du quaternion donné.
	*\param[in] q
	*	Le quaternion.
	*\return
	*	La matrice de rotation.
	*/
	template< typename T >
	Mat4T< T > toMat4( QuaternionT< T > const & q )noexcept;
	/**
	*\name Opérateurs logiques.
	*/
	/**@{*/
	template< typename T >
	inline bool operator==( QuaternionT< T > const & lhs
		, QuaternionT< T > const & rhs )noexcept;
	template< typename T >
	inline bool operator!=( QuaternionT< T > const & lhs
		, QuaternionT< T > const & rhs )noexcept;
	/**@}*/
	/**
	*\name Opérateurs arithmétiques.
	*/
	/**@{*/
	template< typename T, typename U >
	inline QuaternionT< T > operator+( QuaternionT< T > const & lhs
		, QuaternionT< U > const & rhs )noexcept;
	template< typename T, typename U >
	inline QuaternionT< T > operator-( QuaternionT< T > const & lhs
		, QuaternionT< U > const & rhs )noexcept;
	template< typename T, typename U >
	inline QuaternionT< T > operator*( QuaternionT< T > const & lhs
		, QuaternionT< U > const & rhs )noexcept;
	template< typename T, typename U >
	inline QuaternionT< T > operator*( QuaternionT< T > const & lhs
		, U const & rhs )noexcept;
	template< typename T, typename U >
	inline QuaternionT< T > operator/( QuaternionT< T > const & lhs
		, U const & rhs )noexcept;
	template< typename T, typename U >
	inline Vec3T< T > operator*( QuaternionT< T > const & lhs
		, Vec3T< U > const & rhs )noexcept;
	template< typename T, typename U >
	inline Vec4T< T > operator*( QuaternionT< T > const & lhs
		, Vec4T< U > const & rhs )noexcept;
	/**@}*/
}

#include "Quaternion.inl"

#endif
