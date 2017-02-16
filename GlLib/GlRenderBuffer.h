/**
*\file
*	GlRenderBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlLib_RenderBuffer_HPP___
#define ___GlLib_RenderBuffer_HPP___
#pragma once

#include "GlLibPrerequisites.h"

namespace gl
{
	/**
	*\brief
	*	Gère un tampon de rendu GPU.
	*	Voir \ref gllib_framebuffer.
	*/
	class RenderBuffer
	{
		RenderBuffer( RenderBuffer const & ) = delete;
		RenderBuffer & operator=( RenderBuffer const & ) = delete;
		RenderBuffer( RenderBuffer && ) = delete;
		RenderBuffer & operator=( RenderBuffer && ) = delete;

	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] format
		*	Le format des pixels.
		*\param[in] size
		*	Les dimensions voulues pour le tampon.
		*/
		RenderBuffer( PixelFormat format
			, Size2D const & size )noexcept;
		/**
		*\brief
		*	Destructeur.
		*/
		~RenderBuffer()noexcept;
		/**
		*\return
		*	Le nom OpenGL du tampon.
		*/
		uint32_t glName()const noexcept
		{
			return m_name;
		}
		/**
		*\return
		*	Les dimensions de la texture.
		*/
		inline Size2D const & dimensions()const noexcept
		{
			return m_size;
		}

	private:
		//! Le nom OpenGL du tampn.
		uint32_t m_name{ GL_INVALID_INDEX };
		//! Le format des pixels du tampon.
		uint32_t m_format;
		//! Les dimensions du tampon.
		Size2D m_size;

	};
}

#endif
