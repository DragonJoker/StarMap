#include "StarMap.h"

#include "ConstellationStar.h"
#include "ScreenEvents.h"
#include "StarMapState.h"

#include <RenderLib/Billboard.h>
#include <RenderLib/BorderPanelOverlay.h>
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
		static const gl::RgbColour ConstellationTextColour{ 0.5, 0.0, 0.5 };
		static const gl::RgbColour PickBillboardColour{ 0.0, 0.0, 0.5 };
		static const gl::RgbColour PickDescriptionColour{ 1.0, 1.0, 1.0 };
		static const gl::RgbColour PickDescriptionHolderColour{ 0.0, 0.0, 0.0 };
		static const gl::RgbColour PickDescriptionHolderBorderColour{ 1.0, 1.0, 1.0 };
		static const float PickDescriptionHolderOpacity{ 0.5 };
		static const gl::IVec4 PickDescriptionHolderBorderSize{ 3, 3, 3, 3 };
		static const gl::IVec2 StarNameOffset{ 10, 10 };
		static const gl::IVec2 ConstellationNameOffset{};

		String doConvertStdString( std::string const & str )
		{
			String result{ { 0 } };
			assert( str.size() < result.size() - 1 );
			memcpy( result.data(), str.data(), str.size() );
			return result;
		}

		std::string doConvertString( String const & str )
		{
			return std::string{ str.data(), strlen( str.data() ) };
		}

		std::ostream & operator <<( std::ostream & stream, Star const & star )
		{
			stream << star.name() << "\n"
				<< "Magnitude : " << star.magnitude();
			return stream;
		}
	}

	StarMap::StarMap( ScreenEvents & events
		, uint32_t maxDisplayedStarNames
		, gl::IVec2 const & size
		, render::FontLoader & loader
		, render::ByteArray const & opacityMap )
		: m_onPick{ events.onPick.connect( [this]( gl::IVec2 const & coord )
			{
				if ( m_pickedStar )
				{
					onUnpick();
				}
				else
				{
					m_window.pick( coord );
				}
			} ) }
		, m_onReset{ events.onReset.connect( [this]()
			{
				m_window.state().reset();
			} ) }
		, m_onSetVelocity{ events.onSetVelocity.connect( [this]( gl::IVec2 const & value )
			{
				m_window.state().velocity( gl::Vec2{ value } );
			} ) }
		, m_onSetZoomVelocity{ events.onSetZoomVelocity.connect( [this]( float value )
			{
				m_window.state().zoomVelocity( value );
			} ) }
		, m_maxDisplayedStarNames{ maxDisplayedStarNames }
#if 1 || !defined( NDEBUG )
		, m_window{ size, loader, true }
#else
		, m_window{ size, loader, false }
#endif
	{
		doLoadFontTextures( loader );
		doLoadOpacityMap( opacityMap );
		events.starMap( this );
	}

	void StarMap::restore( render::ByteArray const & save )
	{
		auto & state = *reinterpret_cast< StarMapState const *  >( save.data() );
		auto * stars = reinterpret_cast< StarInfo const *  >( save.data()
			+ sizeof( starmap::ConstellationInfoArray )
			+ sizeof( uint32_t )
			+ sizeof( render::CameraState ) );

		std::for_each( stars
			, stars + state.m_count
			, [this]( StarInfo const & star )
			{
				m_stars.emplace_back( doConvertString( star.m_name )
					, star.m_magnitude
					, gl::Vec3{ star.m_position[0], star.m_position[1], star.m_position[2] }
					, gl::RgbColour{ star.m_colour[0], star.m_colour[1], star.m_colour[2] } );
				m_stars.back().index( star.m_index );
			} );

		for ( auto & info : state.m_constellations )
		{
			Constellation constellation{ doConvertString( info.m_name ) };

			for ( auto & star : info.m_stars )
			{
				if ( star.m_index )
				{
					constellation.addStar( star.m_id
						, doConvertString( star.m_letter )
						, m_stars[star.m_index].name() );
				}
			}

			add( constellation );
			auto constell = findConstellation( constellation.name() );

			for ( auto & link : info.m_links )
			{
				if ( link.m_a != link.m_b )
				{
					constell->addLink( doConvertString( link.m_a )
						, doConvertString( link.m_b ) );
				}
			}
		}

		m_window.state() = state.m_state;
		m_sorted = true;
	}

	void StarMap::save( render::ByteArray & save )
	{
		save.resize( m_stars.size() * sizeof( starmap::StarInfo )
			+ sizeof( starmap::ConstellationInfoArray )
			+ sizeof( uint32_t )
			+ sizeof( render::CameraState ) );
		auto & state = *reinterpret_cast< StarMapState *  >( save.data() );
		state.m_count = uint32_t( m_stars.size() );
		auto * stars = reinterpret_cast< StarInfo *  >( save.data()
			+ sizeof( starmap::ConstellationInfoArray )
			+ sizeof( uint32_t )
			+ sizeof( render::CameraState ) );
		auto itst = stars;

		for ( auto & star : m_stars )
		{
			*itst = StarInfo{ star.index()
				, doConvertStdString( star.name() )
				, star.magnitude()
				, { star.position().x, star.position().y, star.position().z }
				, { star.colour().x, star.colour().y, star.colour().z } };
			++itst;
		}

		auto itc = state.m_constellations.begin();

		for ( auto & it : m_constellations )
		{
			auto & constellation = it.second;
			ConstellationInfo info{ doConvertStdString( it.first ) };
			auto its = info.m_stars.begin();
			assert( constellation->stars().size() <= info.m_stars.size() );

			for ( auto & star : constellation->stars() )
			{
				*its = { star.star().index()
					, star.id()
					, doConvertStdString( star.letter() ) };
				++its;
			}

			auto itl = info.m_links.begin();
			assert( constellation->links().size() <= info.m_links.size() );

			for ( auto & link : constellation->links() )
			{
				*itl = { doConvertStdString( link.m_a->letter() )
					, doConvertStdString( link.m_b->letter() ) };
				++itl;
			}

			*itc = std::move( info );
			++itc;
		}

		state.m_state = m_window.state();
	}

	void StarMap::initialise()
	{
		auto & scene = m_window.scene();

		// Initialise the scene
		scene.backgroundColour( gl::RgbaColour{ 0, 0, 0, 1 } );

		doInitialisePickObjects();
		doInitialiseLines();

		if ( !m_stars.empty() )
		{
			doSortStars();
			//auto range = render::makeRange( m_stars.begin()->magnitude()
			//	, m_stars.rbegin()->magnitude() );
			auto range = render::makeRange( 0.0f
				, m_stars.rbegin()->magnitude() / 2.0f );
			m_window.scene().thresholdBounds( 4.0f, 21.0f );

			for ( auto & star : m_stars )
			{
				doAddStar( star, range );
			}

			for ( auto & constellation : m_constellations )
			{
				doAddConstellation( *constellation.second );
			}

			doInitialiseStarNames();
			doInitialiseConstellationNames();
		}

		auto & picking = m_window.picking();
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
		m_pickDescriptionHolder.reset();
		m_pickBillboard.reset();
		m_onObjectPicked.disconnect();
		m_onBillboardPicked.disconnect();
		m_onUnpick.disconnect();
		m_holders.clear();
		m_lines.reset();
	}

	void StarMap::resize( gl::IVec2 const & size )
	{
		m_window.resize( size );
	}

	void StarMap::beginFrame()
	{
		m_window.beginFrame();
	}

	void StarMap::drawFrame()
	{
		m_window.update();
		doUpdatePickDescription();
		doUpdateStarNames();
		doUpdateConstellationNames();
		m_window.updateOverlays();
		m_window.draw();
	}

	void StarMap::endFrame()
	{
		m_window.endFrame();
	}

	void StarMap::add( Star const & star )
	{
		m_stars.push_back( star );
	}

	void StarMap::add( StarArray const & stars )
	{
		for ( auto & star : stars )
		{
			add( star );
		}
	}

	void StarMap::add( Constellation const & constellation )
	{
		doSortStars();
		auto it = m_constellations.find( constellation.name() );

		if ( it == m_constellations.end() )
		{
			auto it = m_constellations.emplace( constellation.name()
				, std::make_unique< Constellation >( constellation.name() ) ).first;

			for ( auto & star : constellation.stars() )
			{
				it->second->addStar( star.id(), star.letter(), star.name() );
			}

			it->second->fill( m_stars );
		}
	}

	Constellation * StarMap::findConstellation( std::string const & name )
	{
		auto it = m_constellations.find( name );
		Constellation * result{ nullptr };

		if ( it != m_constellations.end() )
		{
			result = &( *it->second );
		}
		else
		{
			std::cerr << "Constellation [" << name << "] not found." << std::endl;
		}

		return result;
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
		m_pickDescriptionHolder->show( false );
		m_pickedStar = nullptr;
	}

	void StarMap::doUpdatePicked( render::Movable const & movable )
	{
		m_pickBillboard->rescale( movable.scale() );
		m_pickBillboard->reorient( movable.orientation() );
		m_pickBillboard->update();
		m_pickBillboard->show( true );

		m_pickDescription->update();
		m_pickDescriptionHolder->size( m_pickDescription->size()
			+ gl::IVec2{ 20, 20 } );
		m_pickDescriptionHolder->update();
		m_pickDescription->show( true );
		m_pickDescriptionHolder->show( true );
	}

	void StarMap::doUpdatePicked( render::Object const & object )
	{
		m_pickDescription->caption( object.name() );
		doUpdateOverlay( *m_pickDescriptionHolder
			, m_window.scene().camera()
			, object.position()
			, StarNameOffset );
		m_pickDescription->position( m_pickDescriptionHolder->position()
			+ gl::IVec2{ 10, 10 } );

		m_pickBillboard->moveTo( object.position()
			- gl::Vec3{ 0, 0, object.boundaries().z + 0.2 } );
		doUpdatePicked( static_cast< render::Movable const & >( object ) );
		m_pickBillboard->dimensions( gl::IVec2{ gl::toVec2( object.boundaries() * 2.0f ) } );
		m_pickBillboard->buffer().at( 0u
			, { -1000.0f, gl::Vec3{ 0, 0, 0 }, gl::Vec2{ 1, 1 } } );
		auto percent = m_window.state().zoomBounds().percent( m_window.state().zoom() );
		m_pickBillboard->cull( m_window.scene().camera(), 2.0f * percent );
	}

	void StarMap::doUpdatePicked( render::Billboard const & billboard
		, uint32_t index )
	{
		auto & holder = doFindHolder( billboard.material().ambient() );

		if ( holder.m_stars.size() > index )
		{
			auto & star = *holder.m_stars[index];
			m_pickedStar = &star;
			auto it = std::find_if( std::begin( m_constellations )
				, std::end( m_constellations )
				, [&star]( auto const & pair )
				{
					return pair.second->findStar( star.name() );
				} );

			std::stringstream stream;
			stream << star;

			if ( it != std::end( m_constellations ) )
			{
				auto csStar = it->second->findStar( star.name() );
				stream << "\n"
					<< "Constellation : " << csStar->constellation().name() << "\n"
					<< "Bayer : " << csStar->letter() << "\n"
					<< "ID : " << csStar->id();
				m_pickDescription->caption( stream.str() );
			}
			else
			{
				m_pickDescription->caption( stream.str() );
			}

			doUpdatePickDescription();
			m_pickBillboard->moveTo( billboard.position() - gl::Vec3{ 0, 0, 0.02 } );
			doUpdatePicked( static_cast< render::Movable const & >( billboard ) );
			auto & data = billboard.buffer()[index];
			auto percent = m_window.state().zoomBounds().percent( m_window.state().zoom() );
			m_pickBillboard->buffer().at( 0u
				, { -1000.0f, data.center, gl::Vec2{ 1.0, 1.0 } } );
			m_pickBillboard->cull( m_window.scene().camera(), 2.0f * percent );
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
			auto & scene = m_window.scene();
			auto starsMat = std::make_shared< render::Material >();
			starsMat->opacityMap( m_opacity );
			starsMat->ambient( holder.m_colour );
			starsMat->diffuse( holder.m_colour );
			starsMat->emissive( holder.m_colour );
			//starsMat->alphaTest( true );
			scene.materials().addElement( sstars, starsMat );

			auto halosMat = std::make_shared< render::Material >();
			halosMat->opacityMap( m_opacity );
			halosMat->ambient( holder.m_colour );
			halosMat->diffuse( holder.m_colour );
			halosMat->emissive( holder.m_colour );
			scene.materials().addElement( shalos, halosMat );

			auto stars = std::make_shared< render::Billboard >( sstars
				, *holder.m_buffer );
			stars->dimensions( gl::IVec2{ 1, 1 } );
			stars->material( starsMat );
			scene.add( stars );

			//auto halos = std::make_shared< render::Billboard >( shalos
			//	, *holder.m_buffer );
			//halos->dimensions( gl::IVec2{ 2, 2 } );
			//halos->material( halosMat );
			//scene.add( halos );

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
			, gl::Vec2{ scale, scale } } );
		doInitialiseHolder( holder );
	}

	void StarMap::doAddConstellation( Constellation & constellation )
	{
		for ( auto & link : constellation.links() )
		{
			m_lines->add( { link.m_a->star().position()
				, link.m_b->star().position() } );
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
		auto & scene = m_window.scene();
		m_opacity = scene.textures().findElement( "halo.bmp" );

		if ( !m_opacity )
		{
			m_opacity = std::make_shared< render::Texture >();
			render::loadTexture( opacityMap, *m_opacity );
			scene.textures().addElement( "halo.bmp", m_opacity );
		}
	}

	void StarMap::doInitialisePickObjects()
	{
		auto & scene = m_window.scene();
		auto pickedMat = std::make_shared< render::Material >();
		pickedMat->opacityMap( m_opacity );
		pickedMat->ambient( PickBillboardColour );
		pickedMat->diffuse( PickBillboardColour );
		scene.materials().addElement( "picked", pickedMat );
		
		auto pickedDescriptionMat = std::make_shared< render::Material >();
		pickedDescriptionMat->ambient( PickDescriptionColour );
		pickedDescriptionMat->diffuse( PickDescriptionColour );
		scene.materials().addElement( "pickedDescription"
			, pickedDescriptionMat );

		auto pickedDescriptionHolderMat = std::make_shared< render::Material >();
		pickedDescriptionHolderMat->ambient( PickDescriptionHolderColour );
		pickedDescriptionHolderMat->diffuse( PickDescriptionHolderColour );
		pickedDescriptionHolderMat->opacity( PickDescriptionHolderOpacity );
		scene.materials().addElement( "pickedDescriptionHolder"
			, pickedDescriptionHolderMat );

		auto pickedDescriptionHolderBorderMat = std::make_shared< render::Material >();
		pickedDescriptionHolderBorderMat->ambient( PickDescriptionHolderBorderColour );
		pickedDescriptionHolderBorderMat->diffuse( PickDescriptionHolderBorderColour );
		scene.materials().addElement( "pickedDescriptionHolderBorder"
			, pickedDescriptionHolderBorderMat );

		auto pickedBuffers = std::make_shared< render::BillboardBuffer >( true );
		pickedBuffers->add( { -1000.0
			, gl::Vec3{ 0, 0, 0 }
			, gl::Vec2{ 1, 1 } } );
		scene.addBillboardBuffer( "picked", pickedBuffers );
		m_pickBillboard = std::make_shared< render::Billboard >( "picked"
			, *pickedBuffers );
		m_pickBillboard->material( pickedMat );
		m_pickBillboard->dimensions( { 30, 30 } );
		m_pickBillboard->show( false );
		scene.add( m_pickBillboard );

		m_pickDescriptionHolder = std::make_shared< render::BorderPanelOverlay >();
		m_pickDescriptionHolder->show( false );
		m_pickDescriptionHolder->material( pickedDescriptionHolderMat );
		m_pickDescriptionHolder->borderMaterial( pickedDescriptionHolderBorderMat );
		m_pickDescriptionHolder->borderSize( PickDescriptionHolderBorderSize );
		m_pickDescriptionHolder->borderPosition( render::BorderPosition::eInternal );
		m_pickDescriptionHolder->index( 2u );
		scene.overlays().addElement( "picked_holder", m_pickDescriptionHolder );

		m_pickDescription = std::make_shared< render::TextOverlay >();
		m_pickDescription->fontTexture( *m_fontTextureNames );
		m_pickDescription->show( false );
		m_pickDescription->material( pickedDescriptionMat );
		m_pickDescription->lineSpacingMode( render::TextLineSpacingMode::eMaxFontHeight );
		m_pickDescription->index( 3u );
		scene.overlays().addElement( "picked", m_pickDescription );
	}

	void StarMap::doInitialiseLines()
	{
		auto & scene = m_window.scene();
		auto linesMat = std::make_shared< render::Material >();
		linesMat->ambient( gl::toVec3( ConstellationColour ) );
		linesMat->diffuse( gl::toVec3( ConstellationColour ) );
		linesMat->emissive( gl::toVec3( ConstellationColour ) );
		scene.materials().addElement( "lines", linesMat );

		m_lines = std::make_shared< render::PolyLine >( "lines", 0.06f, 1.0f );
		m_lines->material( linesMat );
		scene.add( m_lines );
	}

	void StarMap::doInitialiseStarNames()
	{
		auto & scene = m_window.scene();
		m_starNames.resize( m_maxDisplayedStarNames );

		for ( auto i = 0u; i < m_maxDisplayedStarNames; ++i )
		{
			std::stringstream stream;
			stream << "Star_" << i;
			auto overlay = std::make_shared< render::TextOverlay >();
			overlay->material( scene.materials().findElement( "FullAlphaWhite" ) );
			overlay->fontTexture( *m_fontTextureNames );
			overlay->show( false );
			overlay->index( 1u );
			scene.overlays().addElement( stream.str(), overlay );

			m_starNames[i].m_overlay = overlay;
			m_starNames[i].m_element = nullptr;
		}
	}

	void StarMap::doInitialiseConstellationNames()
	{
		auto & scene = m_window.scene();
		m_constellationNames.resize( m_constellations.size() );
		uint32_t index{ 0u };

		auto constellationMat = std::make_shared< render::Material >();
		constellationMat->ambient( ConstellationTextColour );
		constellationMat->diffuse( ConstellationTextColour );
		scene.materials().addElement( "ConstellationName"
			, constellationMat );

		for ( auto const & constellation : m_constellations )
		{
			auto overlay = std::make_shared< render::TextOverlay >();
			overlay->caption( constellation.first );
			overlay->material( constellationMat );
			overlay->fontTexture( *m_fontTextureNames );
			overlay->index( 0u );
			scene.overlays().addElement( constellation.first, overlay );

			m_constellationNames[index].m_element = &( *constellation.second );
			m_constellationNames[index].m_overlay = overlay;
		}
	}

	void StarMap::doUpdatePickDescription()
	{
		if ( m_pickedStar )
		{
			doUpdateOverlay( *m_pickDescriptionHolder
				, m_window.scene().camera()
				, m_pickedStar->position()
				, StarNameOffset );

			auto const & camera = m_window.scene().camera();
			auto const & viewport = camera.viewport();
			auto position = m_pickDescriptionHolder->position();

			if ( position.x < 0 )
			{
				position.x = 0;
			}
			else if ( position.x + m_pickDescriptionHolder->size().x > viewport.size().x )
			{
				position.x = viewport.size().x - m_pickDescriptionHolder->size().x;
			}

			if ( position.y < 0 )
			{
				position.y = 0;
			}
			else if ( position.y + m_pickDescriptionHolder->size().y > viewport.size().y )
			{
				position.y = viewport.size().y - m_pickDescriptionHolder->size().y;
			}

			m_pickDescriptionHolder->position( position );
			m_pickDescription->position( position
				+ gl::IVec2{ 10, 10 } );
			m_pickDescription->show( m_pickDescriptionHolder->visible() );
		}
	}

	void StarMap::doUpdateStarNames()
	{
		uint32_t index = 0;

		for ( auto it = m_stars.begin();
			it != m_stars.end() && index < m_maxDisplayedStarNames;
			++it )
		{
			auto & star = *it;

			if ( m_window.scene().camera().visible( star.position() ) )
			{
				m_starNames[index].m_element = &star;
				m_starNames[index].m_overlay->caption( star.name() );
				m_starNames[index].m_overlay->show( true );
				doUpdateOverlay( *m_starNames[index].m_overlay
					, star.position()
					, StarNameOffset );
				++index;
			}
		}

		for ( auto it = m_starNames.begin() + index;
			it != m_starNames.end();
			++it )
		{
			it->m_element = nullptr;
			it->m_overlay->show( false );
		}
	}

	void StarMap::doUpdateConstellationNames()
	{
		auto & scene = m_window.scene();
		auto & camera = m_window.scene().camera();

		for ( auto const & constellation : m_constellations )
		{
			auto overlay = scene.overlays().findElement( constellation.first );
			doUpdateOverlay( *overlay
				, camera
				, constellation.second->position()
				, ConstellationNameOffset );
		}
	}

	void StarMap::doUpdateOverlay( render::Overlay & overlay
		, render::Camera const & camera
		, gl::Vec3 const & position
		, gl::IVec2 const & offset )
	{
		if ( camera.visible( position ) )
		{
			doUpdateOverlay( overlay, position, offset );
		}
		else
		{
			overlay.show( false );
		}
	}

	void StarMap::doUpdateOverlay( render::Overlay & overlay
		, gl::Vec3 const & position
		, gl::IVec2 const & offset )
	{
		auto const & camera = m_window.scene().camera();
		auto const & viewport = camera.viewport();
		auto const & projection = camera.projection();
		auto const & view = camera.view();
		auto projected = gl::project( position
			, view
			, projection
			, gl::Vec4{ 0, 0, viewport.size().x, viewport.size().y } );
		auto ovPosition = offset + gl::IVec2{ viewport.size().x - projected.x
			, viewport.size().y - projected.y };
		overlay.position( ovPosition );
		overlay.show( true );
	}

	void StarMap::doFilterConstellations( bool show )
	{
		auto & scene = m_window.scene();
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
			auto it = std::find_if( m_window.scene().billboards().begin()
				, m_window.scene().billboards().end()
				, [&holder]( render::BillboardPtr & billboard )
			{
				return &billboard->buffer() == holder.m_buffer.get();
			} );
			while ( it != m_window.scene().billboards().end() )
			{
				( *it )->show( show );
				++it;
				it = std::find_if( it
					, m_window.scene().billboards().end()
					, [&holder]( render::BillboardPtr & billboard )
				{
					return &billboard->buffer() == holder.m_buffer.get();
				} );
			}
		}
	}

	void StarMap::doSortStars()
	{
		if ( !m_sorted )
		{
			std::sort( std::begin( m_stars )
				, std::end( m_stars )
				, []( Star const & lhs, Star const & rhs )
				{
					return lhs.magnitude() < rhs.magnitude();
				} );

			uint32_t index{ 0u };

			for ( auto & star : m_stars )
			{
				star.index( index++ );
			}

			std::cout << "Most shiny star: " << m_stars.front() << std::endl;
			std::cout << "Least shiny star: " << m_stars.back() << std::endl;
			m_sorted = true;
		}
	}
}
