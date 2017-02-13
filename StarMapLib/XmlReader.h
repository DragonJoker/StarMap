/**
*\file
*	XmlReader.h
*\author
*	Sylvain Doremus
*/
#ifndef ___StarMapLib_XmlReader_HPP___
#define ___StarMapLib_XmlReader_HPP___
#pragma once

#include "StarMapLibPrerequisites.h"

namespace starmap
{
	/**
	*\brief
	*	Charge des étoiles depuis un fichier XML.
	*/
	void loadStarsFromXml( StarMap & starmap, std::string content );
	/**
	*\brief
	*	Charge des constellations depuis un fichier XML.
	*/
	void loadConstellationsFromXml( StarMap & starmap, std::string content );
}

#endif
