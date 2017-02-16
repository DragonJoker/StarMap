/**
*\file
*	GlBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlLib_Buffer_HPP___
#define ___GlLib_Buffer_HPP___
#pragma once

#include "GlBufferBase.h"

namespace gl
{
	/**
	*\brief
	*	Gère un tampon GPU.
	*	Voir \ref gllib_buffer.
	*/
	template< typename T >
	class Buffer
		: public BufferBase
	{
		Buffer( Buffer< T > const & ) = delete;
		BufferBase & operator=( Buffer< T > const & ) = delete;
		Buffer( Buffer< T > && ) = delete;
		Buffer & operator=( Buffer< T > && ) = delete;

	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] target
		*	Le type de cible tu tampon.
		*\param[in] data
		*	Les données.
		*\param[in] count
		*	Le nombre d'éléments dans \p data.
		*/
		inline Buffer( BufferTarget target
			, T const * data
			, size_t count )noexcept
			: BufferBase{ target, data, uint32_t( count * m_elemSize ) }
		{
		}
		/**
		*\brief
		*	Constructeur.
		*\param[in] target
		*	Le type de cible tu tampon.
		*\param[in] data
		*	Les données du tampon.
		*/
		inline Buffer( BufferTarget target
			, std::vector< T > const & data )noexcept
			: Buffer{ target, data.data(), data.size() }
		{
		}
		/**
		*\brief
		*	Constructeur.
		*\param[in] target
		*	Le type de cible tu tampon.
		*\param[in] begin
		*	Le début des données du tampon.
		*\param[in] end
		*	La fin des données du tampon.
		*/
		inline Buffer( BufferTarget target
			, T const * begin
			, T const * end )noexcept
			: Buffer{ target, begin, size_t( end - begin ) }
		{
		}
		/**
		*\brief
		*	Constructeur n'initialisant pas la taille du tampon en VRAM.
		*\remarks
		*	Le tampon n'ayant pas de taille définie, il faut impérativement
		*	appeler Buffer::resize, puis Buffer::upload pour lui attribuer
		*	des données.
		*\param[in] target
		*	Le type de cible tu tampon.
		*/
		inline Buffer( BufferTarget target )noexcept
			: Buffer{ target, nullptr, uint32_t( 0u ) }
		{
		}
		/**
		*\brief
		*	Destructeur.
		*/
		inline ~Buffer()noexcept
		{
		}
		/**
		*\brief
		*	Réinitialise la taille du tampon en VRAM.
		*\remarks
		*	Pour utiliser cette fonction, il faut que le tampon soit activé.
		*\param[in] count
		*	Le nouveau nombre d'éléments.
		*/
		inline void resize( uint32_t count )noexcept
		{
			BufferBase::resize( count * m_elemSize );
		}
		/**
		*\brief
		*	Copie des données dans la VRAM.
		*\remarks
		*	Pour utiliser cette fonction, il faut que le tampon soit activé.
		*\param[in] offset
		*	Le décalage à partir duquel les données seront copiées, par rapport
		*	au début du stockage du tampon, en VRAM.
		*\param[in] count
		*	Le nombre d'éléments à copier.
		*\param[in] data
		*	Les données à copier.
		*/
		inline void upload( uint32_t offset
			, uint32_t count
			, T const * data )const noexcept
		{
			BufferBase::upload( offset * m_elemSize, count * m_elemSize, data );
		}
		/**
		*\brief
		*	Copie des données dans la RAM.
		*\remarks
		*	Pour utiliser cette fonction, il faut que le tampon soit activé.
		*\param[in] offset
		*	Le décalage à partir duquel les données seront copiées, par rapport
		*	au début du stockage du tampon, en VRAM.
		*\param[in] count
		*	Le nombre d'éléments à copier.
		*\param[in] data
		*	Les données à copier.
		*/
		inline void download( uint32_t offset
			, uint32_t count
			, T * data )const noexcept
		{
			BufferBase::download( offset * m_elemSize, count * m_elemSize, data );
		}
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
		*\param[in] count
		*	Le nombre d'éléments à mapper.
		*\param[in] access
		*	Le type d'accès (combinaison de GL_MAP_READ_BIT, GL_MAP_WRITE_BIT,
		*	GL_MAP_INVALIDATE_RANGE_BIT, GL_MAP_INVALIDATE_BUFFER_BIT,
		*	GL_MAP_FLUSH_EXPLICIT_BIT GL_MAP_UNSYNCHRONIZED_BIT.
		*\return
		*	Le pointeur sur les données mappées en RAM, nullptr en cas d'échec.
		*/
		inline T * lock( uint32_t offset
			, uint32_t count
			, uint32_t access )const noexcept
		{
			return BufferBase::lock( offset * m_elemSize, count * m_elemSize, access );
		}
		/**
		*\return
		*	La nombre d'éléments du tampon.
		*/
		inline uint32_t count()const noexcept
		{
			return BufferBase::size() / m_elemSize;
		}

	public:
		using BufferBase::bind;
		using BufferBase::unbind;
		using BufferBase::unlock;
		using BufferBase::bindingPoint;

	private:
		static constexpr uint32_t m_elemSize{ uint32_t( sizeof( T ) ) };
	};
	/**
	*\brief
	*	Fonction d'aide à la création d'un Buffer.
	*\remarks
	*	Initialise les données du tampon avec celles données.
	*\param[in] target
	*	La cible du tampon.
	*\param[in] data
	*	Les données du tampon.
	*\return
	*	Le tampon créé.
	*/
	template< typename T >
	std::unique_ptr< Buffer< T > > makeBuffer( BufferTarget target
		, std::vector< T > const & data )
	{
		return std::make_unique< Buffer< T > >( target, data );
	}
	/**
	*\brief
	*	Fonction d'aide à la création d'un Buffer.
	*\remarks
	*	Le tampon n'ayant pas de taille définie, il faut impérativement
	*	appeler Buffer::resize, puis Buffer::upload pour lui attribuer
	*	des données.
	*\param[in] target
	*	La cible du tampon.
	*\return
	*	Le tampon créé.
	*/
	template< typename T >
	std::unique_ptr< Buffer< T > > makeBuffer( BufferTarget target )
	{
		return std::make_unique< Buffer< T > >( target );
	}
}

#endif
