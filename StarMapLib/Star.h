/**
*\file
*	Star.h
*\author
*	Sylvain Doremus
*/
#ifndef ___StarMapLib_Star_HPP___
#define ___StarMapLib_Star_HPP___
#pragma once

#include "StarMapLibPrerequisites.h"

namespace starmap
{
	/**
	*\brief
	*	Une étoile, avec tout le nécessaire permettant de l'afficher.
	*/
	class Star
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] name
		*	Le nom de l'étoile.
		*\param[in] magnitude
		*	La magnitude de l'étoile, permettant de la cacher, en fonction
		*	du zoom.
		*\param[in] position
		*	La position en coordonnées sphériques.
		*\param[in] colour
		*	La couleur de l'étoile.
		*/
		Star( std::string const & name
			, float magnitude
			, gl::Vector2D const & position
			, gl::RgbColour const & colour );
		/**
		*\return
		*	L'identifiant de l'étoile.
		*/
		inline size_t id()const noexcept
		{
			return m_id;
		}
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
		*	La magnitude de l'étoile.
		*/
		inline float magnitude()const noexcept
		{
			return m_magnitude;
		}
		/**
		*\return
		*	La position de l'étoile, en coordonnées cartésiennes.
		*/
		inline gl::Vector3D const & position()const noexcept
		{
			return m_position;
		}
		/**
		*\return
		*	La couleur de l'étoile.
		*/
		inline gl::RgbColour const & colour()const noexcept
		{
			return m_colour;
		}

	private:
		size_t m_id;
		std::string m_name;
		float m_magnitude;
		gl::Vector3D m_position;
		gl::RgbColour m_colour;
	};
	/**
	*\brief
	*	Opérateur de comparaison "inférieur à".
	*\param[in] lhs, rhs
	*	Les étoiles à comparer.
	*\return
	*	\p true si la magnitude de lhs est inférieure à la magnitude de rhs.
	*/
	inline bool operator<( Star const & lhs, Star const & rhs )
	{
		return lhs.magnitude() < rhs.magnitude();
	}
	/**
	*\brief
	*	Opérateur de comparaison "supérieur à".
	*\param[in] lhs, rhs
	*	Les étoiles à comparer.
	*\return
	*	\p true si la magnitude de lhs est supérieure à la magnitude de rhs.
	*/
	inline bool operator>( Star const & lhs, Star const & rhs )
	{
		return lhs.magnitude() > rhs.magnitude();
	}
}

#endif
