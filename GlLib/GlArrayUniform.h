/**
*\file
*	GlArrayUniform.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlLib_ArrayUniform_HPP___
#define ___GlLib_ArrayUniform_HPP___
#pragma once

#include "GlUniformBase.h"

namespace gl
{
	/**
	*\brief
	*	Gère une variable uniforme de type tableau.
	*	Voir \ref gllib_uniform.
	*/
	template< typename T >
	class ArrayUniform
		: public UniformBase
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] name
		*	Le nom de la variable.
		*\param[in] count
		*	Le nombre de cases du tableau.
		*\param[in] program
		*	Le programme shader.
		*/
		inline ArrayUniform( std::string const & name
			, uint32_t count
			, ShaderProgram const & program )noexcept
			: UniformBase{ name, program }
			, m_values( count, T{} )
		{
			doSetPointer( m_values.data()
				, uint32_t( sizeof( T ) * m_values.size() ) );
		}
		/**
		*\brief
		*	Destructeur.
		*/
		inline ~ArrayUniform()noexcept
		{
		}
		/**
		*\brief
		*	Définit une valeur de la variable uniforme.
		*\param[in] index
		*	L'indice de la valeur à définit.
		*\param[in] value
		*	La valeur.
		*/
		inline void value( uint32_t index, T const & value )noexcept
		{
			assert( UniformBase::valid() );
			assert( index < m_values.size() );
			m_values[index] = value;
			changed( true );
		}
		/**
		*\brief
		*	Définit les valeurs de la variable uniforme.
		*\param[in] values
		*	Les valeurs.
		*/
		inline void values( std::vector< T > const & values )noexcept
		{
			assert( UniformBase::valid() );
			assert( values.size() <= m_values.size() );
			uint32_t index = 0;

			for ( auto & value : values )
			{
				m_values[index++] = value;
			}

			changed( true );
		}
		/**
		*\brief
		*	Applique la variable uniforme.
		*/
		inline void bind()const noexcept override
		{
			assert( UniformBase::valid() );
			UniformBase::doSetValues( m_values.data()
				, uint32_t( m_values.size() ) );
		}

	private:
		//! Le tableau de valeurs.
		std::vector< T > m_values;
	};
	/**
	*\brief
	*	Fonction d'aide à la création d'un ArrayUniform.
	*\param[in] name
	*	Le nom de la variable uniforme.
	*\param[in] count
	*	Le nombre de cases du tableau.
	*\param[in] program
	*	Le programme shader dans lequel on va rechercher la variable.
	*\return
	*	La variable uniforme créée.
	*/
	template< typename T >
	std::unique_ptr< ArrayUniform< T > > makeUniform( std::string const & name
		, uint32_t count
		, ShaderProgram const & program )
	{
		return std::make_unique< ArrayUniform< T > >( name, count, program );
	}
}

#endif
