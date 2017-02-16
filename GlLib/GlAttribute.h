/**
*\file
*	GlAttribute.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlLib_Attribute_HPP___
#define ___GlLib_Attribute_HPP___
#pragma once

#include "GlAttributeBase.h"

namespace gl
{
	/**
	*\brief
	*	Structure de traits définissant les informations d'un attribut, en
	*	fonction de son type.
	*/
	template< typename T >
	struct AttributeTraits;
	/**
	*\brief
	*	Structure de traits définissant les informations d'un attribut, en
	*	fonction de son type.
	*\remarks
	*	Spécialisation pour int8_t.
	*/
	template<>
	struct AttributeTraits< int8_t >
	{
		static int constexpr size = 1;
		static AttributeType constexpr type = AttributeType::eByte;
		static AttributeFullType constexpr full_type = AttributeFullType::eByte;
	};
	/**
	*\brief
	*	Structure de traits définissant les informations d'un attribut, en
	*	fonction de son type.
	*\remarks
	*	Spécialisation pour int16_t.
	*/
	template<>
	struct AttributeTraits< int16_t >
	{
		static int constexpr size = 1;
		static AttributeType constexpr type = AttributeType::eShort;
		static AttributeFullType constexpr full_type = AttributeFullType::eShort;
	};
	/**
	*\brief
	*	Structure de traits définissant les informations d'un attribut, en
	*	fonction de son type.
	*\remarks
	*	Spécialisation pour int32_t.
	*/
	template<>
	struct AttributeTraits< int32_t >
	{
		static int constexpr size = 1;
		static AttributeType constexpr type = AttributeType::eInt;
		static AttributeFullType constexpr full_type = AttributeFullType::eInt;
	};
	/**
	*\brief
	*	Structure de traits définissant les informations d'un attribut, en
	*	fonction de son type.
	*\remarks
	*	Spécialisation pour uint8_t.
	*/
	template<>
	struct AttributeTraits< uint8_t >
	{
		static int constexpr size = 1;
		static AttributeType constexpr type = AttributeType::eUByte;
		static AttributeFullType constexpr full_type = AttributeFullType::eUByte;
	};
	/**
	*\brief
	*	Structure de traits définissant les informations d'un attribut, en
	*	fonction de son type.
	*\remarks
	*	Spécialisation pour uint16_t.
	*/
	template<>
	struct AttributeTraits< uint16_t >
	{
		static int constexpr size = 1;
		static AttributeType constexpr type = AttributeType::eUShort;
		static AttributeFullType constexpr full_type = AttributeFullType::eUShort;
	};
	/**
	*\brief
	*	Structure de traits définissant les informations d'un attribut, en
	*	fonction de son type.
	*\remarks
	*	Spécialisation pour uint32_t.
	*/
	template<>
	struct AttributeTraits< uint32_t >
	{
		static int constexpr size = 1;
		static AttributeType constexpr type = AttributeType::eUInt;
		static AttributeFullType constexpr full_type = AttributeFullType::eUInt;
	};
	/**
	*\brief
	*	Structure de traits définissant les informations d'un attribut, en
	*	fonction de son type.
	*\remarks
	*	Spécialisation pour float.
	*/
	template<>
	struct AttributeTraits< float >
	{
		static int constexpr size = 1;
		static AttributeType constexpr type = AttributeType::eFloat;
		static AttributeFullType constexpr full_type = AttributeFullType::eFloat;
	};
	/**
	*\brief
	*	Structure de traits définissant les informations d'un attribut, en
	*	fonction de son type.
	*\remarks
	*	Spécialisation pour gl::Vector2D.
	*/
	template<>
	struct AttributeTraits< gl::Vector2D >
	{
		static int constexpr size = 2;
		static AttributeType constexpr type = AttributeType::eFloat;
		static AttributeFullType constexpr full_type = AttributeFullType::eVec2;
	};
	/**
	*\brief
	*	Structure de traits définissant les informations d'un attribut, en
	*	fonction de son type.
	*\remarks
	*	Spécialisation pour gl::Vector3D.
	*/
	template<>
	struct AttributeTraits< gl::Vector3D >
	{
		static int constexpr size = 3;
		static AttributeType constexpr type = AttributeType::eFloat;
		static AttributeFullType constexpr full_type = AttributeFullType::eVec3;
	};
	/**
	*\brief
	*	Structure de traits définissant les informations d'un attribut, en
	*	fonction de son type.
	*\remarks
	*	Spécialisation pour gl::Vector4D.
	*/
	template<>
	struct AttributeTraits< gl::Vector4D >
	{
		static int constexpr size = 4;
		static AttributeType constexpr type = AttributeType::eFloat;
		static AttributeFullType constexpr full_type = AttributeFullType::eVec4;
	};
	/**
	*\brief
	*	Structure de traits définissant les informations d'un attribut, en
	*	fonction de son type.
	*\remarks
	*	Spécialisation pour glm::ivec2.
	*/
	template<>
	struct AttributeTraits< glm::ivec2 >
	{
		static int constexpr size = 2;
		static AttributeType constexpr type = AttributeType::eInt;
		static AttributeFullType constexpr full_type = AttributeFullType::eIVec2;
	};
	/**
	*\brief
	*	Structure de traits définissant les informations d'un attribut, en
	*	fonction de son type.
	*\remarks
	*	Spécialisation pour glm::ivec3.
	*/
	template<>
	struct AttributeTraits< glm::ivec3 >
	{
		static int constexpr size = 3;
		static AttributeType constexpr type = AttributeType::eInt;
		static AttributeFullType constexpr full_type = AttributeFullType::eIVec3;
	};
	/**
	*\brief
	*	Structure de traits définissant les informations d'un attribut, en
	*	fonction de son type.
	*\remarks
	*	Spécialisation pour glm::ivec4.
	*/
	template<>
	struct AttributeTraits< glm::ivec4 >
	{
		static int constexpr size = 4;
		static AttributeType constexpr type = AttributeType::eInt;
		static AttributeFullType constexpr full_type = AttributeFullType::eIVec4;
	};
	/**
	*\brief
	*	Gère un attribut de sommet lié à un tampon GPU.
	*	Voir \ref gllib_attribute.
	*/
	template< typename T >
	class Attribute
		: public AttributeBase
	{
	public:
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
		*/
		inline Attribute( uint32_t location
			, uint32_t stride
			, uint32_t offset
			, uint32_t divisor )noexcept
			: AttributeBase{ location
				, stride
				, offset
				, divisor
				, AttributeTraits< T >::size
				, AttributeTraits< T >::type }
		{
		}
		/**
		*\brief
		*	Destructeur.
		*/
		inline ~Attribute()noexcept
		{
		}
	};
}

#endif
