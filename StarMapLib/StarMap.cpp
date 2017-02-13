#include "StarMap.h"

#include "ScreenEvents.h"

#include <RenderLib/Billboard.h>
#include <RenderLib/Font.h>
#include <RenderLib/FontLoader.h>
#include <RenderLib/Object.h>
#include <RenderLib/PolyLine.h>

#include <GlLib/glm/gtc/matrix_transform.hpp>

namespace starmap
{
	namespace
	{
		static const glm::vec4 ConstellationColour{ 0.8, 0.5, 0.8, 1.0 };
		static const glm::vec4 ConstellationTextColour{ 0.5, 0.0, 0.5, 1.0 };
		static const glm::vec3 PickBillboardColour{ 0.0, 0.0, 0.5 };
		static const glm::vec4 PickDescriptionColour{ 1.0, 1.0, 1.0, 1.0 };
		static const glm::ivec2 StarNameOffset{ 50, 50 };
		static const glm::ivec2 ConstellationNameOffset{};
	}

	StarMap::StarMap( ScreenEvents & events )
		: m_onPick{ events.onPick.connect
		( [this]( glm::ivec2 const & coord )
		{
			if ( m_window )
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
		( [this]( glm::ivec2 const & value )
		{
			m_window->state().velocity( value );
		} ) }
		, m_onSetZoomVelocity{ events.onSetZoomVelocity.connect
		( [this]( float value )
		{
			m_window->state().zoomVelocity( value );
		} ) }
		, m_debug{ true }
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

	void StarMap::initialise( glm::ivec2 const & size
		, render::ByteArray const & opacityMap
		, render::FontLoader & loader )
	{
		// Initialise OpenGL
		gl::OpenGL::initialise();

		// Initialise the render window
		m_window = std::make_unique< render::RenderWindow >( size );
		doLoadFontTextures( loader );
		doLoadOpacityMap( opacityMap );

		auto & scene = m_window->scene();
		m_debug.initialise( scene, loader );

		// Initialise the scene
		scene.backgroundColour( glm::vec4{ 0, 0, 0, 1 } );

		doInitialisePickObjects();
		doInitialiseLines();

		if ( !m_stars.empty() )
		{
			auto minMagnitude = m_stars.begin()->magnitude();
			auto maxMagnitude = m_stars.rbegin()->magnitude();

			for ( auto & star : m_stars )
			{
				doAddStar( star, minMagnitude, maxMagnitude );
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

	void StarMap::resize( glm::ivec2 const & size )
	{
		if ( m_window )
		{
			m_window->resize( size );
		}
	}

	void StarMap::beginFrame()
	{
		m_debug.start();
	}

	void StarMap::drawFrame()
	{
		m_window->update();
		//doUpdateStarNames();
		doUpdateConstellationNames();
		m_window->draw();
	}

	void StarMap::endFrame()
	{
		m_debug.end();
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

		m_pickBillboard->moveTo( object.position() - glm::vec3{ 0, 0, object.boundaries().z + 0.2 } );
		doUpdatePicked( static_cast< render::Movable const & >( object ) );
		m_pickBillboard->dimensions( object.boundaries() * 2.0f );
		m_pickBillboard->buffer().at( 0u, { -1000.0f, glm::vec3{ 0, 0, 0 }, glm::vec2{ 1, 1 } } );
	}

	void StarMap::doUpdatePicked( render::Billboard const & billboard
		, uint32_t index )
	{
		auto & holder = doFindHolder( billboard.material().ambient() );

		if ( holder.m_stars.size() > index )
		{
			auto & star = *holder.m_stars[index];
			m_pickDescription->caption( star.name() );
			doUpdateOverlay( *m_pickDescription
				, star.position()
				, StarNameOffset );

			m_pickBillboard->moveTo( billboard.position() - glm::vec3{ 0, 0, 0.2 } );
			doUpdatePicked( static_cast< render::Movable const & >( billboard ) );
			auto data = billboard.buffer()[index];
			m_pickBillboard->buffer().at( 0u, { -1000.0f, data.center, glm::vec2{ 1, 1 } } );
		}
	}

	StarHolder & StarMap::doFindHolder( glm::vec3 const & colour )
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
				, render::BillboardBuffer::create() );
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
			stars->dimensions( glm::vec2{ 1, 1 } );
			stars->material( starsMat );
			scene.add( stars );

			auto halos = std::make_shared< render::Billboard >( shalos
				, *holder.m_buffer );
			halos->dimensions( glm::vec2{ 2, 2 } );
			halos->material( halosMat );
			scene.add( halos );

			scene.addBillboardBuffer( sstars, holder.m_buffer );

			holder.m_initialised = true;
		}
	}

	void StarMap::doAddStar( Star const & star
		, float minMagnitude
		, float maxMagnitude )
	{
		auto range = maxMagnitude - minMagnitude;
		auto percent = ( maxMagnitude - star.magnitude() ) / range;
		auto scale = 2.0f * percent;
		auto & holder = doFindHolder( star.colour() );
		holder.m_stars.push_back( &star );
		holder.m_buffer->add( { star.magnitude()
			, star.position()
			, glm::vec2{ scale, scale } } );
		doInitialiseHolder( holder );
	}

	void StarMap::doAddConstellation( Constellation & constellation )
	{
		glm::vec3 position;

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
		// Load the font's texture.
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
		auto pickedBuffers = render::BillboardBuffer::create();
		pickedBuffers->add( { -1000.0
			, glm::vec3{ 0, 0, 0 }
			, glm::vec2{ 1, 1 } } );
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
		linesMat->ambient( ConstellationColour );
		linesMat->diffuse( ConstellationColour );
		linesMat->emissive( ConstellationColour );

		m_lines = std::make_shared< render::PolyLine >( "lines", 0.1f );
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
			overlay->colour( glm::vec4{ 1.0, 1.0, 1.0, 1.0 } );
			overlay->fontTexture( *m_fontTextureNames );
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
		, glm::vec3 const & position
		, glm::ivec2 const & offset )
	{
		auto & scene = m_window->scene();
		auto const & camera = scene.camera();
		auto const & viewport = camera.viewport();
		auto const & projection = camera.projection();
		auto const & view = camera.view();
		auto projected = glm::project( position
			, view
			, projection
			, glm::vec4{ 0, 0, viewport.size().x, viewport.size().y } );
		overlay.position( offset + glm::ivec2{ viewport.size().x - projected.x
			, viewport.size().y - projected.y } );
	}
}
