/**
*\file
*	CsvReader.h
*\author
*	Sylvain Doremus
*/
#ifndef ___StarMapLib_CsvReader_HPP___
#define ___StarMapLib_CsvReader_HPP___
#pragma once

#include "StarMapLibPrerequisites.h"

namespace starmap
{
	/**
	*\brief
	*	Charge des étoiles depuis un fichier CSV.
	*\remarks
	*	Le séparateur des champs doit être ',' et le séparateur de décimales
	*	doit être '.'.\n
	*	La première ligne doit servir à décrire les éléments du CSV, afin de
	*	savoir quels champs en seront récupérés.\n
	*	De ce fait, le loader cherche les champs suivants:\n
	*\li proper: Le nom commun de l'étoile (optionnel).
	*\li gl: L'ID de l'étoile dans le Gliese Catalog of Nearby Stars (optionnel).
	*\li hip: L'ID de l'étoile dans le catalogue Hipparcos.
	*\li rarad: L'ascension de l'étoile, en radians.
	*\li decrad: La déclinaison de l'étoile, en radians.
	*\li mag: La magnitude de l'étoile.
	*\li spect: Le type spectral de l'étoile (optionnel).
	*\li ci: L'index de la couleur de l'étoile (optionnel).
	*
	*/
	void loadStarsFromCsv( StarMap & starmap, std::string content );
}

#endif
