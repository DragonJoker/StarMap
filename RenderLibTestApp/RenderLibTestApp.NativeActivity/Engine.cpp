#include "Engine.h"

#include <AndroidUtils/AndroidApp.h>
#include <AndroidUtils/FontLoader.h>
#include <AndroidUtils/ObjLoader.h>
#include <AndroidUtils/Log.h>

#include <array>
#include <string>

#include <android/configuration.h>
#include <android/window.h>

#include <RenderLib/Billboard.h>
#include <RenderLib/PolyLine.h>
#include <RenderLib/RenderWindow.h>

//*****************************************************************************

Window::Window( utils::AndroidApp const & parent
	, ANativeWindow * window
	, render::ByteArray const & state )
	: utils::AndroidWindow{ parent
		, window
		, state }
{
}

Window::~Window()
{
}

void Window::onCreate()
{
	// Initialise the render window
	auto loader = doCreateFontLoader( "arial.ttf" );
	m_renderWindow = std::make_unique< render::RenderWindow >( m_size
		, *loader
		, true );
	m_renderWindow->viewport().fovY( gl::Angle{ 45.0_degrees } );

	// Initialise the scene
	auto & scene = m_renderWindow->scene();
	scene.backgroundColour( gl::RgbaColour{ 0.5, 0.5, 0.5, 1.0 } );
	m_fontTexture = doCreateFontTexture( "arial.ttf"
		, 32 );

	auto texture = scene.textures().findElement( "texture.bmp" );

	if ( !texture )
	{
		texture = std::make_shared< render::Texture >();
		render::loadTexture( m_parent.getFileBinaryContent( "texture.bmp"
				, true )
			, *texture );
		scene.textures().addElement( "texture.bmp", texture );
	}

	auto opacity = scene.textures().findElement( "halo.bmp" );

	if ( !opacity )
	{
		opacity = std::make_shared< render::Texture >();
		render::loadTexture( m_parent.getFileBinaryContent( "halo.bmp"
				, true )
			, *opacity );
		scene.textures().addElement( "halo.bmp", opacity );
	}

	// Populate the scene
	std::string content = m_parent.getFileTextContent( "tex_cube.obj", true );

	if ( !content.empty() )
	{
		render::ObjectPtr object = utils::loadObjFile( m_parent
			, "Cube"
			, content
			, scene.materials()
			, scene.textures()
			, scene.meshes() );
		object->moveTo( gl::Vec3{ 0.0, 0.0, 52.0 } );
		scene.add( object );
	}

	auto pickedMat = std::make_shared< render::Material >();
	pickedMat->opacityMap( opacity );
	pickedMat->ambient( gl::RgbColour{ 0.0, 0.0, 0.5 } );
	pickedMat->diffuse( gl::RgbColour{ 0.0, 0.0, 0.5 } );
	scene.materials().addElement( "picked", pickedMat );

	auto pickedBuffers = std::make_shared< render::BillboardBuffer >( false );
	pickedBuffers->add( { -1000.0f, gl::Vec3{ 0, 0, 0 }, gl::Vec2{ 1, 1 } } );
	scene.addBillboardBuffer( "picked", pickedBuffers );
	m_picked = std::make_shared< render::Billboard >( "picked", *pickedBuffers );
	m_picked->material( pickedMat );
	m_picked->show( false );
	scene.add( m_picked );

	auto billboardMat = std::make_shared< render::Material >();
	billboardMat->diffuseMap( texture );
	billboardMat->opacityMap( opacity );
	billboardMat->ambient( gl::RgbColour{ 1.0, 0.0, 0.5 } );
	billboardMat->diffuse( gl::RgbColour{ 1.0, 0.0, 0.5 } );
	billboardMat->emissive( gl::RgbColour{ 1.0, 0.0, 0.5 } );
	scene.materials().addElement( "billboard", billboardMat );
	auto billboardBuffers = std::make_shared< render::BillboardBuffer >( false );
	billboardBuffers->add( { -100.0f, gl::Vec3{ 1, 0, 0 }, gl::Vec2{ 1, 1 } } );
	billboardBuffers->add( { -100.0f, gl::Vec3{ 0, 1, 0 }, gl::Vec2{ 1, 0.5 } } );
	billboardBuffers->add( { -100.0f, gl::Vec3{ -1, 0, 0 }, gl::Vec2{ 0.5, 1 } } );
	billboardBuffers->add( { -100.0f, gl::Vec3{ 0, -1, 0 }, gl::Vec2{ 1.5, 1.5 } } );
	scene.addBillboardBuffer( "billboard", billboardBuffers );
	auto billboard = std::make_shared< render::Billboard >( "billboard", *billboardBuffers );
	billboard->dimensions( gl::IVec2{ 1, 1 } );
	billboard->moveTo( gl::Vec3{ 0, 0, 50 } );
	billboard->scale( gl::Vec3{ 1.5, 1.5, 1.5 } );
	billboard->material( billboardMat );
	scene.add( billboard );

	auto starsMat = std::make_shared< render::Material >();
	starsMat->opacityMap( opacity );
	starsMat->ambient( gl::RgbColour{ 1.0, 1.0, 0.5 } );
	starsMat->diffuse( gl::RgbColour{ 1.0, 1.0, 0.5 } );
	starsMat->emissive( gl::RgbColour{ 1.0, 1.0, 0.5 } );
	starsMat->alphaTest( true );
	scene.materials().addElement( "stars", starsMat );
	auto halosMat = std::make_shared< render::Material >();
	halosMat->opacityMap( opacity );
	halosMat->ambient( gl::RgbColour{ 1.0, 1.0, 0.5 } );
	halosMat->diffuse( gl::RgbColour{ 1.0, 1.0, 0.5 } );
	halosMat->emissive( gl::RgbColour{ 1.0, 1.0, 0.5 } );
	scene.materials().addElement( "halos", halosMat );
	auto starsBuffers = std::make_shared< render::BillboardBuffer >( false );
	starsBuffers->add( { 50.0f, gl::Vec3{ -1, 1, 0 }, gl::Vec2{ 1, 1 } } );
	starsBuffers->add( { 50.0f, gl::Vec3{ 1, 1, 0 }, gl::Vec2{ 1, 0.5 } } );
	starsBuffers->add( { 50.0f, gl::Vec3{ 1, -1, 0 }, gl::Vec2{ 0.5, 1 } } );
	starsBuffers->add( { 50.0f, gl::Vec3{ -1, -1, 0 }, gl::Vec2{ 1.5, 1.5 } } );
	scene.addBillboardBuffer( "stars", starsBuffers );
	auto stars = std::make_shared< render::Billboard >( "stars", *starsBuffers );
	stars->dimensions( gl::IVec2{ 1, 1 } );
	stars->moveTo( gl::Vec3{ 0, 0, 50 } );
	stars->material( starsMat );
	scene.add( stars );
	auto halos = std::make_shared< render::Billboard >( "halos", *starsBuffers );
	halos->dimensions( gl::IVec2{ 2, 2 } );
	halos->moveTo( gl::Vec3{ 0, 0, 50 } );
	halos->material( halosMat );
	scene.add( halos );

	auto linesMat = std::make_shared< render::Material >();
	linesMat->ambient( gl::RgbColour{ 1.0, 1.0, 0.5 } );
	linesMat->diffuse( gl::RgbColour{ 1.0, 1.0, 0.5 } );
	linesMat->emissive( gl::RgbColour{ 1.0, 1.0, 0.5 } );
	scene.materials().addElement( "lines", linesMat );
	auto lines = std::make_shared< render::PolyLine >( "lines" );
	lines->add( { gl::Vec3{ -1, 1, 0 }, gl::Vec3{ 1, 1, 0 } } );
	lines->add( { gl::Vec3{ 1, 1, 0 }, gl::Vec3{ 1, -1, 0 } } );
	lines->add( { gl::Vec3{ 1, -1, 0 }, gl::Vec3{ -1, -1, 0 } } );
	lines->add( { gl::Vec3{ -1, -1, 0 }, gl::Vec3{ -1, 1, 0 } } );
	lines->moveTo( gl::Vec3{ 0, 0, 50 } );
	lines->material( linesMat );
	scene.add( lines );

	auto coinMat = doCreateOverlayMaterial( "coin", gl::RgbColour{ 0, 1, 0 }, 1 );
	auto overlay = std::make_shared< render::TextOverlay >();
	overlay->position( gl::IVec2{ 200, 200 } );
	overlay->material( coinMat );
	overlay->caption( "coin !!" );
	overlay->fontTexture( *m_fontTexture );
	scene.overlays().addElement( "coin", overlay );

	auto glopMat = doCreateOverlayMaterial( "glop", gl::RgbColour{ 1, 0, 0 }, 1 );
	overlay = std::make_shared< render::TextOverlay >();
	overlay->position( gl::IVec2{ 400, 200 } );
	overlay->material( glopMat );
	overlay->caption( "glop !!" );
	overlay->fontTexture( *m_fontTexture );
	scene.overlays().addElement( "glop", overlay );

	m_onObjectPicked = m_renderWindow->picking().onObjectPicked.connect
		( std::bind( &Window::onObjectPicked
		, this
		, std::placeholders::_1 ) );
	m_onBillboardPicked = m_renderWindow->picking().onBillboardPicked.connect
		( std::bind( &Window::onBillboardPicked
		, this
		, std::placeholders::_1
		, std::placeholders::_2 ) );
	m_onUnpick = m_renderWindow->picking().onUnpick.connect
		( std::bind( &Window::onUnpick
		, this ) );
}

void Window::onDestroy()
{
	onUnpick();
	m_picked.reset();
	m_onUnpick.disconnect();
	m_onObjectPicked.disconnect();
	m_onBillboardPicked.disconnect();
	m_fontTexture.reset();
	m_renderWindow.reset();
}

void Window::onDraw()
{
	m_renderWindow->beginFrame();
	m_renderWindow->update();
	m_renderWindow->updateOverlays();
	m_renderWindow->draw();
	eglSwapBuffers( m_display, m_surface );
	m_renderWindow->endFrame();
}

void Window::onSave( render::ByteArray & state )
{
	state.resize( sizeof( render::CameraState ) );
	*( reinterpret_cast< render::CameraState * >( state.data() ) ) = m_renderWindow->state();
}

void Window::onRestore( render::ByteArray const & state )
{
	assert( state.size() >= sizeof( render::CameraState ) );
	m_renderWindow->state() = *( reinterpret_cast< render::CameraState const * >( state.data() ) );
}

void Window::onSingleTap( gl::IVec2 const & position )
{
	if ( m_renderWindow )
	{
		m_renderWindow->pick( position );
	}
}

void Window::onDoubleTap( gl::IVec2 const & position )
{
	if ( m_renderWindow )
	{
		m_renderWindow->state().reset();
	}
}

void Window::onSingleMove( gl::IVec2 const & position )
{
	if ( m_renderWindow )
	{
		m_renderWindow->state().velocity( gl::Vec2{ position } );
	}
}

void Window::onDoubleMove( gl::IVec2 const & posDiff, int distDiff )
{
	if ( m_renderWindow )
	{
		m_renderWindow->state().zoomVelocity( float( -distDiff ) );
	}
}

void Window::onObjectPicked( render::Object & object )
{
	if ( m_picked )
	{
		m_pickedObject = &object;
		doUpdatePicked( object );
	}
}

void Window::onBillboardPicked( render::Billboard & billboard, uint32_t index )
{
	if ( m_picked )
	{
		m_pickedBillboard = &billboard;
		doUpdatePicked( billboard, index );
	}
}

void Window::onUnpick()
{
	if ( m_picked )
	{
		m_picked->show( false );
	}
}

void Window::doUpdatePicked( render::Movable const & movable )
{
	m_picked->rescale( movable.scale() );
	m_picked->reorient( movable.orientation() );
	m_picked->update();
	m_picked->show( true );
}

void Window::doUpdatePicked( render::Object const & object )
{
	m_picked->moveTo( object.position() - gl::Vec3{ 0, 0, object.boundaries().z + 0.2 } );
	doUpdatePicked( static_cast< render::Movable const & >( object ) );
	m_picked->dimensions( gl::IVec2{ gl::toVec2( object.boundaries() ) } );
	m_picked->buffer().at( 0u, { -1000.0f, gl::Vec3{ 0, 0, 0 }, gl::Vec2{ 1, 1 } } );
	auto percent = m_renderWindow->state().zoomBounds().invpercent( m_renderWindow->state().zoom() );
	m_picked->cull( m_renderWindow->scene().camera(), 2.0f - 2.0f * percent );
}

void Window::doUpdatePicked( render::Billboard const & billboard
	, uint32_t index )
{
	m_picked->moveTo( billboard.position() - gl::Vec3{ 0, 0, 0.2 } );
	doUpdatePicked( static_cast< render::Movable const & >( billboard ) );
	m_picked->dimensions( billboard.dimensions() );
	auto data = billboard.buffer()[index];
	auto percent = m_renderWindow->state().zoomBounds().invpercent( m_renderWindow->state().zoom() );
	auto scale = 0.1f + percent;
	m_picked->buffer().at( 0u
		, { -1000.0f, data.center, gl::Vec2{ scale, scale } } );
	m_picked->cull( m_renderWindow->scene().camera(), 2.0f - 2.0f * percent );
}

render::MaterialPtr Window::doCreateOverlayMaterial( std::string const & name
	, gl::RgbColour const & colour
	, float opacity )
{
	auto & scene = m_renderWindow->scene();
	render::MaterialPtr result = scene.materials().findElement( name );

	if ( !result )
	{
		result = std::make_unique< render::Material >();
		result->ambient( colour );
		result->diffuse( colour );
		result->specular( colour );
		result->emissive( colour );
		result->opacity( opacity );
		scene.materials().addElement( name, result );
	}

	return result;
}

//*****************************************************************************

int32_t Engine::handleInput( android_app * app, AInputEvent * event )
{
	int32_t ret = 0;
	auto engine = reinterpret_cast< utils::AndroidApp * >( app->userData );

	switch ( AInputEvent_getType( event ) )
	{
	case AINPUT_EVENT_TYPE_MOTION:
		if ( AMotionEvent_getPointerCount( event ) == 1 )
		{
			ret = engine->onSingleTouchEvent( event );
		}
		else
		{
			ret = engine->onMultiTouchEvent( event );
		}
		break;
	}

	return ret;
}

void Engine::handleCommand( android_app * app, int32_t cmd )
{
	auto engine = reinterpret_cast< utils::AndroidApp * >( app->userData );

	switch ( cmd )
	{
	case APP_CMD_INIT_WINDOW:
		// La fenêtre est affichée : opération de préparation.
		engine->onInitWindow( app->window
			, app->savedState
			, app->savedStateSize );
		break;

	case APP_CMD_TERM_WINDOW:
		// La fenêtre est masquée ou fermée : opération de nettoyage.
		engine->onTermWindow();
		break;

	case APP_CMD_GAINED_FOCUS:
		// Quand l'application obtient le focus, la surveillance de l'accéléromètre est démarrée.
		engine->onGainedFocus();
		break;

	case APP_CMD_LOST_FOCUS:
		// Quand l'application perd le focus, la surveillance de l'accéléromètre est arrêtée.
		// Cela évite de décharger la batterie quand elle n'est pas utilisée.
		engine->onLostFocus();
		break;

	case APP_CMD_CONFIG_CHANGED:
		engine->onUpdateConfig();
		break;

	case APP_CMD_SAVE_STATE:
		// Le système demande d'enregistrer l'état actuel. Cette opération est effectuée.
		engine->onSave( app->savedState, app->savedStateSize );
		break;
	}
}

//*****************************************************************************
