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
	*	Le séparateur des champs doit être ';' et le séparateur de décimales
	*	doit être '.'.
	*/
	void loadStarsFromCsv( StarMap & starmap, std::string content );
}

#endif
