/**
*\file
*	Constellation.h
*\author
*	Sylvain Doremus
*/
#ifndef ___StarMapLib_Constellation_HPP___
#define ___StarMapLib_Constellation_HPP___
#pragma once

#include "ConstellationStar.h"

namespace starmap
{
	/**
	*\brief
	*	Une constellation, avec tout le nécessaire permettant de l'afficher.
	*/
	class Constellation
	{
	private:
		/**
		*\brief
		*	Description d'un lien entre 2 étoiles de la constellation
		*/
		struct Link
		{
			ConstellationStar const * m_a;
			ConstellationStar const * m_b;
		};
		//! Un tableau de liens
		using LinkArray = std::vector< Link >;

	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] name
		*	Le nom de la constellation.
		*/
		Constellation( std::string const & name );
		/**
		*\brief
		*	Ajoute une étoile à la constellation.
		*\param[in] id
		*	L'ID de l'étoile dans la constellation.
		*\param[in] letter
		*	La lettre associée à l'étoile, dans la constellation.
		*\param[in] name
		*	Le nom de l'étoile.
		*/
		void addStar( uint32_t id
			, std::string const & letter
			, std::string const & name );
		/**
		*\brief
		*	Cherche une étoile dans la constellation.
		*\param[in] name
		*	Le nom de l'étoile.
		*\return
		*	L'étoile trouvée, \p nullptr si non trouvée
		*/
		ConstellationStar const * findStar( std::string const & name )const;
		/**
		*\brief
		*	Remplit les étoiles de cette constellation depuis celles données.
		*\param[in] stars
		*	Les étoiles de la carte du ciel.
		*/
		void fill( StarSet & stars );
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
		*	La liste des étoiles composant cette constellation.
		*/
		inline ConstellationStarArray const & stars()const noexcept
		{
			return m_stars;
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
		//! Le nom de la constellation.
		std::string m_name;
		//! Les étoiles composant la constellation.
		ConstellationStarArray m_stars;
		//! Les liens entre les étoiles, formant la constellation.
		LinkArray m_links;
		//! La position de la constellation, fonction des étoiles qui la composent.
		gl::Vector3D m_position;
	};
}

#endif
