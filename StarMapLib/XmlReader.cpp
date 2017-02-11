#include "XmlReader.h"

#include "StarMap.h"

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
					std::string letter{ lAttrib->value()
						, lAttrib->value() + lAttrib->value_size() };
					std::string name{ nAttrib->value()
						, nAttrib->value() + nAttrib->value_size() };
					constellation.AddStar( letter, name );
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
					std::string nameA{ aAttrib->value()
						, aAttrib->value() + aAttrib->value_size() };
					std::string nameB{ bAttrib->value()
						, bAttrib->value() + bAttrib->value_size() };
					constellation.AddLink( nameA, nameB );
				}
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
				std::string name{ nAttrib->value()
					, nAttrib->value() + nAttrib->value_size() };
				auto & constellation = starmap.createConstellation( name );
				doLoadStars( constellation, cNode );
				doLoadLinks( constellation, cNode );
			}
		}
	}
}
