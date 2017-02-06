/**
*\file
*	StarMapState.h
*\author
*	Sylvain Doremus
*/
#ifndef ___StarMapLib_StarMapState_HPP___
#define ___StarMapLib_StarMapState_HPP___

#include "Star.h"
#include "CameraState.h"

namespace starmap
{
	/**
	*\brief
	*	L'état de la carte du ciel.
	*/
	struct StarMapState
	{
		//! La liste d'étoiles.
		StarArray m_stars;
		//! L'état de la caméra.
		CameraState m_cameraState;
	};
}

#endif
