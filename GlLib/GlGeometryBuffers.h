/**
*\file
*	GlGeometryBuffers.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlLib_GeometryBuffers_HPP___
#define ___GlLib_GeometryBuffers_HPP___
#pragma once

#include "GlLibPrerequisites.h"

namespace gl
{
	/**
	*\brief
	*	Contient un tampon GPU et ses attributs.
	*/
	struct BufferAttributes
	{
		//! Le tampon GPU.
		BufferBase const & buffer;
		//! Les attributs liés à ce tampon.
		AttributeArray attributes;
	};
	//! Liste de tampons associés à leurs attributs
	using BufferAttributesArray = std::vector< BufferAttributes >;
	/**
	*\brief
	*	Classe implémentant le concept de VAO OpenGL.
	*	Voir \ref gllib_buffer.
	*/
	class GeometryBuffers
	{
		GeometryBuffers( GeometryBuffers const & ) = delete;
		GeometryBuffers & operator=( GeometryBuffers const & ) = delete;
		GeometryBuffers( GeometryBuffers && ) = delete;
		GeometryBuffers & operator=( GeometryBuffers && ) = delete;

	public:
		/**
		*\brief
		*	Constructeur.
		*\remarks
		*	Acquiert la responsabilité des attributs donnés.
		*\param[in] attributes
		*	La liste des attributs avec leur tampon.
		*/
		GeometryBuffers( BufferAttributesArray && attributes )noexcept;
		/**
		*\brief
		*	Destructeur.
		*/
		~GeometryBuffers()noexcept;
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

	private:
		//! Le nom OpenGL du VAO.
		uint32_t m_name;
		//! Les attributs du VAO.
		AttributeArray m_attributes;
	};
}

#endif
