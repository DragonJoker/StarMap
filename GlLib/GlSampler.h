/**
*\file
*	GlSampler.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlLib_Sampler_HPP___
#define ___GlLib_Sampler_HPP___
#pragma once

#include "GlLibPrerequisites.h"

namespace gl
{
	/**
	*\brief
	*	Gère un échantillonneur sur le GPU.
	*	Voir \ref gllib_texture.
	*/
	class Sampler
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] wrapS, wrapT
		*	Les modes de wrap de texture.
		*\param[in] minFilter, magFilter
		*	Les filtres de minification et magnification.
		*/
		Sampler( WrapMode wrapS
			, WrapMode wrapT
			, MinFilter minFilter
			, MagFilter magFilter )noexcept;
		/**
		*\brief
		*	Destructeur.
		*/
		virtual ~Sampler()noexcept;
		/**
		*\brief
		*	Active l'échantillonneur sur l'unité voulue pour le prochain draw call.
		*\param[in] unit
		*	L'unité de texture voulue.
		*/
		virtual void bind( uint32_t unit )const noexcept;
		/**
		*\brief
		*	Désactive l'échantillonneur pour l'unité voulue.
		*\param[in] unit
		*	L'unité de texture voulue.
		*/
		virtual void unbind( uint32_t unit )const noexcept;

	private:
		//! Le mode de wrapping sur S (U).
		uint32_t m_wrapS;
		//! Le mode de wrapping sur T (V).
		uint32_t m_wrapT;
		//! Le filtre de minification.
		uint32_t m_minFilter;
		//! Le filtre de magnification.
		uint32_t m_magFilter;
	};
	/**
	*\brief
	*	Gère un échantillonneur sur le GPU.
	*	Voir \ref gllib_texture.
	*/
	class SamplerObject
		: public Sampler
	{
		SamplerObject( SamplerObject const & ) = delete;
		SamplerObject & operator=( SamplerObject const & ) = delete;
		SamplerObject( SamplerObject && ) = delete;
		SamplerObject & operator=( SamplerObject && ) = delete;

	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] wrapS, wrapT
		*	Les modes de wrap de texture.
		*\param[in] minFilter, magFilter
		*	Les filtres de minification et magnification.
		*/
		SamplerObject( WrapMode wrapS
			, WrapMode wrapT
			, MinFilter minFilter
			, MagFilter magFilter )noexcept;
		/**
		*\brief
		*	Destructeur.
		*/
		~SamplerObject()noexcept;
		/**
		*\brief
		*	Active l'échantillonneur sur l'unité voulue pour le prochain draw call.
		*\param[in] unit
		*	L'unité de texture voulue.
		*/
		void bind( uint32_t unit )const noexcept override;
		/**
		*\brief
		*	Désactive l'échantillonneur pour l'unité voulue.
		*\param[in] unit
		*	L'unité de texture voulue.
		*/
		void unbind( uint32_t unit )const noexcept override;

	private:
		//! Le nom OpenGL de l'objet
		uint32_t m_name{ GL_INVALID_INDEX };
	};
}

#endif
