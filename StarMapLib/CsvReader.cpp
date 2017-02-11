#include "CsvReader.h"

#include "Star.h"

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
	}

	StarArray loadFromCsv( std::string content )
	{
		StarArray result;
		content = render::replace( content, '\r', "" );
		auto begin = std::begin( content );
		auto end = std::end( content );
		auto lines = render::split( content
			, "\n"
			, std::count( begin, end, '\n' ) + 1
			, false );

		for ( auto & line : lines )
		{
			auto values = render::split( line, ";", 4, true );

			if ( values.size() == 4 )
			{
				auto name = render::replace( values[0], "\"", "" );
				auto magnitude = doParseFloat( values[1] );
				auto ra = doParseFloat( values[2] );
				auto dec = doParseFloat( values[3] );
				result.push_back( { name, magnitude,{ ra, dec },{ 0.8, 0.8, 1.0 } } );
			}
		}

		return result;
	}
}
