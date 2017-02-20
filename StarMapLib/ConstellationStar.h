/**
*\file
*	ConstellationStar.h
*\author
*	Sylvain Doremus
*/
#ifndef ___StarMapLib_ConstellationStar_HPP___
#define ___StarMapLib_ConstellationStar_HPP___
#pragma once

#include "Star.h"

namespace starmap
{
	/**
	*\brief
	*	Une étoile dans une constellation.
	*/
	class ConstellationStar
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] id
		*	L'ID de l'étoile dans la constellation.
		*\param[in] letter
		*	La lettre associée à l'étoile, dans la constellation.
		*\param[in] name
		*	Le nom de l'étoile.
		*\param[in] constellation
		*	La constellation.
		*/
		ConstellationStar( uint32_t id
			, std::string const & letter
			, std::string const & name
			, Constellation const & constellation );
		/**
		*\brief
		*	Définit l'étoile.
		*\param[in] star
		*	L'étoile.
		*/
		inline void fill( Star const & star )noexcept
		{
			m_name.clear();
			m_star = &star;
		}
		/**
		*\return
		*	Le nom de l'étoile.
		*/
		inline std::string const & name()const noexcept
		{
			return m_star ? m_star->name() : m_name;
		}
		/**
		*\return
		*	L'ID de l'étoile dans la constellation.
		*/
		inline uint32_t id()const noexcept
		{
			return m_id;
		}
		/**
		*\return
		*	La lettre associée à l'étoile, dans la constellation.
		*/
		inline std::string const & letter()const noexcept
		{
			return m_letter;
		}
		/**
		*\return
		*	L'étoile.
		*/
		inline Star const & star()const noexcept
		{
			assert( m_star );
			return *m_star;
		}
		/**
		*\return
		*	L'étoile.
		*/
		inline Constellation const & constellation()const noexcept
		{
			return m_constellation;
		}

	private:
		//! L'ID de l'étoile dans la constellation.
		uint32_t m_id;
		//! La lettre associée à l'étoile, dans la constellation.
		std::string m_letter;
		//! Le nom de l'étoile.
		std::string m_name;
		//! La constellation.
		Constellation const & m_constellation;
		//! L'étoile
		Star const * m_star{ nullptr };
	};
}

#endif
