#include "StarMap.h"

#include "ScreenEvents.h"

#include <RenderLib/Billboard.h>
#include <RenderLib/Font.h>
#include <RenderLib/FontLoader.h>
#include <RenderLib/Object.h>
#include <RenderLib/PolyLine.h>

#include <GlLib/Transform.h>

namespace starmap
{
	namespace
	{
		static const gl::RgbaColour ConstellationColour{ 0.8, 0.5, 0.8, 0.5 };
		static const gl::RgbaColour ConstellationTextColour{ 0.5, 0.0, 0.5, 1.0 };
		static const gl::RgbColour PickBillboardColour{ 0.0, 0.0, 0.5 };
		static const gl::RgbaColour PickDescriptionColour{ 1.0, 1.0, 1.0, 1.0 };
		static const gl::Offset2D StarNameOffset{ 50, 50 };
		static const gl::Offset2D ConstellationNameOffset{};
	}

	StarMap::StarMap( ScreenEvents & events )
		: m_onPick{ events.onPick.connect
		( [this]( gl::Position2D const & coord )
		{
			if ( m_pickedStar )
			{
				onUnpick();
			}
			else if ( m_window )
			{
				m_window->pick( coord );
			}
		} ) }
		, m_onReset{ events.onReset.connect
		( [this]()
		{
			m_window->state().reset();
		} ) }
		, m_onSetVelocity{ events.onSetVelocity.connect
		( [this]( gl::Offset2D const & value )
		{
			m_window->state().velocity( value );
		} ) }
		, m_onSetZoomVelocity{ events.onSetZoomVelocity.connect
		( [this]( float value )
		{
			m_window->state().zoomVelocity( value );
		} ) }
	{
		events.starMap( this );
	}

	void StarMap::restore( render::CameraState const & state )
	{
		m_window->state() = state;
	}

	void StarMap::save( render::CameraState & state )
	{
		state = m_window->state();
	}

	void StarMap::initialise( gl::Size2D const & size
		, render::ByteArray const & opacityMap
		, render::FontLoader & loader )
	{
		// Initialise OpenGL
		gl::OpenGL::initialise();

		// Initialise the render window
		m_window = std::make_unique< render::RenderWindow >( size
			, loader
			, true );
		doLoadFontTextures( loader );
		doLoadOpacityMap( opacityMap );

		auto & scene = m_window->scene();

		// Initialise the scene
		scene.backgroundColour( gl::RgbaColour{ 0, 0, 0, 1 } );

		doInitialisePickObjects();
		doInitialiseLines();

		if ( !m_stars.empty() )
		{
			auto minMagnitude = m_stars.begin()->magnitude();
			auto maxMagnitude = m_stars.rbegin()->magnitude();
			m_window->scene().thresholdBounds( minMagnitude, maxMagnitude );

			for ( auto & star : m_stars )
			{
				doAddStar( star, m_window->scene().thresholdBounds() );
			}

			for ( auto & constellation : m_constellations )
			{
				doAddConstellation( constellation.second );
			}

			//doInitialiseStarNames();
			doInitialiseConstellationNames();
		}

		auto & picking = m_window->picking();
		m_onObjectPicked = picking.onObjectPicked.connect
			( std::bind( &StarMap::onObjectPicked
				, this
				, std::placeholders::_1 ) );
		m_onBillboardPicked = picking.onBillboardPicked.connect
			( std::bind( &StarMap::onBillboardPicked
				, this
				, std::placeholders::_1
				, std::placeholders::_2 ) );
		m_onUnpick = picking.onUnpick.connect
			( std::bind( &StarMap::onUnpick
				, this ) );
	}

	void StarMap::cleanup()
	{
		onUnpick();
		m_pickDescription.reset();
		m_pickBillboard.reset();
		m_onObjectPicked.disconnect();
		m_onBillboardPicked.disconnect();
		m_onUnpick.disconnect();
		m_holders.clear();
		m_lines.reset();
		m_window.reset();
	}

	void StarMap::resize( gl::Size2D const & size )
	{
		if ( m_window )
		{
			m_window->resize( size );
		}
	}

	void StarMap::beginFrame()
	{
		m_window->beginFrame();
	}

	void StarMap::drawFrame()
	{
		m_window->update();
		doUpdatePickDescription();
		//doUpdateStarNames();
		doUpdateConstellationNames();
		m_window->updateOverlays();
		m_window->draw();
	}

	void StarMap::endFrame()
	{
		m_window->endFrame();
	}

	void StarMap::add( Star const & star )
	{
		m_stars.insert( star );
	}

	void StarMap::add( StarArray const & stars )
	{
		for ( auto & star : stars )
		{
			add( star );
		}
	}

	Constellation & StarMap::createConstellation( std::string const & name )
	{
		auto it = m_constellations.find( name );

		if ( it == m_constellations.end() )
		{
			it = m_constellations.insert( { name, { name, m_stars } } ).first;
		}

		return it->second;
	}

	void StarMap::filter( ElementType type, bool show )
	{
		switch ( type )
		{
		case ElementType::eConstellation:
			doFilterConstellations( show );
			break;

		case ElementType::eStar:
			doFilterStars( show );
			break;
		}
	}

	void StarMap::onObjectPicked( render::Object & object )
	{
		m_pickedObject = &object;
		doUpdatePicked( object );
	}

	void StarMap::onBillboardPicked( render::Billboard & billboard
		, uint32_t index )
	{
		m_pickedBillboard = &billboard;
		doUpdatePicked( billboard, index );
	}

	void StarMap::onUnpick()
	{
		m_pickBillboard->show( false );
		m_pickDescription->show( false );
		m_pickedStar = nullptr;
	}

	void StarMap::doUpdatePicked( render::Movable const & movable )
	{
		m_pickBillboard->rescale( movable.scale() );
		m_pickBillboard->reorient( movable.orientation() );
		m_pickBillboard->update();
		m_pickBillboard->show( true );

		m_pickDescription->update();
		m_pickDescription->show( true );
	}

	void StarMap::doUpdatePicked( render::Object const & object )
	{
		m_pickDescription->caption( object.name() );
		doUpdateOverlay( *m_pickDescription
			, object.position()
			, StarNameOffset );

		m_pickBillboard->moveTo( object.position()
			- gl::Vector3D{ 0, 0, object.boundaries().z + 0.2 } );
		doUpdatePicked( static_cast< render::Movable const & >( object ) );
		m_pickBillboard->dimensions( gl::Size2D{ gl::toVec2( object.boundaries() * 2.0f ) } );
		m_pickBillboard->buffer().at( 0u
			, { -1000.0f, gl::Vector3D{ 0, 0, 0 }, gl::Vector2D{ 1, 1 } } );
		m_pickBillboard->buffer().upload();
	}

	void StarMap::doUpdatePicked( render::Billboard const & billboard
		, uint32_t index )
	{
		auto & holder = doFindHolder( billboard.material().ambient() );

		if ( holder.m_stars.size() > index )
		{
			auto & star = *holder.m_stars[index];
			m_pickedStar = &star;
			m_pickDescription->caption( star.name() );
			doUpdatePickDescription();
			m_pickBillboard->moveTo( billboard.position() - gl::Vector3D{ 0, 0, 0.2 } );
			doUpdatePicked( static_cast< render::Movable const & >( billboard ) );
			auto & data = billboard.buffer()[index];
			auto scale = 0.1f + m_window->state().zoomBounds().percent( m_window->state().zoom() );
			m_pickBillboard->buffer().at( 0u
				, { -1000.0f, data.center, gl::Vector2D{ scale, scale } } );
			m_pickBillboard->buffer().upload();
		}
	}

	StarHolder & StarMap::doFindHolder( gl::RgbColour const & colour )
	{
		auto it = std::find_if( std::begin( m_holders )
			, std::end( m_holders )
			, [&colour]( StarHolder const & holder )
			{
				return holder.m_colour == colour;
			} );

		if ( it == std::end( m_holders ) )
		{
			m_holders.emplace_back( colour
				, std::make_shared< render::BillboardBuffer >( true ) );
			it = m_holders.begin() + m_holders.size() - 1;
		}

		return *it;
	}

	void StarMap::doInitialiseHolder( StarHolder & holder )
	{
		if ( !holder.m_initialised )
		{
			using render::operator<<;
			std::stringstream scolour;
			scolour << holder.m_colour;
			std::string const shalos = "halos_" + scolour.str();
			std::string const sstars = "stars_" + scolour.str();
			auto & scene = m_window->scene();
			auto starsMat = scene.materials().addElement( sstars );
			starsMat->opacityMap( m_opacity );
			starsMat->ambient( holder.m_colour );
			starsMat->diffuse( holder.m_colour );
			starsMat->emissive( holder.m_colour );
			starsMat->alphaTest( true );
			auto halosMat = scene.materials().addElement( shalos );
			halosMat->opacityMap( m_opacity );
			halosMat->ambient( holder.m_colour );
			halosMat->diffuse( holder.m_colour );
			halosMat->emissive( holder.m_colour );

			auto stars = std::make_shared< render::Billboard >( sstars
				, *holder.m_buffer );
			stars->dimensions( gl::Size2D{ 1, 1 } );
			stars->material( starsMat );
			scene.add( stars );

			auto halos = std::make_shared< render::Billboard >( shalos
				, *holder.m_buffer );
			halos->dimensions( gl::Size2D{ 2, 2 } );
			halos->material( halosMat );
			scene.add( halos );

			scene.addBillboardBuffer( sstars, holder.m_buffer );

			holder.m_initialised = true;
		}
	}

	void StarMap::doAddStar( Star const & star
		, render::Range< float > const & range )
	{
		auto scale = 0.1f + range.invpercent( star.magnitude() );
		auto & holder = doFindHolder( star.colour() );
		holder.m_stars.push_back( &star );
		holder.m_buffer->add( { star.magnitude()
			, star.position()
			, gl::Vector2D{ scale, scale } } );
		doInitialiseHolder( holder );
	}

	void StarMap::doAddConstellation( Constellation & constellation )
	{
		gl::Vector3D position;

		for ( auto & star : constellation.stars() )
		{
			position += star.second->position();
		}

		constellation.position( position / float( constellation.stars().size() ) );

		for ( auto & link : constellation.links() )
		{
			m_lines->add( { link.m_a->position()
				, link.m_b->position() } );
		}
	}

	void StarMap::doLoadFontTextures( render::FontLoader & loader )
	{
		render::FontPtr font = std::make_unique< render::Font >( "Arial"
			, 24 );
		render::loadFont( loader, *font );
		m_fontTextureNames = std::make_unique< render::FontTexture >
			( std::move( font ) );
	}

	void StarMap::doLoadOpacityMap( render::ByteArray const & opacityMap )
	{
		auto & scene = m_window->scene();
		m_opacity = scene.textures().findElement( "halo.bmp" );

		if ( !m_opacity )
		{
			m_opacity = scene.textures().addElement( "halo.bmp" );
			render::loadTexture( opacityMap, *m_opacity );
		}
	}

	void StarMap::doInitialisePickObjects()
	{
		auto & scene = m_window->scene();
		render::MaterialPtr pickedMat = scene.materials().addElement
		( "picked" );
		pickedMat->opacityMap( m_opacity );
		pickedMat->ambient( PickBillboardColour );
		pickedMat->diffuse( PickBillboardColour );
		auto pickedBuffers = std::make_shared< render::BillboardBuffer >( false );
		pickedBuffers->add( { -1000.0
			, gl::Vector3D{ 0, 0, 0 }
			, gl::Vector2D{ 1, 1 } } );
		scene.addBillboardBuffer( "picked", pickedBuffers );
		m_pickBillboard = std::make_shared< render::Billboard >( "picked"
			, *pickedBuffers );
		m_pickBillboard->material( pickedMat );
		m_pickBillboard->dimensions( { 30, 30 } );
		m_pickBillboard->show( false );
		scene.add( m_pickBillboard );

		m_pickDescription = scene.overlays().addElement( "picked" );
		m_pickDescription->fontTexture( *m_fontTextureNames );
		m_pickDescription->show( false );
		m_pickDescription->colour( PickDescriptionColour );
	}

	void StarMap::doInitialiseLines()
	{
		auto & scene = m_window->scene();
		render::MaterialPtr linesMat = scene.materials().addElement( "lines" );
		linesMat->ambient( gl::toVec3( ConstellationColour ) );
		linesMat->diffuse( gl::toVec3( ConstellationColour ) );
		linesMat->emissive( gl::toVec3( ConstellationColour ) );

		m_lines = std::make_shared< render::PolyLine >( "lines", 0.06f, 1.0f );
		m_lines->material( linesMat );
		scene.add( m_lines );
	}

	void StarMap::doInitialiseStarNames()
	{
		auto & scene = m_window->scene();

		for ( auto const & star : m_stars )
		{
			auto overlay = scene.overlays().addElement( star.name() );
			overlay->caption( star.name() );
			overlay->colour( gl::RgbaColour{ 1.0, 1.0, 1.0, 1.0 } );
			overlay->fontTexture( *m_fontTextureNames );
			overlay->show( false );
		}
	}

	void StarMap::doInitialiseConstellationNames()
	{
		auto & scene = m_window->scene();

		for ( auto const & constellation : m_constellations )
		{
			auto overlay = scene.overlays().addElement( constellation.first );
			overlay->caption( constellation.first );
			overlay->colour( ConstellationTextColour );
			overlay->fontTexture( *m_fontTextureNames );
		}
	}

	void StarMap::doUpdatePickDescription()
	{
		if ( m_pickedStar )
		{
			doUpdateOverlay( *m_pickDescription
				, m_pickedStar->position()
				, StarNameOffset );
		}
	}

	void StarMap::doUpdateStarNames()
	{
		auto & scene = m_window->scene();

		for ( auto & star : m_stars )
		{
			auto overlay = scene.overlays().findElement( star.name() );
			doUpdateOverlay( *overlay
				, star.position()
				, StarNameOffset );
		}
	}

	void StarMap::doUpdateConstellationNames()
	{
		auto & scene = m_window->scene();

		for ( auto const & constellation : m_constellations )
		{
			auto overlay = scene.overlays().findElement( constellation.first );
			doUpdateOverlay( *overlay
				, constellation.second.position()
				, ConstellationNameOffset );
		}
	}

	void StarMap::doUpdateOverlay( render::TextOverlay & overlay
		, gl::Vector3D const & position
		, gl::Offset2D const & offset )
	{
		auto & scene = m_window->scene();
		auto const & camera = scene.camera();
		auto const & viewport = camera.viewport();
		auto const & projection = camera.projection();
		auto const & view = camera.view();
		auto projected = gl::project( position
			, view
			, projection
			, gl::Vector4D{ 0, 0, viewport.size().x, viewport.size().y } );
		overlay.position( offset + gl::Offset2D{ viewport.size().x - projected.x
			, viewport.size().y - projected.y } );
	}

	void StarMap::doFilterConstellations( bool show )
	{
		auto & scene = m_window->scene();
		m_lines->show( show );

		for ( auto const & constellation : m_constellations )
		{
			auto overlay = scene.overlays().findElement( constellation.first );
			overlay->show( show );
		}
	}

	void StarMap::doFilterStars( bool show )
	{
		for ( auto & holder : m_holders )
		{
			auto it = std::find_if( m_window->scene().billboards().begin()
				, m_window->scene().billboards().end()
				, [&holder]( render::BillboardPtr & billboard )
			{
				return &billboard->buffer() == holder.m_buffer.get();
			} );
			while ( it != m_window->scene().billboards().end() )
			{
				( *it )->show( show );
				++it;
				it = std::find_if( it
					, m_window->scene().billboards().end()
					, [&holder]( render::BillboardPtr & billboard )
				{
					return &billboard->buffer() == holder.m_buffer.get();
				} );
			}
		}
	}
}
