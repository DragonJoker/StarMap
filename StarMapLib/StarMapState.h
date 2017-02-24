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
	using String = std::array< char, 35 >;
	/**
	*\brief
	*	Structure contenant les informations nécessaires à la construction
	*	d'une étoile.
	*/
	struct StarInfo
	{
		//! L'index de l'étoile dans le tableau trié par magnitude.
		uint32_t m_index;
		//! Le nom de l'étoile.
		String m_name;
		//! La magnitude de l'étoile.
		float m_magnitude;
		//! La position de l'étoile.
		float m_position[3];
		//! La couleur de l'étoile.
		float m_colour[3];
	};
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
			String m_letter;
		};
		//! Un tableau d'étoiles
		using StarArray = std::array< Star, 125 >;
		/**
		*\brief
		*	Description d'un lien entre 2 étoiles de la constellation
		*/
		struct Link
		{
			//! l'ID de la première étoile.
			String m_a;
			//! l'ID de la seconde étoile.
			String m_b;
		};
		//! Un tableau de liens
		using LinkArray = std::array< Link, 15 >;
		//! Le nom de la constellation.
		String m_name;
		//! Les étoiles composant la constellation.
		StarArray m_stars;
		//! Les liens entre les étoiles, formant la constellation.
		LinkArray m_links;
	};
	//! Un tableau de Constellations.
	using ConstellationInfoArray = std::array< ConstellationInfo, 88 >;
	/**
	*\brief
	*	Structure de sauvegarde de l'état de la carte des étoiles.
	*/
	struct StarMapState
	{
		//! Les étoiles.
		ConstellationInfoArray m_constellations;
		//! Les étoiles.
		render::CameraState m_state;
		//! Le nombre d'étoiles.
		uint32_t m_count;
	};
}

#endif
