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
	, m_debug{ true }
{
}

Window::~Window()
{
}

void Window::onCreate()
{
	// Initialise the render window
	m_renderWindow = std::make_unique< render::RenderWindow >( m_size );

	auto & scene = m_renderWindow->scene();

	{
		auto loader = doCreateFontLoader( "arial.ttf" );

		if ( loader )
		{
			m_debug.initialise( scene, *loader );
		}
	}

	m_fontTexture = doCreateFontTexture( "arial.ttf"
		, 32 );

	// Initialise the scene
	auto texture = scene.textures().findElement( "texture.bmp" );

	if ( !texture )
	{
		texture = scene.textures().addElement( "texture.bmp" );
		render::loadTexture( m_parent.getFileBinaryContent( "texture.bmp"
			, true )
			, *texture );
	}

	auto opacity = scene.textures().findElement( "halo.bmp" );

	if ( !opacity )
	{
		opacity = scene.textures().addElement( "halo.bmp" );
		render::loadTexture( m_parent.getFileBinaryContent( "halo.bmp"
			, true )
			, *opacity );
	}

	scene.backgroundColour( gl::RgbaColour{ 0.5, 0.5, 0.5, 1.0 } );

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
		object->moveTo( gl::Vector3D{ 0.0, 0.0, 52.0 } );
		scene.add( object );
	}

	auto pickedMat = scene.materials().addElement( "picked" );
	pickedMat->opacityMap( opacity );
	pickedMat->ambient( gl::RgbColour{ 0.0, 0.0, 0.5 } );
	pickedMat->diffuse( gl::RgbColour{ 0.0, 0.0, 0.5 } );
	auto pickedBuffers = std::make_shared< render::BillboardBuffer >( false );
	pickedBuffers->add( { -1000.0f, gl::Vector3D{ 0, 0, 0 }, gl::Vector2D{ 1, 1 } } );
	scene.addBillboardBuffer( "picked", pickedBuffers );
	m_picked = std::make_shared< render::Billboard >( "picked", *pickedBuffers );
	m_picked->material( pickedMat );
	m_picked->show( false );
	scene.add( m_picked );

	auto billboardMat = scene.materials().addElement( "billboard" );
	billboardMat->diffuseMap( texture );
	billboardMat->opacityMap( opacity );
	billboardMat->ambient( gl::RgbColour{ 1.0, 0.0, 0.5 } );
	billboardMat->diffuse( gl::RgbColour{ 1.0, 0.0, 0.5 } );
	billboardMat->emissive( gl::RgbColour{ 1.0, 0.0, 0.5 } );
	auto billboardBuffers = std::make_shared< render::BillboardBuffer >( false );
	billboardBuffers->add( { -100.0f, gl::Vector3D{ 1, 0, 0 }, gl::Vector2D{ 1, 1 } } );
	billboardBuffers->add( { -100.0f, gl::Vector3D{ 0, 1, 0 }, gl::Vector2D{ 1, 0.5 } } );
	billboardBuffers->add( { -100.0f, gl::Vector3D{ -1, 0, 0 }, gl::Vector2D{ 0.5, 1 } } );
	billboardBuffers->add( { -100.0f, gl::Vector3D{ 0, -1, 0 }, gl::Vector2D{ 1.5, 1.5 } } );
	billboardBuffers->initialise();
	scene.addBillboardBuffer( "billboard", billboardBuffers );
	auto billboard = std::make_shared< render::Billboard >( "billboard", *billboardBuffers );
	billboard->dimensions( gl::Size2D{ 1, 1 } );
	billboard->moveTo( gl::Vector3D{ 0, 0, 50 } );
	billboard->scale( gl::Vector3D{ 1.5, 1.5, 1.5 } );
	billboard->material( billboardMat );
	scene.add( billboard );

	auto starsMat = scene.materials().addElement( "stars" );
	starsMat->opacityMap( opacity );
	starsMat->ambient( gl::RgbColour{ 1.0, 1.0, 0.5 } );
	starsMat->diffuse( gl::RgbColour{ 1.0, 1.0, 0.5 } );
	starsMat->emissive( gl::RgbColour{ 1.0, 1.0, 0.5 } );
	starsMat->alphaTest( true );
	auto halosMat = scene.materials().addElement( "halos" );
	halosMat->opacityMap( opacity );
	halosMat->ambient( gl::RgbColour{ 1.0, 1.0, 0.5 } );
	halosMat->diffuse( gl::RgbColour{ 1.0, 1.0, 0.5 } );
	halosMat->emissive( gl::RgbColour{ 1.0, 1.0, 0.5 } );
	auto starsBuffers = std::make_shared< render::BillboardBuffer >( false );
	starsBuffers->add( { 50.0f, gl::Vector3D{ -1, 1, 0 }, gl::Vector2D{ 1, 1 } } );
	starsBuffers->add( { 50.0f, gl::Vector3D{ 1, 1, 0 }, gl::Vector2D{ 1, 0.5 } } );
	starsBuffers->add( { 50.0f, gl::Vector3D{ 1, -1, 0 }, gl::Vector2D{ 0.5, 1 } } );
	starsBuffers->add( { 50.0f, gl::Vector3D{ -1, -1, 0 }, gl::Vector2D{ 1.5, 1.5 } } );
	starsBuffers->initialise();
	scene.addBillboardBuffer( "stars", starsBuffers );
	auto stars = std::make_shared< render::Billboard >( "stars", *starsBuffers );
	stars->dimensions( gl::Size2D{ 1, 1 } );
	stars->moveTo( gl::Vector3D{ 0, 0, 50 } );
	stars->material( starsMat );
	scene.add( stars );
	auto halos = std::make_shared< render::Billboard >( "halos", *starsBuffers );
	halos->dimensions( gl::Size2D{ 2, 2 } );
	halos->moveTo( gl::Vector3D{ 0, 0, 50 } );
	halos->material( halosMat );
	scene.add( halos );

	auto linesMat = scene.materials().addElement( "lines" );
	linesMat->ambient( gl::RgbColour{ 1.0, 1.0, 0.5 } );
	linesMat->diffuse( gl::RgbColour{ 1.0, 1.0, 0.5 } );
	linesMat->emissive( gl::RgbColour{ 1.0, 1.0, 0.5 } );
	auto lines = std::make_shared< render::PolyLine >( "lines" );
	lines->add( { gl::Vector3D{ -1, 1, 0 }, gl::Vector3D{ 1, 1, 0 } } );
	lines->add( { gl::Vector3D{ 1, 1, 0 }, gl::Vector3D{ 1, -1, 0 } } );
	lines->add( { gl::Vector3D{ 1, -1, 0 }, gl::Vector3D{ -1, -1, 0 } } );
	lines->add( { gl::Vector3D{ -1, -1, 0 }, gl::Vector3D{ -1, 1, 0 } } );
	lines->moveTo( gl::Vector3D{ 0, 0, 50 } );
	lines->material( linesMat );
	scene.add( lines );

	auto overlay = scene.overlays().addElement( "coin" );
	overlay->position( gl::Position2D{ 200, 200 } );
	overlay->colour( gl::RgbaColour{ 0.0, 1.0, 0.0, 1.0 } );
	overlay->caption( "coin !!" );
	overlay->fontTexture( *m_fontTexture );

	overlay = scene.overlays().addElement( "glop" );
	overlay->position( gl::Position2D{ 400, 200 } );
	overlay->colour( gl::RgbaColour{ 1.0, 0.0, 0.0, 1.0 } );
	overlay->caption( "glop !!" );
	overlay->fontTexture( *m_fontTexture );

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
	m_debug.start();
	m_renderWindow->update();
	m_renderWindow->updateOverlays();
	m_renderWindow->draw();
	eglSwapBuffers( m_display, m_surface );
	m_debug.end();
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

void Window::onSingleTap( gl::Position2D const & position )
{
	if ( m_renderWindow )
	{
		m_renderWindow->pick( position );
	}
}

void Window::onDoubleTap( gl::Position2D const & position )
{
	if ( m_renderWindow )
	{
		m_renderWindow->state().reset();
	}
}

void Window::onSingleMove( gl::Position2D const & position )
{
	if ( m_renderWindow )
	{
		m_renderWindow->state().velocity( position );
	}
}

void Window::onDoubleMove( gl::Offset2D const & posDiff, int distDiff )
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
	m_picked->moveTo( object.position() - gl::Vector3D{ 0, 0, object.boundaries().z + 0.2 } );
	doUpdatePicked( static_cast< render::Movable const & >( object ) );
	m_picked->dimensions( object.boundaries() );
	m_picked->buffer().at( 0u, { -1000.0f, gl::Vector3D{ 0, 0, 0 }, gl::Vector2D{ 1, 1 } } );
}

void Window::doUpdatePicked( render::Billboard const & billboard
	, uint32_t index )
{
	m_picked->moveTo( billboard.position() - gl::Vector3D{ 0, 0, 0.2 } );
	doUpdatePicked( static_cast< render::Movable const & >( billboard ) );
	m_picked->dimensions( billboard.dimensions() );
	auto data = billboard.buffer()[index];
	m_picked->buffer().at( 0u, { -1000.0f, data.center, data.scale } );
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
