/**
*\file
*	GlFrameBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlLib_FrameBuffer_HPP___
#define ___GlLib_FrameBuffer_HPP___
#pragma once

#include "GlLibPrerequisites.h"

namespace gl
{
	/**
	*\brief
	*	Gère un tampon d'image GPU.
	*	Voir \ref gllib_framebuffer.
	*/
	class FrameBuffer
	{
		FrameBuffer( FrameBuffer const & ) = delete;
		FrameBuffer & operator=( FrameBuffer const & ) = delete;
		FrameBuffer( FrameBuffer && ) = delete;
		FrameBuffer & operator=( FrameBuffer && ) = delete;

	public:
		/**
		*\brief
		*	Constructeur.
		*/
		FrameBuffer()noexcept;
		/**
		*\brief
		*	Destructeur.
		*/
		~FrameBuffer()noexcept;
		/**
		*\brief
		*	Active le tampon pour le prochain draw call.
		*/
		void bind()const noexcept;
		/**
		*\brief
		*	Désactive le tampon.
		*/
		void unbind()const noexcept;
		/**
		*\brief
		*	Vide le contenu du tampon.
		*\remarks
		*	Pour utiliser cette fonction, il faut que le tampon soit activé.
		*\param[in] colour
		*	La couleur de vidage du tampon de couleur.
		*\param[in] depth
		*	La valeur de vidage du tampon de profondeur.
		*\param[in] stencil
		*	La valeur de vidage du tampon de stencil.
		*/
		void clear( RgbaColour const & colour
			, float depth = 1.0f
			, int stencil = 0 )const noexcept;
		/**
		*\brief
		*	Attache une texture au point donné.
		*\remarks
		*	Pour utiliser cette fonction, il faut que le tampon soit activé.
		*\param[in] texture
		*	La texture à attacher.
		*\param[in] point
		*	Le point d'attache.
		*/
		void attach( Texture const & texture
			, AttachmentPoint point )const noexcept;
		/**
		*\brief
		*	Détache une texture du point donné.
		*\remarks
		*	Pour utiliser cette fonction, il faut que le tampon soit activé.
		*\param[in] texture
		*	La texture à détacher.
		*\param[in] point
		*	Le point d'attache.
		*/
		void detach( Texture const & texture
			, AttachmentPoint point )const noexcept;
		/**
		*\brief
		*	Attache un tampon de rendu au point donné.
		*\remarks
		*	Pour utiliser cette fonction, il faut que le tampon soit activé.
		*\param[in] buffer
		*	Le tampon de rendu à attacher.
		*\param[in] point
		*	Le point d'attache.
		*/
		void attach( RenderBuffer const & buffer
			, AttachmentPoint point )const noexcept;
		/**
		*\brief
		*	Détache un tampon de rendu du point donné.
		*\remarks
		*	Pour utiliser cette fonction, il faut que le tampon soit activé.
		*\param[in] buffer
		*	Le tampon de rendu à détacher.
		*\param[in] point
		*	Le point d'attache.
		*/
		void detach( RenderBuffer const & buffer
			, AttachmentPoint point )const noexcept;
		/**
		*\brief
		*	Copie des données dans la RAM.
		*\remarks
		*	Pour utiliser cette fonction, il faut que le tampon soit activé.
		*\param[in] xoffset, yoffset
		*	Le décalage à partir duquel les données seront copiées, par rapport
		*	au début du stockage de la texture, en VRAM.
		*\param[in] width, height
		*	Les dimensions des données à copier.
		*\param[in] format
		*	Le format voulu pour les données.
		*\param[out] data
		*	Reçoit les données copiées.
		*/
		void download( uint32_t xoffset
			, uint32_t yoffset
			, uint32_t width
			, uint32_t height
			, PixelFormat format
			, uint8_t * data )const noexcept;

	private:
		//! Le nom OpenGL du frame buffer.
		uint32_t m_name{ GL_INVALID_INDEX };
		//! Les dimensions du frame buffer, déterminées au premier objet attaché.
		mutable gl::Size2D m_size;
		//! Debug: dit si le tampon est activé.
		GlLib_DebugBool( m_bound, false );
	};
}

#endif
