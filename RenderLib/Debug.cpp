#include "Debug.h"

#include "Scene.h"
#include "TextOverlay.h"

#include <iomanip>
#include <sstream>

namespace render
{
	namespace
	{
		static std::string const Time = "Debug_Time";
		static std::string const Version = "Debug_Version";
		static std::string const BillboardCount = "Debug_BillboardCount";
		static std::string const BufferCount = "Debug_BufferCount";
	}

	Debug::Debug( bool enable )
		: m_enabled{ enable }
	{
	}

	void Debug::initialise( render::Scene & scene
		, render::FontLoader & loader )
	{
		if ( m_enabled )
		{
			render::FontPtr font = std::make_unique< render::Font >( "Arial"
				, 32 );
			render::loadFont( loader, *font );
			m_fontTexture = std::make_unique< render::FontTexture >
				( std::move( font ) );

			m_scene = &scene;
			m_version = m_scene->overlays().addElement( Version );
			m_version->position( { 0, 0 } );
			m_version->colour( { 1, 1, 1, 1 } );
			m_version->caption( gl::OpenGL::getVersion() );
			m_version->fontTexture( *m_fontTexture );

			m_time = m_scene->overlays().addElement( Time );
			m_time->position( { 0, 40 } );
			m_time->colour( { 1, 1, 1, 1 } );
			m_time->fontTexture( *m_fontTexture );

			m_billboardCount = m_scene->overlays().addElement( BillboardCount );
			m_billboardCount->position( { 0, 80 } );
			m_billboardCount->colour( { 1, 1, 1, 1 } );
			m_billboardCount->fontTexture( *m_fontTexture );

			m_buffersCount = m_scene->overlays().addElement( BufferCount );
			m_buffersCount->position( { 0, 120 } );
			m_buffersCount->colour( { 1, 1, 1, 1 } );
			m_buffersCount->fontTexture( *m_fontTexture );
		}
	}

	void Debug::cleanup()
	{
		if ( m_enabled )
		{
			m_time.reset();
			m_buffersCount.reset();
			m_billboardCount.reset();
			m_version.reset();
			m_scene->overlays().removeElement( BufferCount );
			m_scene->overlays().removeElement( BillboardCount );
			m_scene->overlays().removeElement( Version );
			m_scene->overlays().removeElement( Time );
			m_fontTexture.reset();
		}
	}

	void Debug::start()
	{
		if ( m_enabled )
		{
			m_startTime = Clock::now();
		}
	}

	void Debug::end()
	{
		if ( m_enabled )
		{
			{
				auto end = Clock::now();
				auto duration = std::chrono::duration_cast< std::chrono::microseconds >( end - m_startTime );
				std::stringstream stream;
				stream << std::fixed << std::setprecision( 2 ) << duration.count() / 1000.0 << "ms";
				m_time->caption( "Time: " + stream.str() );
			}
			{
				std::stringstream stream;
				stream << m_scene->billboardsCount();
				m_billboardCount->caption( "Billboards: " + stream.str() );
			}
			{
				std::stringstream stream;
				stream << m_scene->buffersCount();
				m_buffersCount->caption( "Buffers: " + stream.str() );
			}
		}
	}
}
