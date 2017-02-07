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
		glm::vec3 doConvertSphericalToCartesian( glm::vec2 const & coord
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

		render::MaterialPtr pickedMat = scene.getMaterials().addElement
			( "picked" );
		pickedMat->setOpacityMap( m_opacity );
		pickedMat->setAmbient( glm::vec3{ 0.0, 0.0, 0.5 } );
		pickedMat->setDiffuse( glm::vec3{ 0.0, 0.0, 0.5 } );
		auto pickedBuffers = scene.getBillboardsBuffers().addElement
			( "picked" );
		pickedBuffers->add( { glm::vec3{ 0, 0, 0 }, glm::vec2{ 1, 1 } } );
		m_picked = std::make_shared< render::Billboard >( "picked"
			, *pickedBuffers );
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

		for ( auto & holder : m_state.m_holders )
		{
			doInitialiseHolder( holder );
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
		m_window.reset();

		for ( auto & holder : m_state.m_holders )
		{
			holder.m_initialised = false;
		}
	}

	void StarMap::beginFrame()
	{
		m_debug.start();
	}

	void StarMap::drawFrame()
	{
		m_state.m_cameraState.update();
		m_window->getScene().getCamera().setOrientation( glm::quat{
			{ m_state.m_cameraState.getAngle(), 0.0f } } );
		m_window->getScene().getCamera().moveTo( { 0
			, 0
			, m_state.m_cameraState.getZoom() } );
		m_window->update();
		m_window->draw();
	}

	void StarMap::endFrame()
	{
		m_debug.end();
	}

	void StarMap::add( Star const & star )
	{
		auto & holder = doFindHolder( star.getColour() );
		auto position = doConvertSphericalToCartesian( star.getPosition() );
		holder.m_buffer->add( { position
			, glm::vec2{ star.getMagnitude(), star.getMagnitude() } } );

		if ( m_window )
		{
			doInitialiseHolder( holder );
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
		auto it = std::find_if( std::begin( m_state.m_holders )
			, std::end( m_state.m_holders )
			, [&colour]( StarHolder const & holder )
			{
				return holder.m_colour == colour;
			} );

		if ( it == std::end( m_state.m_holders ) )
		{
			m_state.m_holders.emplace_back( colour
				, render::BillboardBuffer::create() );
			it = m_state.m_holders.begin() + m_state.m_holders.size() - 1;
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

			holder.m_initialised = true;
		}
	}
}
