/**
*\file
*	RenderWindow.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_RenderWindow_HPP___
#define ___RenderLib_RenderWindow_HPP___
#pragma once

#include "CameraState.h"
#include "FontTexture.h"
#include "OverlayRenderer.h"
#include "RenderTarget.h"
#include "Scene.h"

#include <GlLib/GlAttribute.h>
#include <GlLib/GlShaderProgram.h>
#include <GlLib/OpenGL.h>
#include <GlLib/GlPipeline.h>

namespace render
{
	/**
	*\brief
	*	Classe de fenêtre de rendu.
	*\remarks
	*	Utilise une RenderTarget pour faire un rendu en texture,
	*	puis dessine le résultat dans la fenêtre,
	*	enfin, dessine les incrustations.
	*/
	class RenderWindow
	{
	public:
		/**
		*\brief
		*	Un sommet pour le rendu dans la fenêtre.
		*/
		struct Vertex
		{
			//! Une position en 2D.
			gl::Vector2D position;
			//! Les coordonnées de texture.
			gl::Vector2D texture;
		};

	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] dimensions
		*	Les dimensions de la texture recevant le rendu.
		*/
		RenderWindow( gl::Size2D const & dimensions );
		/**
		*\brief
		*	Met à jour la scène.
		*/
		void update();
		/**
		*\brief
		*	Met à jour les incrustations de la scène.
		*/
		void updateOverlays();
		/**
		*\brief
		*	Redimensionne le viewport.
		*\param[in] size
		*	Les dimensions du viewport.
		*/
		void resize( gl::Size2D const & size )noexcept;
		/**
		*\brief
		*	Dessine la scène.
		*\remarks
		*	Le dessin se fait dans la cible de rendu, puis les effets éventuels
		*	sont appliqués et enfin la cible de rendu est dessinée dans le
		*	tampon de la fenêtre.
		*/
		void draw()const noexcept;
		/**
		*\return
		*	La scène.
		*/
		inline Scene const & scene()const noexcept
		{
			return m_scene;
		}
		/**
		*\return
		*	La scène.
		*/
		inline Scene & scene()noexcept
		{
			return m_scene;
		}
		/**
		*\return
		*	Le viewport de la scène.
		*/
		inline Viewport const & viewport()const noexcept
		{
			return m_scene.viewport();
		}
		/**
		*\return
		*	Le viewport de la scène.
		*/
		inline Viewport & viewport()noexcept
		{
			return m_scene.viewport();
		}
		/**
		*\return
		*	L'instance de picking.
		*/
		inline Picking const & picking()const noexcept
		{
			return m_picking;
		}
		/**
		*\return
		*	L'état de la caméra.
		*/
		inline CameraState const & state()const noexcept
		{
			return m_scene.state();
		}
		/**
		*\return
		*	L'état de la caméra.
		*/
		inline CameraState & state()noexcept
		{
			return m_scene.state();
		}
		/**
		*\brief
		*	Active le picking pour la prochaine frame.
		*\param[in] position
		*	La position de la souris.
		*/
		inline void pick( gl::Position2D const & position )
		{
			m_pickPosition = position;
			m_pick = true;
		}

	private:
		//! Le pipeline de rendu dans la fenêtre.
		gl::Pipeline m_pipeline;
		//! La cible de rendu.
		RenderTarget m_target;
		//! La scène qui sera dessinée.
		Scene m_scene;
		//! Les dimensions de la fenêtre.
		gl::Size2D m_size;
		//! L'échantillonneur de la texture de la cible de rendu.
		gl::SamplerPtr m_sampler;
		//! Le programme shader utilisé pour le rendu dans la fenêtre.
		gl::ShaderProgramPtr m_program;
		//! Le tampon GPU contenant les sommets du rendu dans la fenêtre.
		gl::BufferPtr< Vertex > m_vbo;
		//! L'attribut de position.
		gl::Vec2AttributePtr m_posAttrib;
		//! L'attibut de coordonnées de texture.
		gl::Vec2AttributePtr m_texAttrib;
		//! La variable uniforme contenant la texture de la cible.
		gl::IntUniformPtr m_texUniform;
		//! Le viewport du rendu dans la fenêtre.
		Viewport m_viewport;
		//! Le renderer d'incrustations
		OverlayRendererPtr m_overlayRenderer;
		//! La position voulue pour le picking.
		gl::Position2D m_pickPosition;
		//! L'instance de picking.
		Picking m_picking;
		//! Dit si on doit exécuter le picking lors du dessin de la prochaine frame.
		mutable bool m_pick{ false };
	};
}

#endif
