#include "pch.h"
#include "GlUniformBuffer.h"

#include "GlShaderProgram.h"
#include "OpenGL.h"

#include <algorithm>

#define DEBUG_UBO 0

namespace gl
{
	namespace
	{
		using UIntArray = std::vector< uint32_t >;

		UIntArray doListIndices( ShaderProgram const & program
			, UniformBuffer const & ubo )
		{
			gl::StringArray names;

			for ( auto & variable : ubo )
			{
				names.push_back( variable->name() );
			}

			UIntArray ret( names.size() );
			return program.uniformIndices( names );
		}

		void doRetrieveLayout( ShaderProgram const & program
			, UniformBuffer const & ubo
			, UIntArray const & indices
			, UniformBuffer::UniformInfoArray & infos )
		{
#if DEBUG_UBO
			std::clog << "UniformBuffer " << ubo.name() << std::endl;
#endif
			auto it = ubo.begin();

			for ( auto & index : indices )
			{
				if ( index != GL_INVALID_INDEX )
				{
					int offset = program.activeUniforms( index, GL_UNIFORM_OFFSET );
					int stride = program.activeUniforms( index, GL_UNIFORM_ARRAY_STRIDE );
					int size = program.activeUniforms( index, GL_UNIFORM_SIZE );
					infos.push_back( { uint32_t( offset ), uint32_t( stride ), uint32_t( size ) } );
				}
				else
				{
					infos.push_back( { uint32_t( -1 ), uint32_t( -1 ), uint32_t( -1 ) } );
				}

#if DEBUG_UBO
				std::clog << ( *it )->name()
					<< ": I:" << index
					<< ", O:" << infos.back().m_offset
					<< ", S:" << infos.back().m_stride
					<< ", U:" << infos.back().m_size
					<< std::endl;
#endif
				++it;
			}
		}
	}

	UniformBuffer::UniformBuffer( std::string const & name
		, uint32_t point
		, ShaderProgram const & program )
		: m_program{ program }
		, m_name{ name }
		, m_point{ point }
		, m_ubo{ GlLib_UseUBO
			&& OpenGL::checkSupport( FeatureLevel::eGLES3 ) }
	{
	}

	UniformBuffer::~UniformBuffer()
	{
	}

	void UniformBuffer::removeUniform( std::string const & name )
	{
		auto itMap = m_mapVariables.find( name );

		if ( itMap != m_mapVariables.end() )
		{
			m_mapVariables.erase( itMap );
		}

		auto itList = std::find_if( m_listVariables.begin()
			, m_listVariables.end()
			, [&name]( UniformPtr const & variable )
			{
				return name == variable->name();
			} );

		if ( itList != m_listVariables.end() )
		{
			m_listVariables.erase( itList );
		}
	}

	void UniformBuffer::initialise()
	{
		if ( m_ubo )
		{
			m_blockIndex = m_program.uniformBlockIndex( m_name );

			if ( m_blockIndex != GL_INVALID_INDEX )
			{
				uint32_t size = uint32_t( m_program.activeUniformBlock
					( m_blockIndex
					, GL_UNIFORM_BLOCK_DATA_SIZE ) );
				m_program.uniformBlockBinding( m_blockIndex, m_point );
				m_buffer.resize( size );
				auto indices = doListIndices( m_program, *this );
				doRetrieveLayout( m_program
					, *this
					, indices
					, m_infos );
#if DEBUG_UBO
				std::clog << "TotalSize: " << size << std::endl;
#endif
			}

			m_storage = makeBuffer
				( BufferTarget::eUniformBuffer
				, m_buffer );
			m_storage->bindingPoint( m_point );
		}
	}

	void UniformBuffer::cleanup()
	{
		m_storage.reset();
		m_mapVariables.clear();
		m_listVariables.clear();
	}

	void UniformBuffer::bind( uint32_t index )const noexcept
	{
		if ( m_ubo )
		{
			if ( m_blockIndex != GL_INVALID_INDEX )
			{
				doUpdate();
				m_storage->bindingPoint( index );
			}
		}
		else
		{
			for ( auto & variable : *this )
			{
				if ( variable->valid() )
				{
					variable->bind();
				}
			}
		}
	}

	void UniformBuffer::doUpdate()const
	{
		if ( m_ubo )
		{
			bool changed = m_listVariables.end() != std::find_if
			( m_listVariables.begin()
				, m_listVariables.end()
				, []( UniformPtr const & variable )
			{
				return variable->changed();
			} );

			if ( changed )
			{
				auto it = m_infos.begin();

				for ( auto & variable : *this )
				{
					assert( it->m_offset + variable->size() <= m_buffer.size() );

					if ( it->m_stride != uint32_t( -1 ) )
					{
						memcpy( &m_buffer[it->m_offset]
							, variable->c_ptr()
							, variable->size() );
					}

					++it;
					variable->changed( false );
				}

				m_storage->bind();
				m_storage->upload( 0u
					, uint32_t( m_buffer.size() )
					, m_buffer.data() );
				m_storage->unbind();
			}
		}
	}

	//*************************************************************************************************
}
