/**
*\file
*	DesktopUtilsPrerequisites.h
*\author
*	Sylvain Doremus
*/
#ifndef ___DesktopUtils_Prerequisites_HPP___
#define ___DesktopUtils_Prerequisites_HPP___
#pragma once

#include <GlLib/Converter.h>

#include <RenderLib/ElementsList.h>
#include <RenderLib/FontLoader.h>
#include <RenderLib/Glyph.h>
#include <RenderLib/Material.h>
#include <RenderLib/Mesh.h>
#include <RenderLib/Object.h>
#include <RenderLib/StringUtils.h>
#include <RenderLib/Texture.h>

#include <stdlib.h>
#include <string.h>

#include <chrono>
#include <fstream>
#include <sstream>

#if defined( __GNUG__ )
#	define COMPILER_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#	if COMPILER_VERSION > 40900
#		pragma GCC diagnostic ignored "-Wdeprecated-register"
#	endif
#endif

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
