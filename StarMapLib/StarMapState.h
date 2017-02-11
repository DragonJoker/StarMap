/**
*\file
*	StarMapState.h
*\author
*	Sylvain Doremus
*/
#ifndef ___StarMapLib_StarMapState_HPP___
#define ___StarMapLib_StarMapState_HPP___
#pragma once

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
		StarHolder( glm::vec3 const & colour
			, render::BillboardBufferPtr buffer )
			: m_colour{ colour }
			, m_buffer{ buffer }
		{
		}
		glm::vec3 m_colour;
		render::BillboardBufferPtr m_buffer;
		bool m_initialised{ false };
	};
	//! Un tableau de StarHolder.
	using StarHolderArray = std::vector< StarHolder >;
	/**
	*\brief
	*	L'état de la carte du ciel.
	*/
	struct StarMapState
	{
		//! Le tableau de StarHolders.
		StarHolderArray m_holders;
		//! L'état de la caméra.
		render::CameraState m_cameraState;
	};
}

#endif
