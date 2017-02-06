/**
*\file
*	Star.h
*\author
*	Sylvain Doremus
*/
#ifndef ___StarMapLib_Star_HPP___
#define ___StarMapLib_Star_HPP___

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
			, glm::vec2 const & position
			, glm::vec3 const & colour );
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
		*	La magnitude de l'étoile.
		*/
		inline float getMagnitude()const
		{
			return m_magnitude;
		}
		/**
		*\return
		*	La position de l'étoile, en coordonnées sphériques.
		*/
		inline glm::vec2 const & getPosition()const
		{
			return m_position;
		}
		/**
		*\return
		*	La couleur de l'étoile.
		*/
		inline glm::vec3 const & getColour()const
		{
			return m_colour;
		}

	private:
		std::string m_name;
		float m_magnitude;
		glm::vec2 m_position;
		glm::vec3 m_colour;
	};
}

#endif
