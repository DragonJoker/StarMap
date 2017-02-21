#include "CsvReader.h"

#include "StarMap.h"

#include <RenderLib/StringUtils.h>

namespace starmap
{
	namespace
	{
		float doParseFloat( std::string text )
		{
			text = render::replace( text, "\"", "" );
			text = render::replace( text, ",", "." );
			std::stringstream stream{ text };
			float result{ 0.0f };
			stream >> result;
			return result;
		}

		struct Index
		{
			uint32_t count;
			uint32_t proper;
			uint32_t gl;
			uint32_t hip;
			uint32_t rarad;
			uint32_t decrad;
			uint32_t mag;
			uint32_t spect;
			uint32_t ci;
			uint32_t x;
			uint32_t y;
			uint32_t z;
		};

		Index doLoadIndex( std::string const & line )
		{
			auto begin = std::begin( line );
			auto end = std::end( line );
			Index result{ 0xFFFFFFFF };
			result.count = uint32_t( std::count( begin, end, ',' ) + 1u );
			auto fields = render::split( line
				, ","
				, result.count
				, false );
			uint32_t i = 0u;

			for ( auto & field : fields )
			{
				if ( field == "proper" )
				{
					result.proper = i;
				}
				else if ( field == "gl" )
				{
					result.gl = i;
				}
				else if ( field == "hip" )
				{
					result.hip = i;
				}
				else if ( field == "rarad" )
				{
					result.rarad = i;
				}
				else if ( field == "decrad" )
				{
					result.decrad = i;
				}
				else if ( field == "mag" )
				{
					result.mag = i;
				}
				else if ( field == "spect" )
				{
					result.spect = i;
				}
				else if ( field == "ci" )
				{
					result.ci = i;
				}
				else if ( field == "x" )
				{
					result.x = i;
				}
				else if ( field == "y" )
				{
					result.y = i;
				}
				else if ( field == "z" )
				{
					result.z = i;
				}

				++i;
			}

			return result;
		}
	}

	void loadStarsFromCsv( StarMap & starmap, std::string content )
	{
		content = render::replace( content, '\r', "" );
		auto begin = std::begin( content );
		auto end = std::end( content );
		auto lines = render::split( content
			, "\n"
			, uint32_t( std::count( begin, end, '\n' ) + 1u )
			, false );
		content.clear();

		if ( lines.size() > 1 )
		{
#if !defined( NDEBUG )
			auto linesEnd = lines.begin() + std::min( 10000u, uint32_t( lines.size() ) );
#else
			auto linesEnd = lines.end();
#endif
			auto index = doLoadIndex( lines[0] );
			std::for_each( lines.begin() + 1
				, linesEnd
				, [&starmap, &index]( std::string & line )
				{
					auto values = render::split( line, ",", index.count, true );
					assert( values.size() == index.count );
					auto name = render::replace( values[index.proper], "\"", "" );

					if ( name.empty() )
					{
						name = render::replace( values[index.gl], "\"", "" );
					}

					if ( name.empty() )
					{
						name = render::replace( values[index.hip], "\"", "" );
					}

					auto magnitude = doParseFloat( values[index.mag] );
					auto ra = doParseFloat( values[index.rarad] );
					auto dec = doParseFloat( values[index.decrad] );
					auto ci = doParseFloat( values[index.ci] );
					auto colour = bvToRgb( ci );
					starmap.add( Star{ name, magnitude,gl::Vec2{ ra, dec },colour } );
					line.clear();
				} );
		}
	}
}
