#include "BillboardBuffer.h"

#include "Camera.h"

namespace render
{
	BillboardBuffer::BillboardBuffer( bool scale )
		: m_scale{ scale }
	{
	}

	void BillboardBuffer::initialise()
	{
		m_vbo = gl::makeBuffer( gl::BufferTarget::eArrayBuffer
			, m_buffer );
		m_visible.resize( m_buffer.size() );
	}

	void BillboardBuffer::cleanup()
	{
		m_vbo.reset();
	}

	void BillboardBuffer::update( float threshold )
	{
		auto it = std::find_if( std::begin( m_buffer )
			, std::end( m_buffer )
			, [&threshold]( auto const & element )
		{
			return element[0].data.magnitude > threshold;
		} );

		if ( it != std::end( m_buffer ) )
		{
			m_unculled = uint32_t( std::distance( std::begin( m_buffer ), it ) );
		}
		else
		{
			m_unculled = uint32_t( m_buffer.size() );
		}
	}

	void BillboardBuffer::cull( Camera const & camera
		, gl::Vector3D const & position
		, float scale )
	{
		auto count = 0u;
		std::for_each( std::begin( m_buffer )
			, std::begin( m_buffer ) + m_unculled
			, [&count
				, &camera
				, &position
				, &scale
				, this]( Quad const & quad )
		{
			if ( m_scale )
			{
				if ( camera.visible( quad[0].data.center + position ) )
				{
					auto & visible = m_visible[count];
					visible = quad;

					visible[0].data.scale *= scale;
					visible[1].data.scale *= scale;
					visible[2].data.scale *= scale;
					visible[3].data.scale *= scale;
					visible[4].data.scale *= scale;
					visible[5].data.scale *= scale;

					count++;
				}
			}
			else
			{
				if ( camera.visible( quad[0].data.center + position ) )
				{
					auto & visible = m_visible[count];
					visible = quad;
				}

				count++;
			}
		} );

		if ( count )
		{
			m_vbo->bind();
			m_vbo->upload( 0u, count, m_visible.data() );
			m_vbo->unbind();
		}

		m_count = count;
	}

	void BillboardBuffer::remove( uint32_t index )
	{
		assert( index < m_buffer.size() );
		m_buffer.erase( m_buffer.begin() + index );
	}

	void BillboardBuffer::add( BillboardData const & data )
	{
		float fid{ float( m_buffer.size() ) };
		auto quad = Quad
		{
			{
				Vertex{ data,{ -0.5, -0.5 }, fid },
				Vertex{ data,{ +0.5, -0.5 }, fid },
				Vertex{ data,{ +0.5, +0.5 }, fid },
				Vertex{ data,{ -0.5, -0.5 }, fid },
				Vertex{ data,{ +0.5, +0.5 }, fid },
				Vertex{ data,{ -0.5, +0.5 }, fid },
			}
		};
		m_buffer.push_back( quad );
	}

	void BillboardBuffer::add( BillboardDataArray const & datas )
	{
		for ( auto & data : datas )
		{
			add( data );
		}
	}

	BillboardData const & BillboardBuffer::at( uint32_t index )const
	{
		assert( index < m_buffer.size() );
		auto it = std::find_if( std::begin( m_buffer )
			, std::end( m_buffer )
			, [&index]( auto const & in )
		{
			return uint32_t( in[0].id ) == index;
		} );
		assert( it != std::end( m_buffer ) );
		return ( *it )[0].data;
	}

	void BillboardBuffer::at( uint32_t index, BillboardData const & data )
	{
		assert( index < m_buffer.size() );
		auto it = std::find_if( std::begin( m_buffer )
			, std::end( m_buffer )
			, [&index]( auto const & in )
		{
			return uint32_t( in[0].id ) == index;
		} );
		assert( it != std::end( m_buffer ) );

		for ( auto & in : ( *it ) )
		{
			in.data = data;
		}
	}
}
