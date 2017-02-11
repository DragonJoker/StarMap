/**
*\file
*	Constellation.h
*\author
*	Sylvain Doremus
*/
#ifndef ___StarMapLib_Constellation_HPP___
#define ___StarMapLib_Constellation_HPP___
#pragma once

#include "StarMapLibPrerequisites.h"

namespace starmap
{
	/**
	*\brief
	*	Une constellation, avec tout le nécessaire permettant de l'afficher.
	*/
	class Constellation
	{
	private:
		using StarLetters = std::unordered_map< std::string, std::string >;
		struct Link
		{
			Star const * m_a;
			Star const * m_b;
		};
		using LinkArray = std::vector< Link >;

	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] name
		*	Le nom de la constellation.
		*\param[in] stars
		*	La liste des étoiles.
		*/
		Constellation( std::string const & name
			, StarSet const & stars );
		/**
		*\brief
		*	Ajoute une étoile à la constellation.
		*\param[in] letter
		*	La lettre grecque désignant l'étoile dans la constellation.
		*/
		void AddStar( std::string const & letter
			, std::string const & name );
		/**
		*\brief
		*	Ajoute un lien entre 2 étoiles appartenant à la constellation.
		*\param[in] a, b
		*	Les lettres des 2 étoiles.
		*/
		void AddLink( std::string const & a
			, std::string const & b );
		/**
		*\return
		*	Le nom de l'étoile.
		*/
		inline std::string const & getName()const
		{
			return m_name;
		}
		/**
		*\return
		*	L'itérateur de début des liens.
		*/
		inline typename LinkArray::const_iterator begin()const
		{
			return m_links.begin();
		}
		/**
		*\return
		*	L'itérateur de fin des liens.
		*/
		inline typename LinkArray::const_iterator end()const
		{
			return m_links.end();
		}

	private:
		std::string m_name;
		StarSet const & m_stars;
		StarLetters m_letters;
		LinkArray m_links;
	};
}

#endif
