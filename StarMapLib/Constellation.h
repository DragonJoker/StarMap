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
		void addStar( std::string const & letter
			, std::string const & name );
		/**
		*\brief
		*	Ajoute un lien entre 2 étoiles appartenant à la constellation.
		*\param[in] a, b
		*	Les lettres des 2 étoiles.
		*/
		void addLink( std::string const & a
			, std::string const & b );
		/**
		*\return
		*	Le nom de l'étoile.
		*/
		inline std::string const & name()const noexcept
		{
			return m_name;
		}
		/**
		*\return
		*	La liste des liens composant cette constellation.
		*/
		inline LinkArray const & links()const noexcept
		{
			return m_links;
		}
		/**
		*\return
		*	La liste de étoiles composant cette constellation.
		*/
		inline StarPtrMap const & stars()const noexcept
		{
			return m_letters;
		}
		/**
		*\brief
		*	Définit la position de la constellation.
		*\param[in] position
		*	La nouvelle valeur.
		*/
		inline void position( gl::Vector3D const & position )noexcept
		{
			m_position = position;
		}
		/**
		*\return
		*	La position de la constellation.
		*/
		inline gl::Vector3D const & position()const noexcept
		{
			return m_position;
		}

	private:
		std::string m_name;
		StarSet const & m_stars;
		StarPtrMap m_letters;
		LinkArray m_links;
		gl::Vector3D m_position;
	};
}

#endif
