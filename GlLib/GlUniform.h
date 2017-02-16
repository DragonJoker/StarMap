/**
*\file
*	GlUniform.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlLib_Uniform_HPP___
#define ___GlLib_Uniform_HPP___
#pragma once

#include "GlUniformBase.h"

namespace gl
{
	/**
	*\brief
	*	Gère une variable uniforme.
	*	Voir \ref gllib_uniform.
	*/
	template< typename T >
	class Uniform
		: public UniformBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] name
		*	Le nom de la variable.
		*\param[in] program
		*	Le programme shader.
		*/
		inline Uniform( std::string const & name
			, ShaderProgram const & program )noexcept
			: UniformBase{ name, program }
		{
			doSetPointer( UniformBase::doGetPointer( m_value ), sizeof( T ) );
		}
		/**
		*\brief
		*	Destructeur.
		*/
		inline ~Uniform()noexcept
		{
		}
		/**
		*\brief
		*	Définit la valeur de la variable uniforme.
		*\param[in] value
		*	La valeur.
		*/
		inline void value( T const & value )noexcept
		{
			changed( changed() || ( m_value != value ) );
			m_value = value;
		}
		/**
		*\return
		*	La valeur de la variable uniforme.
		*/
		inline T const & value()const noexcept
		{
			return m_value;
		}
		/**
		*\brief
		*	Applique la variable uniforme.
		*/
		inline void bind()const noexcept override
		{
			assert( UniformBase::valid() );
			UniformBase::doSetValue( m_value );
		}

	private:
		//! La valeur de la variable.
		T m_value;
	};
	/**
	*\brief
	*	Fonction d'aide à la création d'un Uniform
	*\param[in] name
	*	Le nom de l'attribut.
	*\param[in] program
	*	Le programme shader dans lequel on va rechercher la variable.
	*\return
	*	La variable uniforme créée.
	*/
	template< typename T >
	std::unique_ptr< Uniform< T > > makeUniform( std::string const & name
		, ShaderProgram const & program )
	{
		return std::make_unique< Uniform< T > >( name, program );
	}
}

#endif
