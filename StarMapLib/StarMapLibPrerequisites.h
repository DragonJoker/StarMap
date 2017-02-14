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

#include <memory>
#include <unordered_map>

namespace starmap
{
	class Constellation;
	class ScreenEvents;
	class Star;
	class StarMap;

	struct StarMapState;

	using StarMapPtr = std::unique_ptr< StarMap >;
	using StarArray = std::vector< Star >;
	using StarPtrArray = std::vector< Star const * >;
	using StarSet = std::multiset< Star >;
	using StarPtrMap = std::map< std::string, Star const * >;
	using ConstellationMap = std::unordered_map< std::string, Constellation >;

	using OnScreenTap = render::Signal< std::function< void( gl::Position2D const & ) > >;
	using OnScreenDoubleTap = render::Signal< std::function< void( gl::Position2D const & ) > >;
	using OnScreenSingleMove = render::Signal< std::function< void( gl::Position2D const & ) > >;
	using OnScreenDoubleMove = render::Signal< std::function< void( gl::Position2D const &, float ) > >;
	using OnPick = render::Signal< std::function< void( gl::Position2D const & ) > >;
	using OnReset = render::Signal< std::function< void() > >;
	using OnSetVelocity = render::Signal< std::function< void( gl::Offset2D const & ) > >;
	using OnSetZoomVelocity = render::Signal< std::function< void( float ) > >;

	using StarMapPtr = std::unique_ptr< StarMap >;

	/**
	*\brief
	*	Le type des éléments supportés pour les filtres.
	*/
	enum class ElementType
	{
		//! Etoiles.
		eStar,
		//! Constellations.
		eConstellation,
	};
}

#endif
