#include "GlUniformBase.h"

#include "GlShaderProgram.h"

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

	void UniformBase::doSetValue( int const & value )const noexcept
	{
		glCheckError( glUniform1i
			, m_location
			, value );
	}

	void UniformBase::doSetValue( IVec2 const & value )const noexcept
	{
		glCheckError( glUniform2iv
			, m_location
			, 1
			, doGetPointer( value ) );
	}

	void UniformBase::doSetValue( IVec3 const & value )const noexcept
	{
		glCheckError( glUniform3iv
			, m_location
			, 1
			, doGetPointer( value ) );
	}

	void UniformBase::doSetValue( IVec4 const & value )const noexcept
	{
		glCheckError( glUniform4iv
			, m_location
			, 1
			, doGetPointer( value ) );
	}

	void UniformBase::doSetValue( float const & value )const noexcept
	{
		glCheckError( glUniform1f
			, m_location
			, value );
	}

	void UniformBase::doSetValue( Vec2 const & value )const noexcept
	{
		glCheckError( glUniform2fv
			, m_location
			, 1
			, doGetPointer( value ) );
	}

	void UniformBase::doSetValue( Vec3 const & value )const noexcept
	{
		glCheckError( glUniform3fv
			, m_location
			, 1
			, doGetPointer( value ) );
	}

	void UniformBase::doSetValue( Vec4 const & value )const noexcept
	{
		glCheckError( glUniform4fv
			, m_location
			, 1
			, doGetPointer( value ) );
	}

	void UniformBase::doSetValue( Mat4 const & value )const noexcept
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

	void UniformBase::doSetValues( IVec2 const * values
		, uint32_t count )const noexcept
	{
		glCheckError( glUniform2iv
			, m_location
			, count
			, doGetPointer( values[0] ) );
	}

	void UniformBase::doSetValues( IVec3 const * values
		, uint32_t count )const noexcept
	{
		glCheckError( glUniform3iv
			, m_location
			, count
			, doGetPointer( values[0] ) );
	}

	void UniformBase::doSetValues( IVec4 const * values
		, uint32_t count )const noexcept
	{
		glCheckError( glUniform4iv
			, m_location
			, count
			, doGetPointer( values[0] ) );
	}

	void UniformBase::doSetValues( float const * values
		, uint32_t count )const noexcept
	{
		glCheckError( glUniform1fv
			, m_location
			, count
			, values );
	}

	void UniformBase::doSetValues( Vec2 const * values
		, uint32_t count )const noexcept
	{
		glCheckError( glUniform2fv
			, m_location
			, count
			, doGetPointer( values[0] ) );
	}

	void UniformBase::doSetValues( Vec3 const * values
		, uint32_t count )const noexcept
	{
		glCheckError( glUniform3fv
			, m_location
			, count
			, doGetPointer( values[0] ) );
	}

	void UniformBase::doSetValues( Vec4 const * values
		, uint32_t count )const noexcept
	{
		glCheckError( glUniform4fv
			, m_location
			, count
			, doGetPointer( values[0] ) );
	}

	void UniformBase::doSetValues( Mat4 const * values
		, uint32_t count )const noexcept
	{
		glCheckError( glUniformMatrix4fv
			, m_location
			, count
			, GL_FALSE
			, doGetPointer( values[0] ) );
	}

	int * UniformBase::doGetPointer( int & value )noexcept
	{
		return &value;
	}

	int * UniformBase::doGetPointer( IVec2 & value )noexcept
	{
		return &value.x;
	}

	int * UniformBase::doGetPointer( IVec3 & value )noexcept
	{
		return &value.x;
	}

	int * UniformBase::doGetPointer( IVec4 & value )noexcept
	{
		return &value.x;
	}

	float * UniformBase::doGetPointer( float & value )noexcept
	{
		return &value;
	}

	float * UniformBase::doGetPointer( Vec2 & value )noexcept
	{
		return &value.x;
	}

	float * UniformBase::doGetPointer( Vec3 & value )noexcept
	{
		return &value.x;
	}

	float * UniformBase::doGetPointer( Vec4 & value )noexcept
	{
		return &value.x;
	}

	float * UniformBase::doGetPointer( Mat4 & value )noexcept
	{
		return &value[0].x;
	}

	int const * UniformBase::doGetPointer( int const & value )noexcept
	{
		return &value;
	}

	int const * UniformBase::doGetPointer( IVec2 const & value )noexcept
	{
		return &value.x;
	}

	int const * UniformBase::doGetPointer( IVec3 const & value )noexcept
	{
		return &value.x;
	}

	int const * UniformBase::doGetPointer( IVec4 const & value )noexcept
	{
		return &value.x;
	}

	float const * UniformBase::doGetPointer( float const & value )noexcept
	{
		return &value;
	}

	float const * UniformBase::doGetPointer( Vec2 const & value )noexcept
	{
		return &value.x;
	}

	float const * UniformBase::doGetPointer( Vec3 const & value )noexcept
	{
		return &value.x;
	}

	float const * UniformBase::doGetPointer( Vec4 const & value )noexcept
	{
		return &value.x;
	}

	float const * UniformBase::doGetPointer( Mat4 const & value )noexcept
	{
		return &value[0].x;
	}
}
