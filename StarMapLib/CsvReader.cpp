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
			float result;
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
			result.count = std::count( begin, end, ',' ) + 1;
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

		double doBvToKelvin( float bv )
		{
			return 4600 * ( ( 1 / ( ( 0.92f * bv ) + 1.7f ) ) + ( 1 / ( ( 0.92f * bv ) + 0.62f ) ) );
		}

		gl::Vector2D doKelvinToXyY( double t )
		{
			double x = 0;
			double y = 0;

			if ( t >= 1667 && t <= 4000 )
			{
				x = ( ( -0.2661239 * pow( 10, 9 ) ) / pow( t, 3 ) ) + ( ( -0.2343580 * pow( 10, 6 ) ) / pow( t, 2 ) ) + ( ( 0.8776956 * pow( 10, 3 ) ) / t ) + 0.179910;
			}
			else if ( t > 4000 && t <= 25000 )
			{
				x = ( ( -3.0258469 * pow( 10, 9 ) ) / pow( t, 3 ) ) + ( ( 2.1070379 * pow( 10, 6 ) ) / pow( t, 2 ) ) + ( ( 0.2226347 * pow( 10, 3 ) ) / t ) + 0.240390;
			}

			if ( t >= 1667 && t <= 2222 )
			{
				y = -1.1063814 * pow( x, 3 ) - 1.34811020 * pow( x, 2 ) + 2.18555832 * x - 0.20219683;
			}
			else if ( t > 2222 && t <= 4000 )
			{
				y = -0.9549476 * pow( x, 3 ) - 1.37418593 * pow( x, 2 ) + 2.09137015 * x - 0.16748867;
			}
			else if ( t > 4000 && t <= 25000 )
			{
				y = 3.0817580 * pow( x, 3 ) - 5.87338670 * pow( x, 2 ) + 3.75112997 * x - 0.37001483;
			}

			return { x, y };
		}

		gl::Vector3D doXyYToXyz( gl::Vector2D const & xyy )
		{
			auto y = ( xyy.y == 0 ) ? 0 : 1;
			auto x = ( xyy.y == 0 ) ? 0 : ( xyy.x * y ) / xyy.y;
			auto z = ( xyy.y == 0 ) ? 0 : ( ( 1 - xyy.x - xyy.y ) * y ) / xyy.y;
			return { x, y, z };
		}

		gl::RgbColour doXyzToSrgb( gl::Vector3D const & xyz )
		{
			auto r = render::makeRangedValue( float( 3.2406 * xyz.x - 1.5372 * xyz.y - 0.4986 * xyz.z ), 0.0f, 1.0f ).value();
			auto g = render::makeRangedValue( float( -0.9689 * xyz.x + 1.8758 * xyz.y + 0.0415 * xyz.z ), 0.0f, 1.0f ).value();
			auto b = render::makeRangedValue( float( 0.0557 * xyz.x - 0.2040 * xyz.y + 1.0570 * xyz.z ), 0.0f, 1.0f ).value();
			return { r, g, b };
		}

		gl::RgbColour doBvToRgb( float bv )
		{
			return doXyzToSrgb( doXyYToXyz( doKelvinToXyY( doBvToKelvin( bv ) ) ) );
		}
	}

	void loadStarsFromCsv( StarMap & starmap, std::string content )
	{
		content = render::replace( content, '\r', "" );
		auto begin = std::begin( content );
		auto end = std::end( content );
		auto lines = render::split( content
			, "\n"
			, std::count( begin, end, '\n' ) + 1
			, false );
		content.clear();

		if ( lines.size() > 1 )
		{
#if !defined( NDEBUG )
			auto linesEnd = lines.begin() + std::min( 10000u, lines.size() );
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
					auto colour = doBvToRgb( ci );
					starmap.add( Star{ name, magnitude,{ ra, dec },colour } );
					line.clear();
				} );
		}
	}
}
