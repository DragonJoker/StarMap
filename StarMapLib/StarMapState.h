/**
*\file
*	StarMapState.h
*\author
*	Sylvain Doremus
*/
#ifndef ___StarMapLib_StarMapState_HPP___
#define ___StarMapLib_StarMapState_HPP___
#pragma once

#include "Constellation.h"
#include "Star.h"

#include <RenderLib/CameraState.h>

namespace starmap
{
	/**
	*\brief
	*	Structure contenant les billboards pour une couleur d'étoile
	*/
	struct StarHolder
	{
		StarHolder( gl::RgbColour const & colour
			, render::BillboardBufferPtr buffer )
			: m_colour{ colour }
			, m_buffer{ buffer }
		{
		}

		gl::RgbColour m_colour;
		render::BillboardBufferPtr m_buffer;
		bool m_initialised{ false };
		StarPtrArray m_stars;
	};
	//! Un tableau de StarHolder.
	using StarHolderArray = std::vector< StarHolder >;
}

#endif
