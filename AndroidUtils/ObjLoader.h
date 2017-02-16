/**
*\file
*	ObjLoader.h
*\author
*	Sylvain Doremus
*/
#ifndef ___ObjLoader_HPP___
#define ___ObjLoader_HPP___
#pragma once

#include "AndroidUtils.h"

namespace utils
{
	/**
	*\brief
	*	Charge un fichier de matériaux et met les matériaux chargés dans la map donnée.
	*\param[in] activity
	*	L'activité permettant de récupérer les fichiers supplémentaires.
	*\param[in] fileContent
	*	Le contenu du fichier MTL.
	*\param[in,out] materialsList
	*	Reçoit les matériaux chargés.
	*\param[in,out] texturesList
	*	Reçoit les textures chargées.
	*/
	void loadMtlFile( AndroidApp const & activity
		, std::string const & fileContent
		, render::MaterialList & materialsList
		, render::TextureList & texturesList );
	/**
	*\brief
	*	Charge un objet depuis un fichier OBJ.
	*\param[in] activity
	*	L'activité permettant de récupérer les fichiers supplémentaires.
	*\param[in] name
	*	Le nom de l'objet.
	*\param[in] fileContent
	*	Le contenu du fichier OBJ.
	*\param[in,out] materialsList
	*	Reçoit les matériaux chargés.
	*\param[in,out] texturesList
	*	Reçoit les textures chargées.
	*\param[in,out] meshesList
	*	Reçoit les maillages chargés.
	*\return
	*	L'objet chargé.
	*/
	render::ObjectPtr loadObjFile( AndroidApp const & activity
		, std::string const & name
		, std::string const & fileContent
		, render::MaterialList & materialsList
		, render::TextureList & texturesList
		, render::MeshList & meshesList );
}

#endif
