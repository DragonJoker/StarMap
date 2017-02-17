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

	int * UniformBase::doGetPointer( int & value )noexcept
	{
		return &value;
	}

	float * UniformBase::doGetPointer( float & value )noexcept
	{
		return &value;
	}

	float * UniformBase::doGetPointer( Vector2D & value )noexcept
	{
		return &value.x;
	}

	float * UniformBase::doGetPointer( Vector3D & value )noexcept
	{
		return &value.x;
	}

	float * UniformBase::doGetPointer( Vector4D & value )noexcept
	{
		return &value.x;
	}

	float * UniformBase::doGetPointer( Matrix4x4 & value )noexcept
	{
		return &value[0].x;
	}

	int const * UniformBase::doGetPointer( int const & value )noexcept
	{
		return &value;
	}

	float const * UniformBase::doGetPointer( float const & value )noexcept
	{
		return &value;
	}

	float const * UniformBase::doGetPointer( Vector2D const & value )noexcept
	{
		return &value.x;
	}

	float const * UniformBase::doGetPointer( Vector3D const & value )noexcept
	{
		return &value.x;
	}

	float const * UniformBase::doGetPointer( Vector4D const & value )noexcept
	{
		return &value.x;
	}

	float const * UniformBase::doGetPointer( Matrix4x4 const & value )noexcept
	{
		return &value[0].x;
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
			, doGetPointer( value ) );
	}

	void UniformBase::doSetValue( Vector3D const & value )const noexcept
	{
		glCheckError( glUniform3fv
			, m_location
			, 1
			, doGetPointer( value ) );
	}

	void UniformBase::doSetValue( Vector4D const & value )const noexcept
	{
		glCheckError( glUniform4fv
			, m_location
			, 1
			, doGetPointer( value ) );
	}

	void UniformBase::doSetValue( Matrix4x4 const & value )const noexcept
	{
		glCheckError( glUniformMatrix4fv
			, m_location
			, 1
			, GL_FALSE
			, doGetPointer( value ) );
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
			, doGetPointer( values[0] ) );
	}

	void UniformBase::doSetValues( Vector3D const * values
		, uint32_t count )const noexcept
	{
		glCheckError( glUniform3fv
			, m_location
			, count
			, doGetPointer( values[0] ) );
	}

	void UniformBase::doSetValues( Vector4D const * values
		, uint32_t count )const noexcept
	{
		glCheckError( glUniform4fv
			, m_location
			, count
			, doGetPointer( values[0] ) );
	}

	void UniformBase::doSetValues( Matrix4x4 const * values
		, uint32_t count )const noexcept
	{
		glCheckError( glUniformMatrix4fv
			, m_location
			, count
			, GL_FALSE
			, doGetPointer( values[0] ) );
	}
}
