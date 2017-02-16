/**
*\file
*	GlBufferBase.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlLib_BufferBase_HPP___
#define ___GlLib_BufferBase_HPP___
#pragma once

#include "GlLibPrerequisites.h"

namespace gl
{
	/**
	*\brief
	*	Classe de base d'un tampon GPU.
	*	Voir \ref gllib_buffer.
	*/
	class BufferBase
	{
	protected:
		BufferBase( BufferBase const & ) = delete;
		BufferBase & operator=( BufferBase const & ) = delete;
		BufferBase( BufferBase && ) = delete;
		BufferBase & operator=( BufferBase && ) = delete;
		/**
		*\brief
		*	Constructeur.
		*\param[in] target
		*	Le type de cible tu tampon.
		*\param[in] data
		*	Les données.
		*\param[in] size
		*	La taille des données.
		*/
		BufferBase( BufferTarget target
			, void const * data
			, size_t size )noexcept;
		/**
		*\brief
		*	Destructeur.
		*/
		~BufferBase()noexcept;

	public:
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
		*	Définit le point d'attache du tampon.
		*/
		void bindingPoint( uint32_t index )const noexcept;
		/**
		*\brief
		*	Réinitialise la taille du tampon en VRAM.
		*\remarks
		*	Pour utiliser cette fonction, il faut que le tampon soit activé.
		*\param[in] size
		*	La nouvelle taille.
		*/
		void resize( uint32_t size )noexcept;
		/**
		*\brief
		*	Copie des données dans la VRAM.
		*\remarks
		*	Pour utiliser cette fonction, il faut que le tampon soit activé.
		*\param[in] offset
		*	Le décalage à partir duquel les données seront copiées, par rapport
		*	au début du stockage du tampon, en VRAM.
		*\param[in] size
		*	La taille des données à copier.
		*\param[in] data
		*	Les données à copier.
		*/
		void upload( uint32_t offset
			, uint32_t size
			, void const * data )const noexcept;
		/**
		*\brief
		*	Copie des données dans la RAM.
		*\remarks
		*	Pour utiliser cette fonction, il faut que le tampon soit activé.
		*\param[in] offset
		*	Le décalage à partir duquel les données seront copiées, par rapport
		*	au début du stockage du tampon, en VRAM.
		*\param[in] size
		*	La taille des données à copier.
		*\param[in] data
		*	Les données à copier.
		*/
		void download( uint32_t offset
			, uint32_t size
			, void * data )const noexcept;
		/**
		*\brief
		*	Mappe les données du tampon en RAM.
		*\remarks
		*	Pour utiliser cette fonction, il faut que le tampon soit activé.
		*	Si le retour de cette fonction n'est pas nullptr, il faut appeler
		*	unlock().
		*\param[in] offset
		*	Le décalage à partir duquel les données seront mappées, par rapport
		*	au début du stockage du tampon, en VRAM.
		*\param[in] size
		*	La taille des données à mapper.
		*\param[in] access
		*	Le type d'accès (combinaison de GL_MAP_READ_BIT, GL_MAP_WRITE_BIT,
		*	GL_MAP_INVALIDATE_RANGE_BIT, GL_MAP_INVALIDATE_BUFFER_BIT,
		*	GL_MAP_FLUSH_EXPLICIT_BIT GL_MAP_UNSYNCHRONIZED_BIT.
		*\return
		*	Le pointeur sur les données mappées en RAM, nullptr en cas d'échec.
		*/
		void * lock( uint32_t offset
			, uint32_t size
			, uint32_t access )const noexcept;
		/**
		*\brief
		*	Démappe les données du tampon de la RAM.
		*\remarks
		*	Pour utiliser cette fonction, il faut que le tampon ait été mappé
		*	avec lock().
		*/
		void unlock()const noexcept;
		/**
		*\return
		*	La taille du tampon.
		*/
		inline uint32_t size()const noexcept
		{
			return m_size;
		}

	private:
		//! Le nom OpenGL du tampon.
		uint32_t m_name{ GL_INVALID_INDEX };
		//! La cible du tampon.
		uint32_t m_target;
		//! La taille du tampon, en VRAM.
		uint32_t m_size;
		//! Debug: dit si le tampon est activé.
		GlLib_DebugBool( m_bound, false );
	};
}

#endif
