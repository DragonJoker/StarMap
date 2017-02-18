/**
*\file
*	Vectorisation.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlLib_Vectorisation_HPP___
#define ___GlLib_Vectorisation_HPP___
#pragma once

#include "Vec4.h"

namespace gl
{
	/**
	*\brief
	*	Appelle une fonction sur chaque composante du vecteur donn�.
	*\remarks
	*	Permet d'appeler, par exemple, des fonctions math�matiques telles que
	*	std::abs, std::cos... sur les composantes d'un vecteur, ce en un seul
	*	appel, et de stocker le r�sultat dans un vecteur.
	*\param[in] func
	*	La fonction.
	*\param[in] vector
	*	Le vecteur (gl::Vec2, gl::Vec3 ou gl::Vec4)
	*\return
	*	Le vecteur r�sultant des appels.
	*/
	template< typename R, typename T, typename V >
	inline V vectorCall( R( *func )( T ), V const & vector );
	/**
	*\brief
	*	Appelle une fonction sur chaque composante du vecteur donn�.
	*\remarks
	*	Permet d'appeler, par exemple, des fonctions math�matiques telles que
	*	std::abs, std::cos... sur les composantes d'un vecteur, ce en un seul
	*	appel, et de stocker le r�sultat dans un vecteur.
	*	Version pour les fonctions prenant en param�tre une r�f�rence constante
	*	(comme gl::cos, par exemple).
	*\param[in] func
	*	La fonction.
	*\param[in] vector
	*	Le vecteur (gl::Vec2, gl::Vec3 ou gl::Vec4)
	*\return
	*	Le vecteur r�sultant des appels.
	*/
	template< typename R, typename T, typename V >
	inline V vectorCall( R( *func )( T const & ), V const & vector );
}

#include "Vectorisation.inl"

#endif
