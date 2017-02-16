#include "GlBufferBase.h"

namespace gl
{
	namespace
	{
		inline uint32_t doGetTarget( BufferTarget target )
		{
			switch ( target )
			{
			case BufferTarget::eArrayBuffer:
				return GL_ARRAY_BUFFER;
				break;

			case BufferTarget::eElementArrayBuffer:
				return GL_ELEMENT_ARRAY_BUFFER;
				break;

			case BufferTarget::eUniformBuffer:
				return GL_UNIFORM_BUFFER;
				break;

			default:
				assert( false );
				break;
			}

			return 0;
		}
	}

	BufferBase::BufferBase( BufferTarget target
		, void const * data
		, size_t size )noexcept
		: m_target{ doGetTarget( target ) }
		, m_size{ uint32_t( size ) }
	{
		glCheckError( glGenBuffers, 1, &m_name );

		if ( size && data )
		{
			bind();
			glCheckError( glBufferData, m_target, size, data, GL_STATIC_DRAW );
			unbind();
		}
	}

	BufferBase::~BufferBase()noexcept
	{
		glCheckError( glDeleteBuffers, 1, &m_name );
	}

	void BufferBase::bind()const noexcept
	{
		GlLib_DebugAssertFalse( m_bound );
		assert( m_name != GL_INVALID_INDEX );
		glCheckError( glBindBuffer, m_target, m_name );
		GlLib_DebugSetTrue( m_bound );
	}

	void BufferBase::unbind()const noexcept
	{
		GlLib_DebugAssertTrue( m_bound );
		glCheckError( glBindBuffer, m_target, 0u );
		GlLib_DebugSetFalse( m_bound );
	}

	void BufferBase::bindingPoint( uint32_t index )const noexcept
	{
		assert( m_name != GL_INVALID_INDEX );
		glCheckError( glBindBufferBase, m_target, index, m_name );
	}

	void BufferBase::resize( uint32_t size )noexcept
	{
		GlLib_DebugAssertTrue( m_bound );
		m_size = size;
		glCheckError( glBufferData, m_target, size, nullptr, GL_STATIC_DRAW );
	}

	void BufferBase::upload( uint32_t offset
		, uint32_t size
		, void const * data )const noexcept
	{
		GlLib_DebugAssertTrue( m_bound );
		assert( offset + size <= this->size() );
		glCheckError( glBufferSubData, m_target, offset, size, data );
	}

	void BufferBase::download( uint32_t offset
		, uint32_t size
		, void * data )const noexcept
	{
		GlLib_DebugAssertTrue( m_bound );
		assert( offset + size <= this->size() );
		auto vdata = lock( offset, size, GL_MAP_READ_BIT );

		if ( vdata )
		{
			memcpy( data, vdata, offset );
			unlock();
		}
	}

	void * BufferBase::lock( uint32_t offset
		, uint32_t size
		, uint32_t access )const noexcept
	{
		GlLib_DebugAssertTrue( m_bound );
		assert( offset + size <= this->size() );
		auto ret = glCheckError( glMapBufferRange, m_target, offset, size, access );
		return ret;
	}

	void BufferBase::unlock()const noexcept
	{
		GlLib_DebugAssertTrue( m_bound );
		glCheckError( glUnmapBuffer, m_target );
	}
}
