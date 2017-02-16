/**
*\file
*	GlPipeline.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlLib_Pipeline_HPP___
#define ___GlLib_Pipeline_HPP___
#pragma once

#include "GlLibPrerequisites.h"

namespace gl
{
	/**
	*\brief
	*	Gère les états OpenGL nécessaires pour le rendu.
	*/
	class Pipeline
	{
	public:
		/**
		*\brief
		*	Constructeur
		*\param[in] culling
		*	\p true pour activer le culling.
		*\param[in] depthTest
		*	\p true pour activer le test de profondeur.
		*\param[in] depthWrite
		*	\p true pour activer les écritures dans le tampon de profondeur.
		*\param[in] blending
		*	\p true pour activer l'alpha blending.
		*/
		Pipeline( bool culling
			, bool depthTest
			, bool depthWrite
			, bool blending )noexcept;
		/**
		*\brief
		*	Récupère la version et détermine le niveau de fonctionnalités
		*	associé.
		*/
		void apply()const noexcept;

	private:
		//! L'état de face culling.
		bool m_culling;
		//! L'état de test de profondeur.
		bool m_depthTest;
		//! L'état d'écriture dans le tampon de profondeur.
		int m_depthWrite;
		//! L'état d'alpaha blending.
		bool m_blending;
	};
}

#endif
