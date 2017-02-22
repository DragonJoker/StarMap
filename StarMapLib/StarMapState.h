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
	*	Structure contenant les informations nécessaires à la construction
	*	d'une constellation.
	*/
	struct ConstellationInfo
	{
		/**
		*\brief
		*	Une étoile dans une constellation.
		*/
		struct Star
		{
			//! L'Index de l'étoile dans la carte des étoiles.
			uint32_t m_index;
			//! L'ID de l'étoile dans la constellation.
			uint32_t m_id;
			//! La lettre associée à l'étoile, dans la constellation.
			std::string m_letter;
		};
		//! Un tableau d'étoiles
		using StarArray = std::vector< Star >;
		/**
		*\brief
		*	Description d'un lien entre 2 étoiles de la constellation
		*/
		struct Link
		{
			//! l'ID de la première étoile.
			uint32_t m_a;
			//! l'ID de la seconde étoile.
			uint32_t m_b;
		};
		//! Un tableau de liens
		using LinkArray = std::vector< Link >;
		//! Le nom de la constellation.
		std::string m_name;
		//! Les étoiles composant la constellation.
		StarArray m_stars;
		//! Les liens entre les étoiles, formant la constellation.
		LinkArray m_links;
		//! La position de la constellation, fonction des étoiles qui la composent.
		gl::Vec3 m_position;
	};
	//! Un tableau de Constellations.
	using ConstellationInfoArray = std::vector< ConstellationInfo >;
	/**
	*\brief
	*	Structure de sauvegarde de l'état de la carte des étoiles.
	*/
	struct StarMapState
	{
		//! Les étoiles.
		StarArray m_stars;
		//! Les étoiles.
		ConstellationInfoArray m_constellations;
		//! Les étoiles.
		render::CameraState m_state;
	};
}

#endif
