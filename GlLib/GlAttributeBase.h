/**
*\file
*	GlAttributeBase.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlLib_AttributeBase_HPP___
#define ___GlLib_AttributeBase_HPP___
#pragma once

#include "GlLibPrerequisites.h"

namespace gl
{
	/**
	*\brief
	*	Gère un attribut de sommet lié à un tampon GPU.
	*	Voir \ref gllib_attribute.
	*/
	class AttributeBase
	{
	protected:
		/**
		*\brief
		*	Constructeur.
		*\param[in] location
		*	La position de l'attribut dans le programme.
		*\param[in] stride
		*	Le nombre d'octets entre chaque attribut.
		*\param[in] offset
		*	Le décalage des données de l'attribut.
		*\param[in] divisor
		*	Le pas entre les attributs, pour les données instanciées.
		*\param[in] size
		*	Le nombre de composantes de l'attribut.
		*\param[in] type
		*	Le type des composantes de l'attribut.
		*/
		AttributeBase( uint32_t location
			, uint32_t stride
			, uint32_t offset
			, uint32_t divisor
			, int size
			, AttributeType type )noexcept;

	public:
		/**
		*\brief
		*	Destructeur.
		*/
		virtual ~AttributeBase()noexcept;
		/**
		*\brief
		*	Active l'attribut pour le prochain draw call.
		*\param[in] buffer
		*	Le tampon contenant les données de l'attribut.
		*\param[in] offset
		*	L'offset à partir duquel l'attibut est activé.
		*/
		void bind( void const * buffer
			, uint32_t offset )const noexcept;
		/**
		*\brief
		*	Active l'attribut pour le prochain draw call.
		*\remarks
		*	Prend en compte le diviseur donné au constructeur.
		*\param[in] buffer
		*	Le tampon contenant les données de l'attribut.
		*\param[in] offset
		*	L'offset à partir duquel l'attibut est activé.
		*/
		void bindInstanced( void const * buffer
			, uint32_t offset )const noexcept;
		/**
		*\brief
		*	Active l'attribut pour le prochain draw call.
		*\remarks
		*	Il faut qu'un tampon GPU ait été activé auparavant.
		*\param[in] offset
		*	L'offset à partir duquel l'attibut est activé.
		*/
		inline void bind( uint32_t offset )const noexcept
		{
			bind( nullptr, offset );
		}
		/**
		*\brief
		*	Active l'attribut pour le prochain draw call.
		*\param[in] buffer
		*	Le tampon contenant les données de l'attribut.
		*/
		void bind( void const * buffer )const noexcept
		{
			bind( buffer, 0u );
		}
		/**
		*\brief
		*	Active l'attribut pour le prochain draw call.
		*\remarks
		*	Il faut qu'un tampon GPU ait été activé auparavant.
		*/
		inline void bind()const noexcept
		{
			bind( nullptr, 0u );
		}
		/**
		*\brief
		*	Active l'attribut pour le prochain draw call.
		*\remarks
		*	Il faut qu'un tampon GPU ait été activé auparavant.\n
		*	Prend en compte le diviseur donné au constructeur.
		*\param[in] offset
		*	L'offset à partir duquel l'attibut est activé.
		*/
		inline void bindInstanced( uint32_t offset )const noexcept
		{
			bindInstanced( nullptr, offset );
		}
		/**
		*\brief
		*	Active l'attribut pour le prochain draw call.
		*\remarks
		*	Prend en compte le diviseur donné au constructeur.
		*\param[in] buffer
		*	Le tampon contenant les données de l'attribut.
		*/
		void bindInstanced( void const * buffer )const noexcept
		{
			bindInstanced( buffer, 0u );
		}
		/**
		*\brief
		*	Active l'attribut pour le prochain draw call.
		*\remarks
		*	Il faut qu'un tampon GPU ait été activé auparavant.\n
		*	Prend en compte le diviseur donné au constructeur.
		*/
		inline void bindInstanced()const noexcept
		{
			bindInstanced( nullptr, 0u );
		}
		/**
		*\brief
		*	Désactive l'attribut.
		*/
		void unbind()const noexcept;
		/**
		*\brief
		*	Désactive l'attribut.
		*/
		void unbindInstanced()const noexcept;
		/**
		*\return
		*	\p false si l'attribut n'est pas défini.
		*/
		inline bool valid()const noexcept
		{
			return m_location != GL_INVALID_INDEX;
		}

	private:
		//! La position de l'attribut dans le programme.
		uint32_t m_location;
		//! Le nombre d'octets entre chaque attribut.
		uint32_t m_stride;
		//! Le décalage des données de l'attribut.
		uint32_t m_offset;
		//! Le pas entre les attributs, pour les données instanciées.
		uint32_t m_divisor;
		//! Le nombre de composantes de l'attribut.
		int m_size;
		//! Le type des composantes de l'attribut.
		uint32_t m_type;
	};
}

#endif
