#include "pch.h"
#include "GlAttributeBase.h"

#include "GlShaderProgram.h"
#include "OpenGL.h"

namespace gl
{
	namespace
	{
		inline uint32_t doGetAttributeType( AttributeType type )
		{
			switch ( type )
			{
			case AttributeType::eByte:
				return GL_BYTE;
				break;

			case AttributeType::eShort:
				return GL_SHORT;
				break;

			case AttributeType::eInt:
				return GL_INT;
				break;

			case AttributeType::eUByte:
				return GL_UNSIGNED_BYTE;
				break;

			case AttributeType::eUShort:
				return GL_UNSIGNED_SHORT;
				break;

			case AttributeType::eUInt:
				return GL_UNSIGNED_INT;
				break;

			case AttributeType::eFloat:
				return GL_FLOAT;
				break;

			default:
				assert( false );
				break;
			}

			return 0;
		}
	}

	AttributeBase::AttributeBase( uint32_t location
		, uint32_t stride
		, uint32_t offset
		, uint32_t divisor
		, int size
		, AttributeType type )noexcept
		: m_location{ location }
		, m_stride{ stride }
		, m_offset{ offset }
		, m_divisor{ divisor }
		, m_size{ size }
		, m_type{ doGetAttributeType( type ) }
	{
	}

	AttributeBase::~AttributeBase()noexcept
	{
	}

	void AttributeBase::bind( void const * buffer
		, uint32_t offset )const noexcept
	{
		assert( valid() );
		glCheckError( glEnableVertexAttribArray, m_location );
		glCheckError( glVertexAttribPointer
			, m_location
			, m_size
			, m_type
			, GL_FALSE
			, m_stride
			, ( uint8_t const * )buffer + offset + m_offset );
	}

	void AttributeBase::bindInstanced( void const * buffer
		, uint32_t offset )const noexcept
	{
		bind( buffer, offset );
		glCheckError( glVertexAttribDivisor, m_location, m_divisor );
	}

	void AttributeBase::unbind()const noexcept
	{
		assert( valid() );
		glCheckError( glDisableVertexAttribArray, m_location );
	}

	void AttributeBase::unbindInstanced()const noexcept
	{
		glCheckError( glVertexAttribDivisor, m_location, 0 );
		unbind();
	}
}
