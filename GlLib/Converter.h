#ifndef ___GlLib_Converter_HPP___
#define ___GlLib_Converter_HPP___
#pragma once

#include "GlLibPrerequisites.h"

namespace gl
{
	/**
	*\brief
	*	Convertit un tampon d'un format donné dans un autre format.
	*\param[in] src
	*	Le tampon source.
	*\param[in] srcs
	*	La taille du tampon source
	*\param[in,out] dst
	*	Le tampon de destination (doit déjà avoir été alloué).
	*\param[in] dstf
	*	Le format voulu pour la destination.
	*\param[in] dsts
	*	La taille du tampon de destination.
	*/
	template< PixelFormat Format >
	inline void convertBuffer( uint8_t const * src
		, size_t srcs
		, uint8_t * dst
		, PixelFormat dstf
		, size_t dsts );
}

#include "Converter.inl"

#endif
