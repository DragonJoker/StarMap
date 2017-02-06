#include "StarMap.h"

#include <RenderLib/Billboard.h>
#include <RenderLib/Font.h>
#include <RenderLib/FontLoader.h>
#include <RenderLib/Object.h>

#include "ScreenEvents.h"

namespace starmap
{
	namespace
	{
		glm::vec3 doConvertSphericalToCartesian( glm::vec2 const & coord, float distance = 100.0f )
		{
			auto sint = sin( coord.x );
			auto cost = cos( coord.x );
			auto sinp = sin( coord.y );
			auto cosp = cos( coord.y );
			return glm::vec3
			{
				distance * sint * cosp,
				distance * sint * sinp,
				distance * cost
			};
		}
	}

	StarMap::StarMap( ScreenEvents & events )
		: m_debug{ true }
	{
		events.setStarMap( this );
		m_onPick = events.onPick.connect( [this]( glm::ivec2 const & coord )
		{
			if ( m_window )
			{
				m_window->pick( coord );
			}
		} );
		m_onReset = events.onReset.connect( [this]()
		{
			m_state.m_cameraState.reset();
		} );
		m_onSetVelocity = events.onSetVelocity.connect( [this]( glm::ivec2 const & value )
		{
			m_state.m_cameraState.setVelocity( value );
		} );
		m_onSetZoomVelocity = events.onSetZoomVelocity.connect( [this]( float value )
		{
			m_state.m_cameraState.setZoomVelocity( value );
		} );
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

		m_window->getViewport().setPerspective( 45.0f
			, ratio
			, 0.1f
			, 200.0f );
		m_window->getViewport().setOrtho( ratio * -10.0f
			, ratio * 10.0f
			, -10.0f
			, 10.0f
			, -200.0f
			, 200.0f );
		m_window->getViewport().setInfinitePerspective( 45.0f
			, ratio
			, 0.1f );

		auto & scene = m_window->getScene();
		m_debug.initialise( scene );

		// Initialise the scene
		scene.setBackgroundColour( glm::vec4{ 0.5, 0.5, 0.5, 1.0 } );

		// Load the font's texture.
		render::FontPtr font = std::make_unique< render::Font >( "Arial"
			, loader.getHeight() );
		render::loadFont( loader, *font );
		m_window->setFontTexture( std::make_unique< render::FontTexture >
			( std::move( font ) ) );

		// Populate the scene
		m_opacity = scene.getTextures().getElement( "halo.bmp" );

		if ( !m_opacity )
		{
			m_opacity = scene.getTextures().addElement( "halo.bmp" );
			render::loadTexture( opacityMap, *m_opacity );
		}

		render::MaterialPtr pickedMat = scene.getMaterials().addElement( "picked" );
		pickedMat->setOpacityMap( m_opacity );
		pickedMat->setAmbient( glm::vec3{ 0.0, 0.0, 0.5 } );
		pickedMat->setDiffuse( glm::vec3{ 0.0, 0.0, 0.5 } );
		m_picked = std::make_shared< render::Billboard >( "picked" );
		m_picked->add( { glm::vec3{ 0, 0, 0 }, glm::vec2{ 1, 1 } } );
		m_picked->setMaterial( pickedMat );
		m_picked->show( false );
		scene.add( m_picked, false );

		//render::MaterialPtr linesMat = scene.getMaterials().addElement( "lines" );
		//linesMat->setAmbient( glm::vec3{ 1.0, 1.0, 0.5 } );
		//linesMat->setDiffuse( glm::vec3{ 1.0, 1.0, 0.5 } );
		//linesMat->setEmissive( glm::vec3{ 1.0, 1.0, 0.5 } );

		//render::PolyLinePtr lines = std::make_shared< render::PolyLine >( "lines" );
		//lines->add( { glm::vec3{ -1, 1, 0 }, glm::vec3{ 1, 1, 0 } } );
		//lines->add( { glm::vec3{ 1, 1, 0 }, glm::vec3{ 1, -1, 0 } } );
		//lines->add( { glm::vec3{ 1, -1, 0 }, glm::vec3{ -1, -1, 0 } } );
		//lines->add( { glm::vec3{ -1, -1, 0 }, glm::vec3{ -1, 1, 0 } } );
		//lines->moveTo( glm::vec3{ 0, 0, 8 } );
		//lines->setMaterial( linesMat );
		//scene.add( lines );

		for ( auto & star : m_state.m_stars )
		{
			doAdd( star );
		}

		m_onObjectPicked = m_window->getPicking().onObjectPicked.connect
			( std::bind( &StarMap::onObjectPicked
				, this
				, std::placeholders::_1 ) );
		m_onBillboardPicked = m_window->getPicking().onBillboardPicked.connect
			( std::bind( &StarMap::onBillboardPicked
				, this
				, std::placeholders::_1
				, std::placeholders::_2 ) );
		m_onUnpick = m_window->getPicking().onUnpick.connect
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
		m_window.reset();
	}

	void StarMap::beginFrame()
	{
		m_debug.start();
	}

	void StarMap::drawFrame()
	{
		m_state.m_cameraState.update();
		m_window->getScene().getCamera().setOrientation( glm::quat{ glm::vec3{ m_state.m_cameraState.getAngle(), 0.0f } } );
		m_window->getScene().getCamera().moveTo( { 0, 0, m_state.m_cameraState.getZoom() } );
		m_window->update();
		m_window->draw();
	}

	void StarMap::endFrame()
	{
		m_debug.end();
	}

	void StarMap::add( Star const & star )
	{
		m_state.m_stars.push_back( star );

		if ( m_window )
		{
			doAdd( star );
		}
	}

	void StarMap::add( StarArray const & stars )
	{
		for ( auto & star : stars )
		{
			add( star );
		}
	}

	void StarMap::onObjectPicked( render::Object & object )
	{
		m_pickedObject = &object;
		doUpdatePicked( object );
	}

	void StarMap::onBillboardPicked( render::Billboard & billboard, uint32_t index )
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
		auto data = billboard[index];
		auto pos = data.position;
		auto transform = billboard.getTransform();
		pos = glm::vec3{ transform * glm::vec4{ pos, 1.0 } };
		m_picked->translate( pos );
		doUpdatePicked( static_cast< render::Movable const & >( billboard ) );
	}

	void StarMap::doAdd( Star const & star )
	{
		auto & holder = doInitialiseStarsHolder( star.getColour() );
		auto position = doConvertSphericalToCartesian( star.getPosition() );
		holder.m_stars.add( { position, glm::vec2{ star.getMagnitude(), star.getMagnitude() } } );
		holder.m_halos.add( { position, glm::vec2{ star.getMagnitude(), star.getMagnitude() } } );
	}

	StarMap::StarHolder & StarMap::doInitialiseStarsHolder( glm::vec3 const & colour )
	{
		auto it = std::find_if( std::begin( m_holders )
			, std::end( m_holders )
			, [&colour]( StarHolder const & holder )
			{
				return holder.m_colour == colour;
			} );

		if ( it == std::end( m_holders ) )
		{
			using render::operator<<;
			std::stringstream scolour;
			scolour << colour;
			auto & scene = m_window->getScene();
			render::MaterialPtr starsMat = scene.getMaterials().addElement( "stars_" + scolour.str() );
			starsMat->setOpacityMap( m_opacity );
			starsMat->setAmbient( colour );
			starsMat->setDiffuse( colour );
			starsMat->setEmissive( colour );
			starsMat->setAlphaTest( true );

			render::BillboardPtr stars = std::make_shared< render::Billboard >( "stars_" + scolour.str() );
			stars->setDimensions( glm::vec2{ 1, 1 } );
			stars->moveTo( glm::vec3{ 0, 0, 8 } );
			stars->setMaterial( starsMat );
			scene.add( stars, true );

			render::MaterialPtr halosMat = scene.getMaterials().addElement( "halos_" + scolour.str() );
			halosMat->setOpacityMap( m_opacity );
			halosMat->setAmbient( colour );
			halosMat->setDiffuse( colour );
			halosMat->setEmissive( colour );

			render::BillboardPtr halos = std::make_shared< render::Billboard >( "halos_" + scolour.str() );
			halos->setDimensions( glm::vec2{ 2, 2 } );
			halos->moveTo( glm::vec3{ 0, 0, 8 } );
			halos->setMaterial( halosMat );
			scene.add( halos, false );

			m_holders.emplace_back( colour, *stars, *halos );
			it = m_holders.begin() + m_holders.size() - 1;
		}

		return *it;
	}
}
