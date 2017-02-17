/**
*\file
*	GlUniformBase.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlLib_UniformBase_HPP___
#define ___GlLib_UniformBase_HPP___
#pragma once

#include "GlLibPrerequisites.h"

namespace gl
{
	/**
	*\brief
	*	Classe de base d'une variable uniforme.
	*	Voir \ref gllib_uniform.
	*/
	class UniformBase
	{
	protected:
		/**
		*\brief
		*	Constructeur.
		*\param[in] name
		*	Le nom de la variable.
		*\param[in] program
		*	Le programme shader.
		*/
		UniformBase( std::string const & name
			, ShaderProgram const & program )noexcept;

	public:
		/**
		*\brief
		*	Destructeur.
		*/
		virtual ~UniformBase()noexcept;
		/**
		*\brief
		*	Applique la variable uniforme.
		*/
		virtual void bind()const noexcept = 0;
		/**
		*\return
		*	\p false si la variable n'est pas défini.
		*/
		inline bool valid()const noexcept
		{
			return m_location != GL_INVALID_INDEX;
		}
		/**
		*\return
		*	Le nom de la variable.
		*/
		inline std::string const & name()const noexcept
		{
			return m_name;
		}
		/**
		*\return
		*	\p true si la valeur de la variable uniforme a changé.
		*/
		inline bool changed()const noexcept
		{
			return m_changed;
		}
		/**
		*\brief
		*	Définit si la valeur de la variable uniforme a changé.
		*\param[in] value
		*	La nouvelle valeur.
		*/
		inline void changed( bool value )noexcept
		{
			m_changed = value;
		}
		/**
		*\return
		*	Un pointeur sur la valeur de cette variable.
		*/
		inline void const * c_ptr()const noexcept
		{
			return m_pointer;
		}
		/**
		*\return
		*	La taille de la valeur de cette variable.
		*/
		inline uint32_t size()const noexcept
		{
			return m_size;
		}

	protected:
		/**
		*\brief
		*	Définit la valeur pointeur de la variable uniforme.
		*\param[in] pointer
		*	Le pointeur.
		*\param[in] size
		*	La taille du pointeur.
		*/
		void doSetPointer( void * pointer, size_t size );
		/**
		*\brief
		*	Définit la valeur int de la variable uniforme.
		*\param[in] value
		*	La valeur.
		*/
		void doSetValue( int const & value )const noexcept;
		/**
		*\brief
		*	Définit la valeur IVec2 de la variable uniforme.
		*\param[in] value
		*	La valeur.
		*/
		void doSetValue( IVec2 const & value )const noexcept;
		/**
		*\brief
		*	Définit la valeur IVec3 de la variable uniforme.
		*\param[in] value
		*	La valeur.
		*/
		void doSetValue( IVec3 const & value )const noexcept;
		/**
		*\brief
		*	Définit la valeur IVec4 de la variable uniforme.
		*\param[in] value
		*	La valeur.
		*/
		void doSetValue( IVec4 const & value )const noexcept;
		/**
		*\brief
		*	Définit la valeur float de la variable uniforme.
		*\param[in] value
		*	La valeur.
		*/
		void doSetValue( float const & value )const noexcept;
		/**
		*\brief
		*	Définit la valeur Vec2 de la variable uniforme.
		*\param[in] value
		*	La valeur.
		*/
		void doSetValue( Vec2 const & value )const noexcept;
		/**
		*\brief
		*	Définit la valeur Vec3 de la variable uniforme.
		*\param[in] value
		*	La valeur.
		*/
		void doSetValue( Vec3 const & value )const noexcept;
		/**
		*\brief
		*	Définit la valeur Vec4 de la variable uniforme.
		*\param[in] value
		*	La valeur.
		*/
		void doSetValue( Vec4 const & value )const noexcept;
		/**
		*\brief
		*	Définit la valeur mat4 de la variable uniforme.
		*\param[in] value
		*	La valeur.
		*/
		void doSetValue( Matrix4x4 const & value )const noexcept;
		/**
		*\brief
		*	Définit la valeur int de la variable uniforme.
		*\param[in] values
		*	La valeur.
		*\param[in] count
		*	Le nombre de valeurs.
		*/
		void doSetValues( int const * values
			, uint32_t count )const noexcept;
		/**
		*\brief
		*	Définit la valeur vec2 de la variable uniforme.
		*\param[in] values
		*	La valeur.
		*\param[in] count
		*	Le nombre de valeurs.
		*/
		void doSetValues( IVec2 const * values
			, uint32_t count )const noexcept;
		/** 
		*\brief
		*	Définit la valeur vec3 de la variable uniforme.
		*\param[in] values
		*	La valeur.
		*\param[in] count
		*	Le nombre de valeurs.
		*/
		void doSetValues( IVec3 const * values
			, uint32_t count )const noexcept;
		/**
		*\brief
		*	Définit la valeur vec4 de la variable uniforme.
		*\param[in] values
		*	La valeur.
		*\param[in] count
		*	Le nombre de valeurs.
		*/
		void doSetValues( IVec4 const * values
			, uint32_t count )const noexcept;
		/**
		*\brief
		*	Définit la valeur float de la variable uniforme.
		*\param[in] values
		*	La valeur.
		*\param[in] count
		*	Le nombre de valeurs.
		*/
		void doSetValues( float const * values
			, uint32_t count )const noexcept;
		/**
		*\brief
		*	Définit la valeur vec2 de la variable uniforme.
		*\param[in] values
		*	La valeur.
		*\param[in] count
		*	Le nombre de valeurs.
		*/
		void doSetValues( Vec2 const * values
			, uint32_t count )const noexcept;
		/** 
		*\brief
		*	Définit la valeur vec3 de la variable uniforme.
		*\param[in] values
		*	La valeur.
		*\param[in] count
		*	Le nombre de valeurs.
		*/
		void doSetValues( Vec3 const * values
			, uint32_t count )const noexcept;
		/**
		*\brief
		*	Définit la valeur vec4 de la variable uniforme.
		*\param[in] values
		*	La valeur.
		*\param[in] count
		*	Le nombre de valeurs.
		*/
		void doSetValues( Vec4 const * values
			, uint32_t count )const noexcept;
		/**
		*\brief
		*	Définit la valeur mat4 de la variable uniforme.
		*\param[in] values
		*	La valeur.
		*\param[in] count
		*	Le nombre de valeurs.
		*/
		void doSetValues( Matrix4x4 const * values
			, uint32_t count )const noexcept;
		/**
		*\brief
		*	Récupère le pointeur de la valeur int donnée.
		*\param[in] value
		*	La valeur.
		*\return
		*	Le pointeur.
		*/
		static int * doGetPointer( int & value )noexcept;
		/**
		*\brief
		*	Récupère le pointeur de la valeur IVec2 donnée.
		*\param[in] value
		*	La valeur.
		*\return
		*	Le pointeur.
		*/
		static int * doGetPointer( IVec2 & value )noexcept;
		/**
		*\brief
		*	Récupère le pointeur de la valeur IVec3 donnée.
		*\param[in] value
		*	La valeur.
		*\return
		*	Le pointeur.
		*/
		static int * doGetPointer( IVec3 & value )noexcept;
		/**
		*\brief
		*	Récupère le pointeur de la valeur IVec4 donnée.
		*\param[in] value
		*	La valeur.
		*\return
		*	Le pointeur.
		*/
		static int * doGetPointer( IVec4 & value )noexcept;
		/**
		*\brief
		*	Récupère le pointeur de la valeur float donnée.
		*\param[in] value
		*	La valeur.
		*\return
		*	Le pointeur.
		*/
		static float * doGetPointer( float & value )noexcept;
		/**
		*\brief
		*	Récupère le pointeur de la valeur Vec2 donnée.
		*\param[in] value
		*	La valeur.
		*\return
		*	Le pointeur.
		*/
		static float * doGetPointer( Vec2 & value )noexcept;
		/**
		*\brief
		*	Récupère le pointeur de la valeur Vec3 donnée.
		*\param[in] value
		*	La valeur.
		*\return
		*	Le pointeur.
		*/
		static float * doGetPointer( Vec3 & value )noexcept;
		/**
		*\brief
		*	Récupère le pointeur de la valeur Vec4 donnée.
		*\param[in] value
		*	La valeur.
		*\return
		*	Le pointeur.
		*/
		static float * doGetPointer( Vec4 & value )noexcept;
		/**
		*\brief
		*	Récupère le pointeur de la valeur Matrix4x4 donnée.
		*\param[in] value
		*	La valeur.
		*\return
		*	Le pointeur.
		*/
		static float * doGetPointer( Matrix4x4 & value )noexcept;
		/**
		*\brief
		*	Récupère le pointeur de la valeur int donnée.
		*\param[in] value
		*	La valeur.
		*\return
		*	Le pointeur.
		*/
		static int const * doGetPointer( int const & value )noexcept;
		/**
		*\brief
		*	Récupère le pointeur de la valeur IVec2 donnée.
		*\param[in] value
		*	La valeur.
		*\return
		*	Le pointeur.
		*/
		static int const * doGetPointer( IVec2 const & value )noexcept;
		/**
		*\brief
		*	Récupère le pointeur de la valeur IVec3 donnée.
		*\param[in] value
		*	La valeur.
		*\return
		*	Le pointeur.
		*/
		static int const * doGetPointer( IVec3 const & value )noexcept;
		/**
		*\brief
		*	Récupère le pointeur de la valeur IVec4 donnée.
		*\param[in] value
		*	La valeur.
		*\return
		*	Le pointeur.
		*/
		static int const * doGetPointer( IVec4 const & value )noexcept;
		/**
		*\brief
		*	Récupère le pointeur de la valeur float donnée.
		*\param[in] value
		*	La valeur.
		*\return
		*	Le pointeur.
		*/
		static float const * doGetPointer( float const & value )noexcept;
		/**
		*\brief
		*	Récupère le pointeur de la valeur Vec2 donnée.
		*\param[in] value
		*	La valeur.
		*\return
		*	Le pointeur.
		*/
		static float const * doGetPointer( Vec2 const & value )noexcept;
		/**
		*\brief
		*	Récupère le pointeur de la valeur Vec3 donnée.
		*\param[in] value
		*	La valeur.
		*\return
		*	Le pointeur.
		*/
		static float const * doGetPointer( Vec3 const & value )noexcept;
		/**
		*\brief
		*	Récupère le pointeur de la valeur Vec4 donnée.
		*\param[in] value
		*	La valeur.
		*\return
		*	Le pointeur.
		*/
		static float const * doGetPointer( Vec4 const & value )noexcept;
		/**
		*\brief
		*	Récupère le pointeur de la valeur Matrix4x4 donnée.
		*\param[in] value
		*	La valeur.
		*\return
		*	Le pointeur.
		*/
		static float const * doGetPointer( Matrix4x4 const & value )noexcept;

	private:
		//! Le nom de la variable.
		std::string m_name;
		//! Le nom OpenGL de la variable.
		uint32_t m_location{ GL_INVALID_INDEX };
		//! Dit si la variable a changé depuis sa dernière mise à jour.
		bool m_changed{ true };
		//! Le pointeur vers la valeur de la variable.
		void * m_pointer{ nullptr };
		//! La taille de la valeur de la variable.
		uint32_t m_size{ 0u };
	};
}

#endif
