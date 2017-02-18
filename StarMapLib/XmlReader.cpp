#include "XmlReader.h"

#include "StarMap.h"

#include <RenderLib/StringUtils.h>

#define RAPIDXML_NO_EXCEPTIONS
#include "rapidxml/rapidxml.hpp"

namespace rapidxml
{
	void parse_error_handler( char const * what, void * where )
	{
		std::cerr << "Parse error: " << what << std::endl;
	}
}

namespace starmap
{
	namespace
	{
		std::string doParseString( rapidxml::xml_attribute< char > * node )
		{
			return std::string{ node->value()
				, node->value() + node->value_size() };
		}

		float doParseFloat( rapidxml::xml_attribute< char > * node )
		{
			auto text = doParseString( node );
			text = render::replace( text, "\"", "" );
			text = render::replace( text, ",", "." );
			std::stringstream stream{ text };
			float result;
			stream >> result;
			return result;
		}

		void doLoadStars( Constellation & constellation
			, rapidxml::xml_node<> * node )
		{
			for ( auto llNode = node->first_node( "StarList" );
				llNode;
				llNode = llNode->next_sibling() )
			{
				for ( auto lNode = llNode->first_node( "Star" );
					lNode;
					lNode = lNode->next_sibling() )
				{
					auto lAttrib = lNode->first_attribute( "Letter" );
					auto nAttrib = lNode->first_attribute( "Name" );
					std::string letter{ doParseString( lAttrib ) };
					std::string name{ doParseString( nAttrib ) };
					constellation.addStar( letter, name );
				}
			}
		}

		void doLoadLinks( Constellation & constellation
			, rapidxml::xml_node<> * node )
		{
			for ( auto llNode = node->first_node( "LinkList" );
				llNode;
				llNode = llNode->next_sibling() )
			{
				for ( auto lNode = llNode->first_node( "Link" );
					lNode;
					lNode = lNode->next_sibling() )
				{
					auto aAttrib = lNode->first_attribute( "StarA" );
					auto bAttrib = lNode->first_attribute( "StarB" );
					std::string nameA{ doParseString( aAttrib ) };
					std::string nameB{ doParseString( bAttrib ) };
					constellation.addLink( nameA, nameB );
				}
			}
		}
	}

	void loadStarsFromXml( StarMap & starmap, std::string content )
	{
		rapidxml::xml_document<> doc;
		doc.parse< rapidxml::parse_non_destructive >( &content[0] );

		for ( auto slNode = doc.first_node( "StarList" );
			slNode;
			slNode = slNode->next_sibling() )
		{
			for ( auto sNode = slNode->first_node( "Star" );
				sNode;
				sNode = sNode->next_sibling() )
			{
				auto name = doParseString( sNode->first_attribute( "proper" ) );

				if ( name.empty() )
				{
					name = doParseString( sNode->first_attribute( "gl" ) );
				}

				if ( name.empty() )
				{
					name = doParseString( sNode->first_attribute( "hip" ) );
				}

				auto mag = doParseFloat( sNode->first_attribute( "mag" ) );
				auto dec = doParseFloat( sNode->first_attribute( "decrad" ) );
				auto ra = doParseFloat( sNode->first_attribute( "rarad" ) );
				auto ci = doParseFloat( sNode->first_attribute( "ci" ) );
				auto colour = bvToRgb( ci );
				sNode->first_attribute( "spect" );
				sNode->first_attribute( "con" );
				starmap.add( Star{ name, mag, gl::Vector2D{ ra, dec },colour } );
			}
		}
	}

	void loadConstellationsFromXml( StarMap & starmap, std::string content )
	{
		rapidxml::xml_document<> doc;
		doc.parse< rapidxml::parse_non_destructive >( &content[0] );

		for ( auto clNode = doc.first_node( "ConstellationList" );
			clNode;
			clNode = clNode->next_sibling() )
		{
			for ( auto cNode = clNode->first_node( "Constellation" );
				cNode;
				cNode = cNode->next_sibling() )
			{
				auto nAttrib = cNode->first_attribute( "Name" );
				std::string name{ doParseString( nAttrib ) };
				auto & constellation = starmap.createConstellation( name );
				doLoadStars( constellation, cNode );
				doLoadLinks( constellation, cNode );
			}
		}
	}
}
