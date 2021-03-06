/**
*\file
*	StarMapLibPrerequisites.h
*\author
*	Sylvain Doremus
*/
#ifndef ___StarMapLib_Prerequisites_HPP___
#define ___StarMapLib_Prerequisites_HPP___
#pragma once

#include <RenderLib/RenderLibPrerequisites.h>

#include <errno.h>
#include <string.h>

#include <memory>
#include <unordered_map>

namespace starmap
{
	class Constellation;
	class ConstellationStar;
	class ScreenEvents;
	class Star;
	class StarMap;

	struct StarMapState;

	using StarMapPtr = std::unique_ptr< StarMap >;
	using ConstellationPtr = std::unique_ptr< Constellation >;
	using StarArray = std::vector< Star >;
	using StarPtrArray = std::vector< Star const * >;
	using StarPtrMap = std::map< std::string, Star const * >;
	using ConstellationMap = std::unordered_map< std::string, ConstellationPtr >;
	using ConstellationStarArray = std::vector< ConstellationStar >;

	using OnScreenTap = render::Signal< std::function< void( gl::IVec2 const & ) > >;
	using OnScreenDoubleTap = render::Signal< std::function< void( gl::IVec2 const & ) > >;
	using OnScreenSingleMove = render::Signal< std::function< void( gl::IVec2 const & ) > >;
	using OnScreenDoubleMove = render::Signal< std::function< void( gl::IVec2 const &, float ) > >;
	using OnPick = render::Signal< std::function< void( gl::IVec2 const & ) > >;
	using OnReset = render::Signal< std::function< void() > >;
	using OnSetVelocity = render::Signal< std::function< void( gl::IVec2 const & ) > >;
	using OnSetZoomVelocity = render::Signal< std::function< void( float ) > >;

	using StarMapPtr = std::unique_ptr< StarMap >;

	/**
	*\brief
	*	Le type des �l�ments support�s pour les filtres.
	*/
	enum class ElementType
	{
		//! Etoiles.
		eStar,
		//! Constellations.
		eConstellation,
	};
	/**
	*\brief
	*	Convertit un index de couleur B-V en couleur RVB
	*\param[in] bv
	*	L'index de la couleur.
	*\return
	*	La couleur correspondante.
	*/
	gl::RgbColour bvToRgb( float bv );
}

#endif
