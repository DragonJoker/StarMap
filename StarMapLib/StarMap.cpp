#include "StarMap.h"

#include "ScreenEvents.h"

#include <RenderLib/Billboard.h>
#include <RenderLib/Font.h>
#include <RenderLib/FontLoader.h>
#include <RenderLib/Object.h>
#include <RenderLib/PolyLine.h>

namespace starmap
{
	namespace
	{
		glm::vec3 doSphericalToCartesian( glm::vec2 const & coord
			, float distance = 100.0f )
		{
			auto const sint = sin( coord.x );
			auto const cost = cos( coord.x );
			auto const sinp = sin( coord.y );
			auto const cosp = cos( coord.y );
			return glm::vec3
			{
				distance * sinp * cost,
				distance * sinp * sint,
				distance * cosp
			};
		}
	}

	StarMap::StarMap( ScreenEvents & events )
		: m_debug{ true }
		, m_onPick{ events.onPick.connect
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
			m_state.m_cameraState.reset();
		} ) }
		, m_onSetVelocity{ events.onSetVelocity.connect
		( [this]( glm::ivec2 const & value )
		{
			m_state.m_cameraState.setVelocity( value );
		} ) }
		, m_onSetZoomVelocity{ events.onSetZoomVelocity.connect
		( [this]( float value )
		{
			m_state.m_cameraState.setZoomVelocity( value );
		} ) }
	{
		events.setStarMap( this );
	}

	void StarMap::restore( StarMapState const & state )
	{
		m_state = state;
	}

	void StarMap::save( StarMapState & state )
	{
		state = m_state;
	}

	void StarMap::initialise( glm::ivec2 const & size
		, render::ByteArray const & opacityMap
		, render::FontLoader const & loader )
	{
		auto width = size.x;
		auto height = size.y;
		float ratio = float( width ) / height;

		// Initialise OpenGL
		gl::OpenGL::initialise();

		// Initialise the render window
		m_window = std::make_unique< render::RenderWindow >( size );
		auto & scene = m_window->getScene();
		m_debug.initialise( scene );

		// Initialise the scene
		scene.setBackgroundColour( glm::vec4{ 0, 0, 0, 1 } );

		doLoadFontTexture( loader );
		doLoadOpacityMap( opacityMap );
		doInitialisePicked();
		doInitialiseLines();

		for ( auto & star : m_stars )
		{
			doAddStar( star );
		}

		for ( auto & constellation : m_constellations )
		{
			doAddConstellation( constellation.second );
		}

		auto & picking = m_window->getPicking();
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
		m_picked.reset();
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
		m_state.m_cameraState.update();
		auto & camera = m_window->getScene().getCamera();
		camera.setOrientation( glm::quat{
			{ m_state.m_cameraState.getAngle(), 0.0f } } );
		camera.setFovY( m_state.m_cameraState.getZoom() );
		m_window->update();
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
		doUpdatePicked( billboard );
	}

	void StarMap::onUnpick()
	{
		m_picked->show( false );
	}

	void StarMap::doUpdatePicked( render::Movable const & movable )
	{
		m_picked->moveTo( movable.getPosition() - glm::vec3{ 0, 0, 3 } );
		m_picked->setScale( movable.getScale() );
		m_picked->show( true );
	}

	void StarMap::doUpdatePicked( render::Object const & object )
	{
		doUpdatePicked( static_cast< render::Movable const & >( object ) );
		m_picked->scale( object.getBoundaries() );
	}

	void StarMap::doUpdatePicked( render::Billboard const & billboard
		, uint32_t index )
	{
		auto data = billboard.getBuffer()[index];
		auto pos = data.center;
		auto transform = billboard.getTransform();
		pos = glm::vec3{ transform * glm::vec4{ pos, 1.0 } };
		m_picked->translate( pos );
		doUpdatePicked( static_cast< render::Movable const & >( billboard ) );
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
			auto & scene = m_window->getScene();
			auto starsMat = scene.getMaterials().addElement( sstars );
			starsMat->setOpacityMap( m_opacity );
			starsMat->setAmbient( holder.m_colour );
			starsMat->setDiffuse( holder.m_colour );
			starsMat->setEmissive( holder.m_colour );
			starsMat->setAlphaTest( true );
			auto halosMat = scene.getMaterials().addElement( shalos );
			halosMat->setOpacityMap( m_opacity );
			halosMat->setAmbient( holder.m_colour );
			halosMat->setDiffuse( holder.m_colour );
			halosMat->setEmissive( holder.m_colour );

			auto stars = std::make_shared< render::Billboard >( sstars
				, *holder.m_buffer );
			stars->setDimensions( glm::vec2{ 1, 1 } );
			stars->setMaterial( starsMat );
			scene.add( stars, true );

			auto halos = std::make_shared< render::Billboard >( shalos
				, *holder.m_buffer );
			halos->setDimensions( glm::vec2{ 2, 2 } );
			halos->setMaterial( halosMat );
			scene.add( halos, false );

			scene.addBillboardBuffer( sstars, holder.m_buffer );

			holder.m_initialised = true;
		}
	}

	void StarMap::doAddStar( Star const & star )
	{
		auto & holder = doFindHolder( star.getColour() );
		auto position = doSphericalToCartesian( star.getPosition() );
		holder.m_buffer->add( { star.getMagnitude()
			, position
			, glm::vec2{ 1.0, 1.0 } } );
		doInitialiseHolder( holder );
	}

	void StarMap::doAddConstellation( Constellation const & constellation )
	{
		for ( auto & link : constellation )
		{
			m_lines->add( { doSphericalToCartesian( link.m_a->getPosition() )
				, doSphericalToCartesian( link.m_b->getPosition() ) } );
		}
	}

	void StarMap::doLoadFontTexture( render::FontLoader const & loader )
	{
		// Load the font's texture.
		render::FontPtr font = std::make_unique< render::Font >( "Arial"
			, loader.getHeight() );
		render::loadFont( loader, *font );
		m_window->setFontTexture( std::make_unique< render::FontTexture >
			( std::move( font ) ) );
	}

	void StarMap::doLoadOpacityMap( render::ByteArray const & opacityMap )
	{
		auto & scene = m_window->getScene();
		m_opacity = scene.getTextures().getElement( "halo.bmp" );

		if ( !m_opacity )
		{
			m_opacity = scene.getTextures().addElement( "halo.bmp" );
			render::loadTexture( opacityMap, *m_opacity );
		}
	}

	void StarMap::doInitialisePicked()
	{
		auto & scene = m_window->getScene();
		render::MaterialPtr pickedMat = scene.getMaterials().addElement
		( "picked" );
		pickedMat->setOpacityMap( m_opacity );
		pickedMat->setAmbient( glm::vec3{ 0.0, 0.0, 0.5 } );
		pickedMat->setDiffuse( glm::vec3{ 0.0, 0.0, 0.5 } );
		auto pickedBuffers = render::BillboardBuffer::create();
		pickedBuffers->add( { 1000.0
			, glm::vec3{ 0, 0, 0 }
		, glm::vec2{ 1, 1 } } );
		scene.addBillboardBuffer( "picked", pickedBuffers );
		m_picked = std::make_shared< render::Billboard >( "picked"
			, *pickedBuffers );
		m_picked->setMaterial( pickedMat );
		m_picked->show( false );
		scene.add( m_picked, false );
	}

	void StarMap::doInitialiseLines()
	{
		auto & scene = m_window->getScene();
		render::MaterialPtr linesMat = scene.getMaterials().addElement( "lines" );
		linesMat->setAmbient( glm::vec3{ 1.0, 1.0, 0.5 } );
		linesMat->setDiffuse( glm::vec3{ 1.0, 1.0, 0.5 } );
		linesMat->setEmissive( glm::vec3{ 1.0, 1.0, 0.5 } );

		m_lines = std::make_shared< render::PolyLine >( "lines", 0.1f );
		m_lines->setMaterial( linesMat );
		scene.add( m_lines );
	}
}
