/**
*\file
*	GlShaderProgram.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlLib_ShaderProgram_HPP___
#define ___GlLib_ShaderProgram_HPP___
#pragma once

#include "GlAttribute.h"

namespace gl
{
	/**
	*\brief
	*	Détient les informations de base d'un attribut de sommet.
	*/
	struct AttributeInfo
	{
		//! Le nom de l'attribut.
		std::string m_name;
		//! Le type de données de l'attribut.
		AttributeFullType m_type;
	};
	//! Un tableau d'informations sur les attributs.
	using AttributeInfoArray = std::vector< AttributeInfo >;
	/**
	*\brief
	*	Gère un programme shader GLSL.
	*	Voir \ref gllib_shader.
	*/
	class ShaderProgram
	{
		ShaderProgram( ShaderProgram const & ) = delete;
		ShaderProgram & operator=( ShaderProgram const & ) = delete;
		ShaderProgram( ShaderProgram && ) = delete;
		ShaderProgram & operator=( ShaderProgram && ) = delete;

	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] vtx
		*	Le source du vertex shader.
		*\param[in] pxl
		*	Le source du fragment shader.
		*/
		ShaderProgram( std::string const & vtx
			, std::string const & pxl )noexcept;
		/**
		*\brief
		*	Destructeur.
		*/
		~ShaderProgram()noexcept;
		/**
		*\brief
		*	Active le shader pour le prochain draw call.
		*/
		void bind()const noexcept;
		/**
		*\brief
		*	Désactive le shader.
		*/
		void unbind()const noexcept;
		/**
		*\brief
		*	Récupère la position d'une variable uniforme dans le programme.
		*/
		uint32_t uniformLocation( std::string const & name )const noexcept;
		/**
		*\brief
		*	Récupère l'indice du bloc de variables uniformes.
		*/
		uint32_t uniformBlockIndex( std::string const & name )const noexcept;
		/**
		*\brief
		*	Récupère les indices des variables uniformes données.
		*/
		UInt32Array uniformIndices( StringArray const & names )const noexcept;
		/**
		*\brief
		*	Récupère la valeur d'une information de la variable uniforme désignée par \p index.
		*/
		int activeUniforms( uint32_t index, uint32_t name )const noexcept;
		/**
		*\brief
		*	Récupère la valeur d'une information d'un bloc d'uniformes désigné par \p index.
		*/
		int activeUniformBlock( uint32_t index, uint32_t name )const noexcept;
		/**
		*\brief
		*	Définit le point d'attache de l'UBO donné.
		*/
		void uniformBlockBinding( uint32_t index, uint32_t point )const noexcept;
		/**
		*\brief
		*	Crée un attribut défini dans le programme.
		*\remarks
		*	Si le nom de l'attribut n'est pas dans la liste d'attributs du
		*	programme, cette fonction renvoie \p nullptr.
		*	De même si un attribut avec le bon nom est trouvé, mais avec
		*	Un type différent de celui demandé, cette fonction renvoie
		*	\p nullptr.
		*\param[in] name
		*	Le nom de l'attribut.
		*\param[in] stride
		*	Le nombre d'octets entre chaque attribut.
		*\param[in] offset
		*	Le décalage des données de l'attribut.
		*\param[in] divisor
		*	Le pas entre les attributs, pour les données instanciées.
		*\return
		*	L'attribut créé.
		*/
		template< typename T >
		inline AttributePtr< T > createAttribute( std::string const & name
			, uint32_t stride = 0u
			, uint32_t offset = 0u
			, uint32_t divisor = 0u )const
		{
			AttributeInfo infos;
			AttributePtr< T > result;

			if ( doFindAttributeInfos( name
				, AttributeTraits< T >::full_type
				, infos ) )
			{
				result = std::make_unique < Attribute< T > >
					( doGetAttributeLocation( name )
					, stride
					, offset
					, divisor );
			}

			return result;
		}

	private:
		/**
		*\brief
		*	Récupère la position d'un attribut dans le programme.
		*/
		uint32_t doGetAttributeLocation( std::string const & name )const noexcept;
		/**
		*\brief
		*	Ajoute les informations d'un attribut au programme.
		*\param[in] attribute
		*	L'attribut.
		*/
		inline void doAddAttributeInfos( AttributeInfo const & infos )
		{
			m_attributes.push_back( infos );
		}
		/**
		*\brief
		*	Récupère les informations d'attribut correspondant aux paramètres
		*	fournis.
		*\param[in] name
		*	Le nom de l'attribut.
		*\param[in] type
		*	Le type de l'attribut.
		*\param[out] infos
		*	Reçoit les informations trouvées.
		*\return
		*	\p false si aucun attribut correspondant à la demande n'a été
		*	trouvé.
		*/
		bool doFindAttributeInfos( std::string const & name
			, AttributeFullType type
			, AttributeInfo & infos )const;

	private:
		//! Le nom OpenGL du programme.
		uint32_t m_name{ GL_INVALID_INDEX };
		//! Le vertex shader.
		std::string m_vertex;
		//! Le pixel shader.
		std::string m_pixel;
		//! Les informations sur les attributs du programme.
		AttributeInfoArray m_attributes;
	};
}

#endif
