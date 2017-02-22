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
		*	La position en coordonnées polaires.
		*	x => ascencion.
		*	y => déclinaison.
		*\param[in] colour
		*	La couleur de l'étoile.
		*/
		Star( std::string const & name
			, float magnitude
			, gl::Vec2 const & position
			, gl::RgbColour const & colour );
		/**
		*\brief
		*	Constructeur.
		*\param[in] name
		*	Le nom de l'étoile.
		*\param[in] magnitude
		*	La magnitude de l'étoile, permettant de la cacher, en fonction
		*	du zoom.
		*\param[in] position
		*	La position en coordonnées cartésiennes.
		*\param[in] colour
		*	La couleur de l'étoile.
		*/
		Star( std::string const & name
			, float magnitude
			, gl::Vec3 const & position
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
		*	L'index de l'étoile.
		*/
		inline uint32_t index()const noexcept
		{
			return m_index;
		}
		/**
		*\brief
		*	Définit l'index de l'étoile.
		*\param[in] value
		*	La nouvelle valeur.
		*/
		inline void index( uint32_t value )noexcept
		{
			m_index = value;
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
		inline gl::Vec3 const & position()const noexcept
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
		//! L'identifiant de l'étoile.
		size_t m_id;
		//! L'index de l'étoile dans le tableau trié par magnitude.
		uint32_t m_index;
		//! Le nom de l'étoile.
		std::string m_name;
		//! La magnitude de l'étoile.
		float m_magnitude;
		//! La position de l'étoile.
		gl::Vec3 m_position;
		//! La couleur de l'étoile.
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
