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

		//gl::RgbColour doBvToRgb( float bv )
		//{
		//	return doXyzToSrgb( doXyYToXyz( doKelvinToXyY( doBvToKelvin( bv ) ) ) );
		//}

		gl::RgbColour doBvToRgb( float bv )
		{
			struct BvToRgb
			{
				float bv;
				gl::RgbColour rgb;
			};
			static std::vector< BvToRgb > const ConversionTable
			{
				{  -0.32f, { 155 / 255.0f, 176 / 255.0f, 255 / 255.0f } },
				{  -0.30f, { 162 / 255.0f, 184 / 255.0f, 255 / 255.0f } },
				{  -0.28f, { 157 / 255.0f, 177 / 255.0f, 255 / 255.0f } },
				{  -0.26f, { 157 / 255.0f, 177 / 255.0f, 255 / 255.0f } },
				{  -0.24f, { 154 / 255.0f, 178 / 255.0f, 255 / 255.0f } },
				{  -0.22f, { 164 / 255.0f, 186 / 255.0f, 255 / 255.0f } },
				{  -0.20f, { 156 / 255.0f, 178 / 255.0f, 255 / 255.0f } },
				{ -0.185f, { 167 / 255.0f, 188 / 255.0f, 255 / 255.0f } },
				{  -0.17f, { 160 / 255.0f, 182 / 255.0f, 255 / 255.0f } },
				{  -0.16f, { 160 / 255.0f, 180 / 255.0f, 255 / 255.0f } },
				{  -0.15f, { 165 / 255.0f, 185 / 255.0f, 255 / 255.0f } },
				{  -0.14f, { 164 / 255.0f, 184 / 255.0f, 255 / 255.0f } },
				{  -0.12f, { 170 / 255.0f, 191 / 255.0f, 255 / 255.0f } },
				{  -0.10f, { 172 / 255.0f, 189 / 255.0f, 255 / 255.0f } },
				{  -0.08f, { 173 / 255.0f, 191 / 255.0f, 255 / 255.0f } },
				{  -0.04f, { 177 / 255.0f, 195 / 255.0f, 255 / 255.0f } },
				{  -0.02f, { 181 / 255.0f, 198 / 255.0f, 255 / 255.0f } },
				{   0.00f, { 185 / 255.0f, 201 / 255.0f, 255 / 255.0f } },
				{   0.03f, { 181 / 255.0f, 199 / 255.0f, 255 / 255.0f } },
				{   0.06f, { 187 / 255.0f, 203 / 255.0f, 255 / 255.0f } },
				{   0.07f, { 191 / 255.0f, 207 / 255.0f, 255 / 255.0f } },
				{   0.12f, { 202 / 255.0f, 215 / 255.0f, 255 / 255.0f } },
				{   0.15f, { 199 / 255.0f, 212 / 255.0f, 255 / 255.0f } },
				{   0.18f, { 200 / 255.0f, 213 / 255.0f, 255 / 255.0f } },
				{   0.21f, { 213 / 255.0f, 222 / 255.0f, 255 / 255.0f } },
				{   0.24f, { 219 / 255.0f, 224 / 255.0f, 255 / 255.0f } },
				{   0.31f, { 224 / 255.0f, 229 / 255.0f, 255 / 255.0f } },
				{   0.35f, { 236 / 255.0f, 239 / 255.0f, 255 / 255.0f } },
				{   0.39f, { 224 / 255.0f, 226 / 255.0f, 255 / 255.0f } },
				{   0.43f, { 248 / 255.0f, 247 / 255.0f, 255 / 255.0f } },
				{   0.47f, { 244 / 255.0f, 241 / 255.0f, 255 / 255.0f } },
				{   0.50f, { 246 / 255.0f, 243 / 255.0f, 255 / 255.0f } },
				{   0.53f, { 255 / 255.0f, 247 / 255.0f, 252 / 255.0f } },
				{   0.56f, { 255 / 255.0f, 247 / 255.0f, 252 / 255.0f } },
				{   0.59f, { 255 / 255.0f, 248 / 255.0f, 252 / 255.0f } },
				{   0.61f, { 255 / 255.0f, 247 / 255.0f, 248 / 255.0f } },
				{   0.64f, { 255 / 255.0f, 245 / 255.0f, 242 / 255.0f } },
				{   0.66f, { 255 / 255.0f, 241 / 255.0f, 229 / 255.0f } },
				{   0.69f, { 255 / 255.0f, 244 / 255.0f, 234 / 255.0f } },
				{   0.71f, { 255 / 255.0f, 244 / 255.0f, 235 / 255.0f } },
				{   0.74f, { 255 / 255.0f, 244 / 255.0f, 235 / 255.0f } },
				{   0.76f, { 255 / 255.0f, 237 / 255.0f, 222 / 255.0f } },
				{   0.79f, { 255 / 255.0f, 239 / 255.0f, 221 / 255.0f } },
				{   0.82f, { 255 / 255.0f, 238 / 255.0f, 221 / 255.0f } },
				{   0.90f, { 255 / 255.0f, 224 / 255.0f, 188 / 255.0f } },
				{   0.98f, { 255 / 255.0f, 227 / 255.0f, 196 / 255.0f } },
				{   1.06f, { 255 / 255.0f, 222 / 255.0f, 195 / 255.0f } },
				{   1.14f, { 255 / 255.0f, 216 / 255.0f, 181 / 255.0f } },
				{   1.22f, { 255 / 255.0f, 210 / 255.0f, 161 / 255.0f } },
				{   1.30f, { 255 / 255.0f, 199 / 255.0f, 142 / 255.0f } },
				{   1.36f, { 255 / 255.0f, 209 / 255.0f, 174 / 255.0f } },
				{   1.41f, { 255 / 255.0f, 195 / 255.0f, 139 / 255.0f } },
				{   1.50f, { 255 / 255.0f, 204 / 255.0f, 142 / 255.0f } },
				{   1.59f, { 255 / 255.0f, 196 / 255.0f, 131 / 255.0f } },
				{   1.68f, { 255 / 255.0f, 206 / 255.0f, 129 / 255.0f } },
				{   1.77f, { 255 / 255.0f, 201 / 255.0f, 127 / 255.0f } },
				{   1.86f, { 255 / 255.0f, 204 / 255.0f, 111 / 255.0f } },
				{   1.95f, { 255 / 255.0f, 195 / 255.0f, 112 / 255.0f } },
				{   2.00f, { 255 / 255.0f, 198 / 255.0f, 109 / 255.0f } }
			};
			auto it = std::find_if( std::begin( ConversionTable )
				, std::end( ConversionTable )
				, [&bv]( BvToRgb const & conv )
				{
					return conv.bv >= bv;
				} );

			if ( it != std::end( ConversionTable ) )
			{
				return it->rgb;
			}
			else
			{
				return ConversionTable.back().rgb;
			}
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
					auto colour = doBvToRgb( ci );
					starmap.add( Star{ name, magnitude,{ ra, dec },colour } );
					line.clear();
				} );
		}
	}
}
