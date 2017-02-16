/**
*\file
*	Picking.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_Picking_HPP___
#define ___RenderLib_Picking_HPP___
#pragma once

#include "PickingRenderer.h"
#include "RenderSignal.h"

#include <GlLib/GlFrameBuffer.h>
#include <GlLib/GlRenderBuffer.h>
#include <GlLib/GlTexture.h>

namespace render
{
	/**
	*\brief
	*	Passe de picking, utilisant les FBO.
	*/
	class Picking
	{
	public:
		/**
		*\brief
		*	Types d'éléments sélectionnables.
		*/
		enum class NodeType
		{
			//! Aucun élément sélectionné.
			eNone,
			//! Un objet complexe a été sélectionné.
			eObject,
			//! Un billboard a été sélectionné.
			eBillboard,
		};
		/**
		*\brief
		*	Structure d'un pixel.
		*/
		struct Pixel
		{
			//! Composante rouge.
			uint8_t r{ 0u };
			//! Composante verte.
			uint8_t g{ 0u };
			//! Composante bleue.
			uint8_t b{ 0u };
			//! Composante alpha.
			uint8_t a{ 0u };
		};
		//! Un tableau de pixels.
		using PixelArray = std::vector< Pixel >;

	public:
		/**
		 *\brief
		 *	Constructeur.
		*\param[in] size
		*	La taille voulue pour le FBO.
		 */
		Picking( gl::Size2D const & size );
		/**
		*\brief
		*	Destructeur.
		*/
		~Picking();
		/**
		*\brief
		*	Sélectionne la géométrie à la position de souris donnée.
		*\param[in] position
		*	La position de la souris.
		*\param[in] camera
			La caméra.
		*\param[in] zoomPercent
			Le pourcentage du zoom actuel.
		*\param[in] objects
			Les objets à dessiner.
		*\param[in] billboards
			Les billboards à dessiner.
		*\return
		*	Picking::NodeType::eNone si rien n'a été pické.
		*/
		NodeType pick( gl::Position2D const & position
			, Camera const & camera
			, float zoomPercent
			, RenderSubmeshArray const & objects
			, RenderBillboardArray const & billboards )const;

	public:
		//! Le signal émis lorsque l'on sélectionne un objet.
		mutable OnObjectPicked onObjectPicked;
		//! Le signal émis lorsque l'on sélectionne un billboard.
		mutable OnBillboardPicked onBillboardPicked;
		//! Le signal émis lorsque rien n'est sélectionné.
		mutable OnUnpick onUnpick;

	private:
		/**
		*\brief
		*	Exécute le picking FBO.
		*\param[in] position
		*	La position de la souris.
		*\param[in] camera
			La caméra.
		*\param[in] zoomPercent
			Le pourcentage du zoom actuel.
		*\param[in] objects
			Les objets à dessiner.
		*\param[in] billboards
			Les billboards à dessiner.
		*\return
		*	La couleur de la sélection.
		*/
		Pixel doFboPick( gl::Position2D const & position
			, Camera const & camera
			, float zoomPercent
			, RenderSubmeshArray const & objects
			, RenderBillboardArray const & billboards )const;
		/**
		*\brief
		*	Sélectionne l'objet correspondant à la couleur donnée.
		*\param[in] pixel
		*	La couleur sélectionnée.
		*\param[in] objects
			Les objets dessinés.
		*\param[in] billboards
			Les billboards dessinés.
		*\return
		*	Picking::NodeType::eNone si rien n'a été pické.
		*/
		Picking::NodeType doPick( Pixel const & pixel
			, RenderSubmeshArray const & objects
			, RenderBillboardArray const & billboards )const;

	private:
		//! Le renderer.
		PickingRenderer m_renderer;
		//! Les dimensions de l'image.
		gl::Size2D m_size;
		//! La texture recevant le rendu couleur.
		gl::TexturePtr m_colour;
		//! Le tampon recevant le rendu profondeur.
		gl::RenderBufferPtr m_depth;
		//! Le tampon d'image.
		gl::FrameBufferPtr m_fbo;
		//! Le tampon dans lequel on va recevoir l'image.
		mutable PixelArray m_buffer;
	};
}

#endif
