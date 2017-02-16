#include "Picking.h"

#include "Billboard.h"
#include "Camera.h"
#include "Mesh.h"
#include "Object.h"
#include "Scene.h"
#include "Submesh.h"
#include "Viewport.h"

namespace render
{
	namespace
	{
		template< typename NodeT >
		struct NodeTraits;

		template<>
		struct NodeTraits< Object >
		{
			static auto constexpr mask = ObjectMask;

			static inline void getNodeValue( RenderSubmeshVector::const_iterator it
				, Object *& object )
			{
				object = it->m_object.get();
			}
		};

		template<>
		struct NodeTraits< Billboard >
		{
			static auto constexpr mask = BillboardMask;

			static inline void getNodeValue( BillboardArray::const_iterator it
				, Billboard *& billboard )
			{
				billboard = it->get();
			}
		};

		template< typename MapT, typename NodeT >
		inline void doPickFromList( MapT const & map
			, Picking::Pixel const & index
			, NodeT *& node )
		{
			using Traits = NodeTraits< NodeT >;
			uint8_t pipelineIndex{ uint8_t( index.r & ~Traits::mask ) };
			uint16_t nodeIndex{ uint16_t( ( uint16_t( index.g ) << 8 )
				+ uint16_t( index.b ) ) };
			assert( map[pipelineIndex].size() > nodeIndex );
			auto itNode = map[pipelineIndex].begin() + nodeIndex;
			Traits::getNodeValue( itNode, node );
		}

		static int constexpr PickingWidth = 50;
		static int constexpr PickingOffset = PickingWidth / 2;
	}

	Picking::Picking( gl::Size2D const & size )
		: m_renderer{}
		, m_size{ size }
		, m_colour{ std::make_unique< gl::Texture >
			( gl::PixelFormat::eR8G8B8A8
			, size ) }
		, m_depth{ std::make_unique< gl::RenderBuffer >
			( gl::PixelFormat::eD16
			, size ) }
		, m_fbo{ std::make_unique< gl::FrameBuffer >() }
		, m_buffer( PickingWidth * PickingWidth )
	{
		m_renderer.initialise();
		m_fbo->bind();
		m_fbo->attach( *m_colour, gl::AttachmentPoint::eColour0 );
		m_fbo->attach( *m_depth, gl::AttachmentPoint::eDepth );
		m_fbo->unbind();
	}

	Picking::~Picking()
	{
		m_fbo->bind();
		m_fbo->detach( *m_colour, gl::AttachmentPoint::eColour0 );
		m_fbo->detach( *m_depth, gl::AttachmentPoint::eDepth );
		m_fbo->unbind();
	}

	Picking::NodeType Picking::pick( gl::Position2D const & position
		, Camera const & camera
		, float zoomPercent
		, RenderSubmeshArray const & objects
		, RenderBillboardArray const & billboards )const
	{
#if DEBUG_PICKING
		m_fbo->clear( gl::RgbaColour{ 0, 0, 0, 1 } );
		m_renderer.draw( camera
			, zoomPercent
			, objects
			, billboards );
		return NodeType::eNone;
#else
		onUnpick();
		auto pixel = doFboPick( position
			, camera
			, zoomPercent
			, objects
			, billboards );
		return doPick( pixel, objects, billboards );
#endif
	}

	Picking::Pixel Picking::doFboPick( gl::Position2D const & position
		, Camera const & camera
		, float zoomPercent
		, RenderSubmeshArray const & objects
		, RenderBillboardArray const & billboards )const
	{
		assert( position.x < m_size.x );
		assert( position.y < m_size.y );
		m_fbo->bind();
		camera.viewport().apply();
		m_fbo->clear( gl::RgbaColour{ 0, 0, 0, 1 } );
		m_renderer.draw( camera
			, zoomPercent
			, objects
			, billboards );
		m_fbo->unbind();
		memset( m_buffer.data(), 0xFF, m_buffer.size() * sizeof( Pixel ) );
		gl::Offset2D offset
		{
			m_size.x - position.x - PickingOffset,
			m_size.y - position.y - PickingOffset
		};
		m_fbo->bind();
		m_fbo->download( uint32_t( offset.x )
			, uint32_t( offset.y )
			, PickingWidth
			, PickingWidth
			, gl::PixelFormat::eR8G8B8A8
			, reinterpret_cast< uint8_t * >( m_buffer.data() ) );
		m_fbo->unbind();

		return m_buffer[( PickingOffset * PickingWidth ) + PickingOffset - 1];
	}

	Picking::NodeType Picking::doPick( Pixel const & pixel
		, RenderSubmeshArray const & objects
		, RenderBillboardArray const & billboards )const
	{
		NodeType ret{ NodeType::eNone };

		if ( pixel.r > 0 || pixel.g > 0 || pixel.b > 0 )
		{
			if ( uint32_t( pixel.r ) & ObjectMask )
			{
				Object * object{ nullptr };
				doPickFromList( objects
					, pixel
					, object );
				onObjectPicked( *object );
			}
			else if ( uint32_t( pixel.r ) & BillboardMask )
			{
				Billboard * billboard{ nullptr };
				doPickFromList( billboards
					, pixel
					, billboard );
				onBillboardPicked( *billboard, uint32_t( pixel.a ) );
			}
		}

		return ret;
	}
}
