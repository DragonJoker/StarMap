#include "stdafx.h"

#include "Engine.h"
#include "resource.h"

#include <DesktopUtils/FontLoader.h>
#include <DesktopUtils/ObjLoader.h>
#include <DesktopUtils/Log.h>

#include <array>
#include <string>

#include <RenderLib/Billboard.h>
#include <RenderLib/Object.h>
#include <RenderLib/PolyLine.h>
#include <RenderLib/Scene.h>

Engine::Engine()
	: MsWindow{}
	, m_debug{ true }
{
	std::string const appName = "RenderLibTestApp";
	m_cout = new render::LogStreambuf< utils::InfoLogStreambufTraits >( appName
		, std::cout );
	m_cerr = new render::LogStreambuf< utils::ErrorLogStreambufTraits >( appName
		, std::cerr );
	m_clog = new render::LogStreambuf< utils::DebugLogStreambufTraits >( appName
		, std::clog );
}

Engine::~Engine()
{
	delete m_cout;
	delete m_cerr;
	delete m_clog;
}

void Engine::onDraw()
{
	if ( m_window )
	{
		m_debug.start();
		m_window->update();
		m_window->updateOverlays();
		m_window->draw();
		doSwapBuffers();
		m_debug.end();
	}
}

void Engine::onCreate()
{
	doInitialise3DElements();
}

void Engine::onDestroy()
{
	doCleanup3DElements();
}

void Engine::onResize( gl::Size2D const & event )
{
	if ( m_window )
	{
		m_window->resize( event );
	}
}

void Engine::onMouseMove( utils::MouseEvent const & event )
{
	if ( event.ldown )
	{
		auto diff = event.position - m_mouse;
		m_window->state().velocity( diff );
		m_mouse = event.position;
	}
}

void Engine::onLButtonDown( utils::MouseEvent const & event )
{
	m_mouse = event.position;
}

void Engine::onLButtonClick( utils::MouseEvent const & event )
{
	if ( m_window )
	{
		m_window->pick( event.position );
	}
}

void Engine::onLButtonDblClick( utils::MouseEvent const & event )
{
	m_window->state().reset();
}

void Engine::onMouseWheel( utils::MouseEvent const & event )
{
	m_window->state().zoomVelocity( float( event.delta ) );
}

void Engine::onObjectPicked( render::Object & object )
{
	if ( m_picked )
	{
		m_pickedObject = &object;
		doUpdatePicked( object );
	}
}

void Engine::onBillboardPicked( render::Billboard & billboard, uint32_t index )
{
	if ( m_picked )
	{
		m_pickedBillboard = &billboard;
		doUpdatePicked( billboard, index );
	}
}

void Engine::onUnpick()
{
	if ( m_picked )
	{
		m_picked->show( false );
	}
}

void Engine::doInitialise3DElements()
{
	auto width = size().x;
	auto height = size().y;
	float ratio = float( width ) / height;

	// Initialise OpenGL
	if ( !gl::OpenGL::isInitialised() )
	{
		gl::OpenGL::initialise();
	}

	// Initialise the render window
	m_window = std::make_unique< render::RenderWindow >( gl::Size2D{ width
		, height } );
	m_window->viewport().fovY( render::Angle{ 45.0_degrees } );

	// Initialise the scene
	auto & scene = m_window->scene();
	utils::FontLoader loader{ "arial.ttf" };
	m_debug.initialise( scene, loader );
	scene.backgroundColour( gl::RgbaColour{ 0.5, 0.5, 0.5, 1.0 } );

	// Populate the scene
	std::string content = utils::getFileTextContent( "tex_cube.obj" );

	if ( !content.empty() )
	{
		render::ObjectPtr object = utils::loadObjFile( "Cube"
			, content
			, scene.materials()
			, scene.textures()
			, scene.meshes() );
		object->moveTo( gl::Vector3D{ 0.0, 0.0, 52.0 } );
		scene.add( object );
	}

	auto texture = scene.textures().findElement( "texture.bmp" );

	if ( !texture )
	{
		texture = scene.textures().addElement( "texture.bmp" );
		render::loadTexture( utils::getFileBinaryContent( "texture.bmp" )
			, *texture );
	}

	auto opacity = scene.textures().findElement( "halo.bmp" );

	if ( !opacity )
	{
		opacity = scene.textures().addElement( "halo.bmp" );
		render::loadTexture( utils::getFileBinaryContent( "halo.bmp" )
			, *opacity );
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

	doInitialiseFontTexture();
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

	m_onObjectPicked = m_window->picking().onObjectPicked.connect
		( std::bind( &Engine::onObjectPicked
			, this
			, std::placeholders::_1 ) );
	m_onBillboardPicked = m_window->picking().onBillboardPicked.connect
		( std::bind( &Engine::onBillboardPicked
			, this
			, std::placeholders::_1
			, std::placeholders::_2 ) );
	m_onUnpick = m_window->picking().onUnpick.connect
		( std::bind( &Engine::onUnpick
			, this ) );
}

void Engine::doCleanup3DElements()
{
	onUnpick();
	m_picked.reset();
	m_onObjectPicked.disconnect();
	m_onBillboardPicked.disconnect();
	m_fontTexture.reset();
	m_window.reset();
}

void Engine::doInitialiseFontTexture()
{
	auto content = utils::getFileBinaryContent( "arial.ttf" );
	render::FontPtr font = std::make_unique< render::Font >( "Arial", 32u );
	utils::FontLoader loader{ "arial.ttf" };
	render::loadFont( loader, *font );
	m_fontTexture = std::make_unique< render::FontTexture >
		( std::move( font ) );
}

void Engine::doUpdatePicked( render::Movable const & movable )
{
	m_picked->rescale( movable.scale() );
	m_picked->reorient( movable.orientation() );
	m_picked->update();
	m_picked->show( true );
}

void Engine::doUpdatePicked( render::Object const & object )
{
	m_picked->moveTo( object.position() - gl::Vector3D{ 0, 0, object.boundaries().z + 0.1 } );
	doUpdatePicked( static_cast< render::Movable const & >( object ) );
	m_picked->dimensions( object.boundaries() );
	m_picked->buffer().at( 0u, { -1000.0f, gl::Vector3D{ 0, 0, 0 }, gl::Vector2D{ 1, 1 } } );
}

void Engine::doUpdatePicked( render::Billboard const & billboard
	, uint32_t index )
{
	if ( &billboard == m_picked.get() )
	{
		m_picked->show( false );
	}
	else
	{
		m_picked->moveTo( billboard.position() - gl::Vector3D{ 0, 0, 0.2 } );
		doUpdatePicked( static_cast< render::Movable const & >( billboard ) );
		m_picked->dimensions( billboard.dimensions() );
		auto data = billboard.buffer()[index];
		m_picked->buffer().at( 0u, { -1000.0f, data.center, data.scale } );
	}
}