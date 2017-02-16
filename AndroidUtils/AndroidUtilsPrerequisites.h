/**
*\file
*	AndroidUtilsPrerequisites.h
*\author
*	Sylvain Doremus
*/
#ifndef ___AndroidUtils_Prerequisites_HPP___
#define ___AndroidUtils_Prerequisites_HPP___
#pragma once

#include <RenderLib/RenderLibPrerequisites.h>

namespace utils
{
	/**
	*\name Prédéclarations.
	*/
	/**@{*/
	class FontLoader;
	class ObjLoader;
	class AndroidApp;
	class AndroidWindow;
	/**@}*/
	/**
	*\name Définitions diverses depuis les prédéclarations.
	*/
	/**@{*/
	using AndroidWindowPtr = std::unique_ptr< AndroidWindow >;
	using FontLoaderPtr = std::unique_ptr< FontLoader >;
	/**@}*/
}

#endif
