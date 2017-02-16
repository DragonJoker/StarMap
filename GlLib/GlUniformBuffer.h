/**
*\file
*	UniformBuffer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlLib_UniformBuffer_HPP___
#define ___GlLib_UniformBuffer_HPP___
#pragma once

#include "GlArrayUniform.h"
#include "GlBuffer.h"
#include "GlUniform.h"

namespace gl
{
	/**
	*\brief
	*	Tampon de variables uniformes.
	*\remark
	*	Il stocke toutes les variables contenues dans un programme shader.
	*	<br />Il est aussi responsable de la création des variables.
	*	<br />Utilise les Uniform Buffer Objects si supportés.
	*/
	class UniformBuffer
	{
	public:
		UniformBuffer( UniformBuffer const & ) = delete;
		UniformBuffer( UniformBuffer && ) = default;
		UniformBuffer & operator=( UniformBuffer const & ) = delete;
		UniformBuffer & operator=( UniformBuffer && ) = default;
		/**
		*\brief
		*	Constructeur.
		*\param[in] name
		*	Le nom du tampon.
		*\param[in] point
		*	Le point d'attache du tampon.
		*\param[in] program
		*	Le programme auquel est lié le tampon.
		*/
		UniformBuffer( std::string const & name
			, uint32_t point
			, ShaderProgram const & program );
		/**
		*\brief
		*	Destructeur.
		*/
		~UniformBuffer();
		/**
		*\brief
		*	Initialise toutes les variables et le tampon GPU associé.
		*/
		void initialise();
		/**
		*\brief
		*	Nettoie toutes les variables et le tampon GPU associé.
		*/
		void cleanup();
		/**
		*\brief
		*	Active le stockage GPU.
		*\param[in] index
		*	Le point d'attache.
		*/
		void bind( uint32_t index )const noexcept;
		/**
		*\brief
		*	Crée une variable.
		*\param[in] name
		*	Le nom de la variable.
		*\return
		*	La variable créée, nullptr en cas d'échec.
		*/
		template< typename Type >
		inline Uniform< Type > & createUniform
			( std::string const & name );
		/**
		*\brief
		*	Crée une variable tableau.
		*\param[in] name
		*	Le nom de la variable.
		*\param[in] occurences
		*	Les dimensions du tableau.
		*\return
		*	La variable créée, nullptr en cas d'échec.
		*/
		template< typename Type >
		inline ArrayUniform< Type > & createUniform
			( std::string const & name
			, int occurences );
		/**
		*\brief
		*	Supprime une variable de ce tampon.
		*\param[in] name
		*	Le nom de la variable.
		*/
		void removeUniform( std::string const & name );
		/**
		*\return
		*	Le stockage GPU.
		*/
		inline Buffer< uint8_t > & storage()const
		{
			assert( m_storage );
			return *m_storage;
		}
		/**
		*\return
		*	l'itérateur sur le début de la liste de variables.
		*/
		inline auto begin()
		{
			return m_listVariables.begin();
		}
		/**
		*\return
		*	L'itérateur sur le début de la liste de variables.
		*/
		inline auto begin()const
		{
			return m_listVariables.begin();
		}
		/**
		*\return
		*	L'itérateur sur la fin de la liste de variables.
		*/
		inline auto end()
		{
			return m_listVariables.end();
		}
		/**
		*\return
		*	L'itérateur sur la fin de la liste de variables.
		*/
		inline auto end()const
		{
			return m_listVariables.end();
		}
		/**
		*\return
		*	Le nom du tampon.
		*/
		inline std::string const & name()const
		{
			return m_name;
		}

	private:
		/**
		*\brief
		*	Met à jour le stockage GPU.
		*/
		void doUpdate()const;

	public:
		struct UniformInfo
		{
			//! L'offset de la variable dans le tampon.
			uint32_t m_offset;
			//! Le stride de la variable, pour les tableaux.
			uint32_t m_stride;
			//! Le nombre d'éléments de la variable, pour les tableaux.
			uint32_t m_size;
		};

		using UniformInfoArray = std::vector< UniformInfo >;

	private:
		using UniformPtr = std::unique_ptr< UniformBase >;
		using UniformRef = std::reference_wrapper< UniformBase >;
		using UniformList = std::vector< UniformPtr >;
		using UniformMap = std::map< std::string, UniformRef >;
		//! Le programme shader.
		gl::ShaderProgram const & m_program;
		//! Le nom du tampon.
		std::string m_name;
		//! La liste de variables.
		UniformList m_listVariables;
		//! Les variables, triées par nom.
		UniformMap m_mapVariables;
		//! Le tampon de données.
		mutable ByteArray m_buffer;
		//! Le tampon GPU.
		std::unique_ptr< Buffer< uint8_t > > m_storage;
		//! Le point d'attache de l'UBO.
		uint32_t m_point;
		//! L'indice du bloc dans le shader.
		uint32_t m_blockIndex{ GL_INVALID_INDEX };
		//! Les informations des variables du tampon.
		UniformInfoArray m_infos;
		//! Dit si on utilise les UBO.
		bool m_ubo{ false };
	};
}

#include "GlUniformBuffer.inl"

#endif
