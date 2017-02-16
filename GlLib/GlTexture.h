/**
*\file
*	GlTexture.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlLib_Texture_HPP___
#define ___GlLib_Texture_HPP___
#pragma once

#include "GlLibPrerequisites.h"

namespace gl
{
	/**
	*\brief
	*	Gère une texture sur le GPU.
	*	Voir \ref gllib_texture.
	*/
	class Texture
	{
		Texture( Texture const & ) = delete;
		Texture & operator=( Texture const & ) = delete;
		Texture( Texture && ) = delete;
		Texture & operator=( Texture && ) = delete;

	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] format
		*	Le format des pixels.
		*\param[in] size
		*	Les dimensions voulues pour la texture.
		*/
		Texture( PixelFormat format
			, Size2D const & size )noexcept;
		/**
		*\brief
		*	Constructeur.
		*\param[in] format
		*	Le format des pixels.
		*\param[in] size
		*	Les dimensions voulues pour la texture.
		*\param[in] data
		*	Les données de la texture.
		*/
		Texture( PixelFormat format
			, Size2D const & size
			, ByteArray const & data )noexcept;
		/**
		*\brief
		*	Destructeur.
		*/
		~Texture()noexcept;
		/**
		*\brief
		*	Active la texture sur l'unité voulue pour le prochain draw call.
		*\param[in] unit
		*	L'unité de texture voulue.
		*/
		void bind( uint32_t unit )const noexcept;
		/**
		*\brief
		*	Désactive la texture pour l'unité voulue.
		*\param[in] unit
		*	L'unité de texture voulue.
		*/
		void unbind( uint32_t unit )const noexcept;
		/**
		*\brief
		*	Génère les mipmaps de la texture.
		*\remarks
		*	Pour utiliser cette fonction, il faut que la texture soit activée.
		*/
		void generateMipmaps()const noexcept;
		/**
		*\brief
		*	Copie des données dans la VRAM.
		*\remarks
		*	Pour utiliser cette fonction, il faut que la texture soit activée.
		*\param[in] xoffset, yoffset
		*	Le décalage à partir duquel les données seront copiées, par rapport
		*	au début du stockage de la texture, en VRAM.
		*\param[in] width, height
		*	Les dimensions des données à copier.
		*\param[in] data
		*	Les données à copier.
		*/
		void upload( uint32_t xoffset
			, uint32_t yoffset
			, uint32_t width
			, uint32_t height
			, uint8_t const * data )const noexcept;
		/**
		*\return
		*	Le nom OpenGL de la texture.
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
		//! Le nom OpenGL de la texture.
		uint32_t m_name{ GL_INVALID_INDEX };
		//! Le type des composantes des pixels.
		uint32_t m_type;
		//! Le format des pixels
		uint32_t m_format;
		//! Les dimensions de la texture.
		Size2D m_size;
		//! Debug: dit si le tampon est activé.
		GlLib_DebugBool( m_bound, false );
	};
}

#endif
