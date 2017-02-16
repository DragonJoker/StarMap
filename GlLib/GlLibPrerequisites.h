/**
*\file
*	GlLibPrerequisites.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlLib_GlLibPrerequisites_HPP___
#define ___GlLib_GlLibPrerequisites_HPP___
#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <exception>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <ostream>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

#include "GlDebug.h"

#define GlLib_UseUBO 1

#define GlLib_EnumBounds( MinValue )\
	eCount,\
	eMin = MinValue,\
	eMax = eCount - 1

#ifndef NDEBUG
#	define GlLib_DebugBool( name, value )\
		mutable bool name{ value }
#	define GlLib_DebugAssertTrue( name )\
		assert( name )
#	define GlLib_DebugAssertFalse( name )\
		assert( !name )
#	define GlLib_DebugSetTrue( name )\
		name = true
#	define GlLib_DebugSetFalse( name )\
		name = false
#else
#	define GlLib_DebugBool( name, value )
#	define GlLib_DebugAssertTrue( name )
#	define GlLib_DebugAssertFalse( name )
#	define GlLib_DebugSetTrue( name )
#	define GlLib_DebugSetFalse( name )
#endif

#ifndef GL_INVALID_INDEX
#	define GL_INVALID_INDEX 0xFFFFFFFF
#endif

#if defined ( __ANDROID__ )
#	if !defined( NDEBUG )
#	define glConcat1( a )\
	#a
#	define glConcat2( a, b )\
	glConcat1( a ) ", " glConcat1( b )
#	define glConcat3( a, b, c )\
	glConcat2( a , glConcat2( b , c ) )
#	define glConcat4( a, b, c, d )\
	glConcat2( a , glConcat3( b, c, d ) )
#	define glConcat5( a, b, c, d, e )\
	glConcat2( a , glConcat4( b, c, d, e ) )
#	define glConcat6( a, b, c, d, e, f )\
	glConcat2( a , glConcat5( b, c, d, e, f ) )
#	define glConcat7( a, b, c, d, e, f, g )\
	glConcat2( a , glConcat6( b, c, d, e, f, g ) )
#	define glConcat8( a, b, c, d, e, f, g, h )\
	glConcat2( a , glConcat7( b, c, d, e, f, g, h ) )
#	define glConcat9( a, b, c, d, e, f, g, h, i )\
	glConcat2( a , glConcat8( b, c, d, e, f, g, h, i ) )
#	define glConcat10( a, b, c, d, e, f, g, h, i, j )\
	glConcat2( a , glConcat9( b, c, d, e, f, g, h, i, j ) )
#	define glConcat11( a, b, c, d, e, f, g, h, i, j, k )\
	glConcat2( a , glConcat10( b, c, d, e, f, g, h, i, j, k ) )
#	define glConcat12( a, b, c, d, e, f, g, h, i, j, k, l )\
	glConcat2( a , glConcat11( b, c, d, e, f, g, h, i, j, k, l ) )
#	define glConcat13( a, b, c, d, e, f, g, h, i, j, k, l, m )\
	glConcat2( a , glConcat12( b, c, d, e, f, g, h, i, j, k, l, m ) )
#	define glConcat14( a, b, c, d, e, f, g, h, i, j, k, l, m, n )\
	glConcat2( a , glConcat13( b, c, d, e, f, g, h, i, j, k, l, m, n ) )
#	define glConcat15( a, b, c, d, e, f, g, h, i, j, k, l, m, n, o )\
	glConcat2( a , glConcat14( b, c, d, e, f, g, h, i, j, k, l, m, n ) )
#	define glNumArgs2( X, X20, X19, X18, X17, X16, X15, X14, X13, X12, X11, X10, X9, X8, X7, X6, X5, X4, X3, X2, X1, N, ... )\
	N
#	define glNumArgs( ... )\
	glNumArgs2( 0, __VA_ARGS__, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0 )
#	define glConcatN3( N, ... )\
	glConcat ## N( __VA_ARGS__ )
#	define glConcatN2( N, ... )\
	glConcatN3( N, __VA_ARGS__ )
#	define glConcatN( ... )\
	glConcatN2( glNumArgs( __VA_ARGS__ ), __VA_ARGS__ )
#	define glCheckError( function, ... )\
	function( __VA_ARGS__ );gl::Debug::checkError( __FILE__, __LINE__, #function "(" glConcatN( __VA_ARGS__ ) ")" )
#else
#	define glCheckError( function, ... ) function( __VA_ARGS__ )
#endif
#elif defined( DESKTOP )
#	define glCheckError( function, ... ) function( __VA_ARGS__ )
#else
#	define glCheckError( function, ... ) function( __VA_ARGS__ )
#endif

namespace gl
{
	/**
	*\name Remplacement des noms de glm.
	*/
	/**@{*/
	using Vector2D = glm::vec2;
	using Vector3D = glm::vec3;
	using Vector4D = glm::vec4;
	using Quaternion = glm::quat;
	using Matrix2x2 = glm::mat2;
	using Matrix3x3 = glm::mat3;
	using Matrix4x4 = glm::mat4;
	/**@}*/
	/**
	*\name Typedefs généralistes.
	*/
	/**@{*/
	using RgbaColour = Vector4D;
	using RgbColour = Vector3D;
	using Size2D = glm::ivec2;
	using Position2D = glm::ivec2;
	using Offset2D = glm::ivec2;
	using ByteArray = std::vector< uint8_t >;
	using UInt16Array = std::vector< uint16_t >;
	using UInt32Array = std::vector< uint32_t >;
	using Vec3Array = std::vector< Vector3D >;
	using Vec2Array = std::vector< Vector2D >;
	using StringArray = std::vector< std::string >;
	/**@}*/
	/**
	*\name Prédéclarations.
	*/
	/**@{*/
	class AttributeBase;
	class BufferBase;
	class FrameBuffer;
	class GeometryBuffers;
	class OpenGL;
	class Pipeline;
	class RenderBuffer;
	class Sampler;
	class ShaderProgram;
	class Texture;
	class UniformBase;
	/**@}*/
	/**
	*\name Définitions diverses depuis les prédéclarations.
	*/
	/**@{*/
	using BufferBasePtr = std::unique_ptr< BufferBase >;
	using ShaderProgramPtr = std::unique_ptr< ShaderProgram >;
	using TexturePtr = std::unique_ptr< Texture >;
	using FrameBufferPtr = std::unique_ptr< FrameBuffer >;
	using RenderBufferPtr = std::unique_ptr< RenderBuffer >;
	using SamplerPtr = std::unique_ptr< Sampler >;
	using AttributeBasePtr = std::unique_ptr< AttributeBase >;
	using UniformBasePtr = std::unique_ptr< UniformBase >;
	using GeometryBuffersPtr = std::unique_ptr< GeometryBuffers >;
	using AttributeArray = std::vector< AttributeBasePtr >;
	/**@}*/
	/**
	*\name Typedefs de tampons GPU.
	*/
	/**@{*/
	template< typename T >
	class Buffer;
	using UInt8Buffer = Buffer< uint8_t >;
	using UInt16Buffer = Buffer< uint16_t >;
	using Vec2Buffer = Buffer< Vector2D >;
	using Vec3Buffer = Buffer< Vector3D >;
	using Vec4Buffer = Buffer< Vector4D >;
	template< typename T >
	using BufferPtr = std::unique_ptr< Buffer< T > >;
	using UInt8BufferPtr = BufferPtr< uint8_t >;
	using UInt16BufferPtr = BufferPtr< uint16_t >;
	using Vec2BufferPtr = BufferPtr< Vector2D >;
	using Vec3BufferPtr = BufferPtr< Vector3D >;
	using Vec4BufferPtr = BufferPtr< Vector4D >;
	/**@}*/
	/**
	*\name Typedefs d'attributs de sommets.
	*/
	/**@{*/
	template< typename T >
	class Attribute;
	using FloatAttribute = Attribute< float >;
	using Vec2Attribute = Attribute< Vector2D >;
	using Vec3Attribute = Attribute< Vector3D >;
	using Vec4Attribute = Attribute< Vector4D >;
	template< typename T >
	using AttributePtr = std::unique_ptr< Attribute< T > >;
	using FloatAttributePtr = AttributePtr< float >;
	using Vec2AttributePtr = AttributePtr< Vector2D >;
	using Vec3AttributePtr = AttributePtr< Vector3D >;
	using Vec4AttributePtr = AttributePtr< Vector4D >;
	/**@}*/
	/**
	*\name Typedefs de variables uniformes.
	*/
	/**@{*/
	template< typename T >
	class Uniform;
	using IntUniform = Uniform< int >;
	using FloatUniform = Uniform< float >;
	using Vec2Uniform = Uniform< Vector2D >;
	using Vec3Uniform = Uniform< Vector3D >;
	using Vec4Uniform = Uniform< Vector4D >;
	using Mat4Uniform = Uniform< Matrix4x4 >;
	template< typename T >
	using UniformPtr = std::unique_ptr< Uniform< T > >;
	using IntUniformPtr = UniformPtr< int >;
	using FloatUniformPtr = UniformPtr< float >;
	using Vec2UniformPtr = UniformPtr< Vector2D >;
	using Vec3UniformPtr = UniformPtr< Vector3D >;
	using Vec4UniformPtr = UniformPtr< Vector4D >;
	using Mat4UniformPtr = UniformPtr< Matrix4x4 >;
	/**@}*/
	/**
	*\name Typedefs de variables uniformes de type tableau.
	*/
	/**@{*/
	template< typename T >
	class ArrayUniform;
	using IntArrayUniform = ArrayUniform< int >;
	using FloatArrayUniform = ArrayUniform< float >;
	using Vec2ArrayUniform = ArrayUniform< Vector2D >;
	using Vec3ArrayUniform = ArrayUniform< Vector3D >;
	using Vec4ArrayUniform = ArrayUniform< Vector4D >;
	using Mat4ArrayUniform = ArrayUniform< Matrix4x4 >;
	template< typename T >
	using ArrayUniformPtr = std::unique_ptr< ArrayUniform< T > >;
	using IntArrayUniformPtr = ArrayUniformPtr< int >;
	using FloatArrayUniformPtr = ArrayUniformPtr< float >;
	using Vec2ArrayUniformPtr = ArrayUniformPtr< Vector2D >;
	using Vec3ArrayUniformPtr = ArrayUniformPtr< Vector3D >;
	using Vec4ArrayUniformPtr = ArrayUniformPtr< Vector4D >;
	using Mat4ArrayUniformPtr = ArrayUniformPtr< Matrix4x4 >;
	/**@}*/
	/**
	*\brief
	*	Enumération des types de tampons GPU.
	*/
	enum class BufferTarget
		: uint32_t
	{
		//! Tampon de sommets.
		eArrayBuffer,
		//! Tampon d'indices.
		eElementArrayBuffer,
		//! Tampon de variables uniformes.
		eUniformBuffer,
	};
	/**
	*\brief
	*	Enumération des types de valeurs pour les attributs de sommets.
	*/
	enum class AttributeType
	{
		//! int8_t
		eByte,
		//! int16_t
		eShort,
		//! int32_t
		eInt,
		//! uint8_t
		eUByte,
		//! uint16_t
		eUShort,
		//! uint32_t
		eUInt,
		//! float
		eFloat,
	};
	/**
	*\brief
	*	Enumération des types de données pour les attributs de sommets.
	*/
	enum class AttributeFullType
	{
		//! Un int8_t.
		eByte,
		//! Un int16_t.
		eShort,
		//! Un int32_t.
		eInt,
		//! Un uint8_t.
		eUByte,
		//! Un uint16_t.
		eUShort,
		//! Un uint32_t.
		eUInt,
		//! Un float.
		eFloat,
		//! Deux floats.
		eVec2,
		//! Trois floats.
		eVec3,
		//! Quatre floats.
		eVec4,
		//! Deux int32_t.
		eIVec2,
		//! Trois int32_t.
		eIVec3,
		//! Quatre int32_t.
		eIVec4,
	};
	/**
	*\brief
	*	Enumération des points d'attache supportés.
	*/
	enum class AttachmentPoint
	{
		//! Attache du tampon de profondeur.
		eDepth,
		//! Attache du tampon de couleurs.
		eColour0,
	};
	/**
	*\brief
	*	Enumération des formats de pixels de texture supportés.
	*/
	enum class PixelFormat
	{
		//! Luminance 8 bits (pour les textures uniquement).
		eL8,
		//! Luminance 8 bits + Alpha 8 bits (pour les textures uniquement).
		eL8A8,
		//! RGB avec 8 bits par composante (pour les textures uniquement).
		eR8G8B8,
		//! RGB sur 16 bits.
		eRGB565,
		//! RGBA avec 8 bits par composante (pour les textures uniquement).
		eR8G8B8A8,
		//! RGBA sur 16 bits.
		eRGBA5551,
		//! RGBA avec4 bits par composante.
		eRGBA4444,
		//! Profondeur en 16 bits (pour les render buffers uniquement).
		eD16,
		//! Stencil en 8 bits (pour les render buffers uniquement).
		eS8,
		GlLib_EnumBounds( eL8 )
	};
	/**
	*\brief
	*	Enumération des modes de wrap.
	*/
	enum class WrapMode
	{
		//! La texture est limitée à ses bords.
		eClampToEdge,
		//! La texture se répète.
		eRepeat,
		//! La texture se répète en miroir.
		eMirroredRepeat,
	};
	/**
	*\brief
	*	Enumération des filtres de minification.
	*/
	enum class MinFilter
	{
		//!\see https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glTexParameter.xml
		eNearest,
		//!\see https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glTexParameter.xml
		eLinear,
		//!\see https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glTexParameter.xml
		eNearestMipmapNearest,
		//!\see https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glTexParameter.xml
		eLinearMipmapNearest,
		//!\see https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glTexParameter.xml
		eNearestMipmapLinear,
		//!\see https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glTexParameter.xml
		eLinearMipmapLinear,
	};
	/**
	*\brief
	*	Enumération des filtres de magnification.
	*/
	enum class MagFilter
	{
		//!\see https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glTexParameter.xml
		eNearest,
		//!\see https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glTexParameter.xml
		eLinear,
	};
	/**
	*\brief
	*	Donne le nombre d'octets du format de pixels donné.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	Le nombre d'octets.
	*/
	uint32_t pixelSize( PixelFormat format )noexcept;
	/**
	*\brief
	*	Donne le format OpenGL du format de pixels donné.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	Le format OpenGL.
	*/
	uint32_t glFormat( PixelFormat format )noexcept;
	/**
	*\brief
	*	Donne le type OpenGL du format de pixels donné.
	*\param[in] format
	*	Le format de pixel.
	*\return
	*	Le type OpenGL.
	*/
	uint32_t glType( PixelFormat format )noexcept;
	/**
	*\brief
	*	Dit si le format de pixel donné a un canal alpha.
	*\param[in] format
	*	Le format de pixel.
	*/
	inline bool hasAlpha( PixelFormat format )
	{
		return format == PixelFormat::eL8A8
			|| format == PixelFormat::eRGBA5551
			|| format == PixelFormat::eRGBA4444
			|| format == PixelFormat::eR8G8B8A8;
	}
}

#endif