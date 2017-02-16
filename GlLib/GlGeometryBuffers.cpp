#include "GlGeometryBuffers.h"

#include "GlAttributeBase.h"
#include "GlBufferBase.h"

namespace gl
{
	GeometryBuffers::GeometryBuffers( BufferAttributesArray && attributes )noexcept
	{
		glCheckError( glGenVertexArrays, 1, &m_name );
		bind();

		for ( auto & attr : attributes )
		{
			attr.buffer.bind();

			for ( auto & attribute : attr.attributes )
			{
				attribute->bindInstanced();
			}
		}

		unbind();

		for ( auto & attr : attributes )
		{
			for ( auto & attribute : attr.attributes )
			{
				attribute->unbindInstanced();
				m_attributes.push_back( std::move( attribute ) );
			}

			attr.buffer.unbind();
		}
	}

	GeometryBuffers::~GeometryBuffers()noexcept
	{
		glCheckError( glDeleteVertexArrays, 1, &m_name );
	}

	void GeometryBuffers::bind()const noexcept
	{
		assert( m_name != GL_INVALID_INDEX );
		glCheckError( glBindVertexArray, m_name );
	}

	void GeometryBuffers::unbind()const noexcept
	{
		glCheckError( glBindVertexArray, 0u );
	}
}
