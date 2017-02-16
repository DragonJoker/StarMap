/**
*\file
*	DesktopUtilsPrerequisites.h
*\author
*	Sylvain Doremus
*/
#ifndef ___DesktopUtils_Prerequisites_HPP___
#define ___DesktopUtils_Prerequisites_HPP___
#pragma once

#include <RenderLib/RenderLibPrerequisites.h>

#include <chrono>

namespace utils
{
	using Clock = std::chrono::high_resolution_clock;

	class FontLoader;
	class ObjLoader;
	/**
	*\brief
	*	Ev�nement de type redimensionnement de fen�tre.
	*/
	struct SizeEvent
	{
		//! Les nouvelle dimensions
		gl::Size2D size;
		//! Dit si la fen�tre est maximis�e.
		bool maximized;
		//! Dit si la fen�tre est minimis�e.
		bool minimized;
		//! Dit si la fen�tre est restaur�e.
		bool restored;
	};
	/**
	*\brief
	*	Ev�nement li� � la souris.
	*/
	struct MouseEvent
	{
		//! La position actuelle de la souris.
		gl::Position2D position;
		//! \p true si le bouton gauche de la souris est enfonc�.
		bool ldown;
		//! \p true si le bouton milieu de la souris est enfonc�.
		bool mdown;
		//! \p true si le bouton droit de la souris est enfonc�.
		bool rdown;
		//! Le delta de la molette.
		int delta;
	};
}

#endif
