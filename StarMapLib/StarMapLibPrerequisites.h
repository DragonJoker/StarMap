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
	using StarSet = std::multiset< Star >;
	using ConstellationMap = std::unordered_map< std::string, Constellation >;

	using OnScreenTap = render::Signal< std::function< void( glm::ivec2 const & ) > >;
	using OnScreenDoubleTap = render::Signal< std::function< void( glm::ivec2 const & ) > >;
	using OnScreenSingleMove = render::Signal< std::function< void( glm::ivec2 const & ) > >;
	using OnScreenDoubleMove = render::Signal< std::function< void( glm::ivec2 const &, float ) > >;
	using OnPick = render::Signal< std::function< void( glm::ivec2 const & ) > >;
	using OnReset = render::Signal< std::function< void() > >;
	using OnSetVelocity = render::Signal< std::function< void( glm::ivec2 const & ) > >;
	using OnSetZoomVelocity = render::Signal< std::function< void( float ) > >;

	using StarMapPtr = std::unique_ptr< StarMap >;
}

#endif
