/**
*\file
*	Transform.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlLib_Transform_HPP___
#define ___GlLib_Transform_HPP___
#pragma once

#include "Mat4.h"
#include "Vec3.h"

namespace gl
{
	/**
	*\brief
	*	Effectue sur \p m une translation du vecteur \p v donné.
	*\param[in] m
	*	La matrice.
	*\param[in] v
	*	La translation.
	*\return
	*	Le résultat de la translation.
	*/
	template< typename T >
	Mat4T< T > translate( Mat4T< T > const & m, Vec3T< T > const & v );
	/**
	*\brief
	*	Effectue sur \p m une mise à l'échelle de du vecteur \p v donné.
	*\param[in] m
	*	La matrice.
	*\param[in] v
	*	La valeur de mise à l'échelle.
	*\return
	*	Le résultat de la mise à l'échelle.
	*/
	template< typename T >
	Mat4T< T > scale( Mat4T< T > const & m, Vec3T< T > const & v );
	/**
	*\brief
	*	Effectue sur \p m une rotation de \p angle autour de l'axe \p v donné.
	*\param[in] m
	*	La matrice.
	*\param[in] angle
	*	L'angle de rotation.
	*\param[in] v
	*	L'axe de rotation.
	*\return
	*	Le résultat de la rotation.
	*/
	template< typename T >
	Mat4T< T > rotate( Mat4T< T > const & m, T angle, Vec3T< T > const & v );
}

#include "Transform.inl"

#endif
