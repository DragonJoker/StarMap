/**
*\file
*	OpenGL.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlLib_OpenGL_HPP___
#define ___GlLib_OpenGL_HPP___
#pragma once

#include "GlLibPrerequisites.h"

namespace gl
{
	/**
	*\brief
	*	Enumération des niveaux de fonctionnalité d'OpenGL.
	*/
	enum class FeatureLevel
	{
		//! Non défini, par défaut OpenGL ES 2.
		eUndefined,
		//! OpenGL ES 2.
		eGLES2,
		//! OpenGL ES 3.
		eGLES3
	};
	/**
	*\brief
	*	Classe de gestion d'OpenGL.
	*\remarks
	*	Permet de récupérer la version, ainsi que le niveau de fonctionnalités
	*	de celui-ci.
	*/
	class OpenGL
	{
	public:
		/**
		*\brief
		*	Récupère la version et détermine le niveau de fonctionnalités
		*	associé.
		*/
		static void initialise();
		/**
		*\return
		*	Dit si la version d'OpenGL chargée supporte l'instanciation
		*	hardware.
		*/
		static bool hasInstancing()noexcept;
		/**
		*\return
		*	Dit si OpenGL a été initialisé.
		*/
		static inline bool isInitialised()noexcept
		{
			return m_level != FeatureLevel::eUndefined;
		}
		/**
		*\return
		*	Dit si le niveau de fonctionnalités demandé est supporté.
		*/
		static inline bool checkSupport( FeatureLevel level )noexcept
		{
			return m_level >= level;
		}
		/**
		*\return
		*	La version.
		*/
		static inline std::string const & getVersion()noexcept
		{
			return m_version;
		}
		/**
		*\brief
		*	Crée un échantillonneur.
		*\remarks
		*	En fonction du niveau de fonctionnalités, soit un gl::Sampler,
		*	soit un gl::SamplerObject sera créé.
		*/
		static SamplerPtr createSampler( WrapMode wrapS
			, WrapMode wrapT
			, MinFilter minFilter
			, MagFilter magFilter );

	private:
		//! Le niveau de fonctionnalités.
		static FeatureLevel m_level;
		//! La chaine contenant le numéro de version.
		static std::string m_version;
	};
}

#endif
