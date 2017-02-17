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
				auto pAttrib = sNode->first_attribute( "proper" );
				auto gAttrib = sNode->first_attribute( "gl" );
				auto hAttrib = sNode->first_attribute( "hip" );
				auto mAttrib = sNode->first_attribute( "mag" );
				auto dAttrib = sNode->first_attribute( "decrad" );
				auto rAttrib = sNode->first_attribute( "rarad" );
				auto sAttrib = sNode->first_attribute( "spect" );
				auto cAttrib = sNode->first_attribute( "ci" );
				std::string name{ doParseString( pAttrib ) };

				if ( name.empty() )
				{
					name = doParseString( gAttrib );
				}

				if ( name.empty() )
				{
					name = doParseString( hAttrib );
				}

				auto magnitude = doParseFloat( mAttrib );
				auto ra = doParseFloat( rAttrib );
				auto dec = doParseFloat( dAttrib );
				auto ci = doParseFloat( cAttrib );
				auto colour = doBvToRgb( ci );
				starmap.add( Star{ name, magnitude,{ ra, dec },colour } );
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
