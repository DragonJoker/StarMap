#include "GlUniformBase.h"

#include "GlShaderProgram.h"

#include "glm/gtc/type_ptr.hpp"

namespace gl
{
	UniformBase::UniformBase( std::string const & name
		, ShaderProgram const & program )noexcept
		: m_name{ name }
		, m_location{ program.uniformLocation( name ) }
	{
	}

	UniformBase::~UniformBase()noexcept
	{
	}

	void UniformBase::doSetPointer( void * pointer, size_t size )
	{
		m_pointer = pointer;
		m_size = uint32_t( size );
	}

	void * UniformBase::doGetPointer( int & value )const noexcept
	{
		return &value;
	}

	void * UniformBase::doGetPointer( float & value )const noexcept
	{
		return &value;
	}

	void * UniformBase::doGetPointer( Vector2D & value )const noexcept
	{
		return glm::value_ptr( value );
	}

	void * UniformBase::doGetPointer( Vector3D & value )const noexcept
	{
		return glm::value_ptr( value );
	}

	void * UniformBase::doGetPointer( Vector4D & value )const noexcept
	{
		return glm::value_ptr( value );
	}

	void * UniformBase::doGetPointer( Matrix4x4 & value )const noexcept
	{
		return glm::value_ptr( value );
	}

	void UniformBase::doSetValue( int const & value )const noexcept
	{
		glCheckError( glUniform1i
			, m_location
			, value );
	}

	void UniformBase::doSetValue( float const & value )const noexcept
	{
		glCheckError( glUniform1f
			, m_location
			, value );
	}

	void UniformBase::doSetValue( Vector2D const & value )const noexcept
	{
		glCheckError( glUniform2fv
			, m_location
			, 1
			, glm::value_ptr( value ) );
	}

	void UniformBase::doSetValue( Vector3D const & value )const noexcept
	{
		glCheckError( glUniform3fv
			, m_location
			, 1
			, glm::value_ptr( value ) );
	}

	void UniformBase::doSetValue( Vector4D const & value )const noexcept
	{
		glCheckError( glUniform4fv
			, m_location
			, 1
			, glm::value_ptr( value ) );
	}

	void UniformBase::doSetValue( Matrix4x4 const & value )const noexcept
	{
		glCheckError( glUniformMatrix4fv
			, m_location
			, 1
			, GL_FALSE
			, glm::value_ptr( value ) );
	}

	void UniformBase::doSetValues( int const * values
		, uint32_t count )const noexcept
	{
		glCheckError( glUniform1iv
			, m_location
			, count
			, values );
	}

	void UniformBase::doSetValues( float const * values
		, uint32_t count )const noexcept
	{
		glCheckError( glUniform1fv
			, m_location
			, count
			, values );
	}

	void UniformBase::doSetValues( Vector2D const * values
		, uint32_t count )const noexcept
	{
		glCheckError( glUniform2fv
			, m_location
			, count
			, glm::value_ptr( values[0] ) );
	}

	void UniformBase::doSetValues( Vector3D const * values
		, uint32_t count )const noexcept
	{
		glCheckError( glUniform3fv
			, m_location
			, count
			, glm::value_ptr( values[0] ) );
	}

	void UniformBase::doSetValues( Vector4D const * values
		, uint32_t count )const noexcept
	{
		glCheckError( glUniform4fv
			, m_location
			, count
			, glm::value_ptr( values[0] ) );
	}

	void UniformBase::doSetValues( Matrix4x4 const * values
		, uint32_t count )const noexcept
	{
		glCheckError( glUniformMatrix4fv
			, m_location
			, count
			, GL_FALSE
			, glm::value_ptr( values[0] ) );
	}
}
