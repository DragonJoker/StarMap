#include "XmlReader.h"

#include "StarMap.h"

#include <RenderLib/StringUtils.h>

#define RAPIDXML_NO_EXCEPTIONS
#include "rapidxml/rapidxml.hpp"

#include <regex>

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
			float result{ 0.0f };
			stream >> result;
			return result;
		}

		int doParseInt( std::string text )
		{
			std::stringstream stream{ text };
			int result{ 0 };
			stream >> result;
			return result;
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

		bool doExtractFromBF( std::string bf
			, uint32_t & id
			, uint32_t & index
			, std::string & letter )
		{
			bool result = false;
			std::regex regexLetter{ R"((\d*)[^a-zA-Z]*([a-zA-Z]*)(\d*).*)" };
			bf = bf.substr( 0u, bf.size() - 3u );
			auto begin = std::begin( bf );
			auto end = std::end( bf );
			const std::sregex_iterator endit;
			std::sregex_iterator it( begin, end, regexLetter );

			if ( it != endit )
			{
				std::smatch match = *it;
				letter = match[2].str();
				auto sid = match[1].str();
				auto sindex = match[3].str();

				if ( !sid.empty()
					|| !letter.empty()
					|| !sindex.empty() )
				{
					id = doParseInt( sid );
					index = doParseInt( sindex );
					result = true;
				}
			}

			return result;
		}

		bool doExtractFromBayer( std::string const & bayer
			, uint32_t & index
			, std::string & letter )
		{
			bool result = false;
			std::regex regexLetter{ R"(([a-zA-Z]*)-?(\d*))" };
			auto begin = std::begin( bayer );
			auto end = std::end( bayer );
			const std::sregex_iterator endit;
			std::sregex_iterator it( begin, end, regexLetter );

			if ( it != endit )
			{
				std::smatch match = *it;
				letter = match[1].str();
				auto sindex = match[2].str();

				if ( !letter.empty()
					|| !sindex.empty() )
				{
					index = doParseInt( sindex );
					result = true;
				}
			}

			return result;
		}

		void doPreAddConstellation( std::string bf
			, std::string const & bayer
			, std::string const & constellation
			, std::string const & star
			, ConstellationMap & constellations )
		{
			static std::map< std::string, std::string > const letters
			{
				{ "Alp", "Alpha" },
				{ "Bet", "Beta" },
				{ "Gam", "Gamma" },
				{ "Del", "Delta" },
				{ "Eps", "Epsilon" },
				{ "Zet", "Zeta" },
				{ "Eta", "Eta" },
				{ "The", "Theta" },
				{ "Iot", "Iota" },
				{ "Kap", "Kappa" },
				{ "Lam", "Lambda" },
				{ "Mu", "Mu" },
				{ "Nu", "Nu" },
				{ "Ksi", "Ksi" },
				{ "Omi", "Omicron" },
				{ "Pi", "Pi" },
				{ "Rho", "Rho" },
				{ "Sig", "Sigma" },
				{ "Tau", "Tau" },
				{ "Ups", "Upsilon" },
				{ "Phi", "Phi" },
				{ "Chi", "Chi" },
				{ "Psi", "Psi" },
				{ "Ome", "Omega" },
				{ "Xi", "Xi" },
			};
			static std::map< std::string, std::string > const names
			{
				{ "And", "Andromeda" },
				{ "Ant", "" },
				{ "Aps", "" },
				{ "Ara", "Ara" },
				{ "Ari", "Aries" },
				{ "Aql", "" },
				{ "Aqr", "Aquarius" },
				{ "Aur", "" },
				{ "Boo", "Bootes" },
				{ "Cae", "" },
				{ "Cam", "" },
				{ "Cap", "" },
				{ "Car", "" },
				{ "Cas", "Cassiopea" },
				{ "Cnc", "" },
				{ "Cen", "Centaurus" },
				{ "Cep", "" },
				{ "Cet", "" },
				{ "Cha", "" },
				{ "Cir", "" },
				{ "CMa", "Canis Majoris" },
				{ "CMi", "Canis Minor" },
				{ "Col", "" },
				{ "Com", "" },
				{ "CrA", "" },
				{ "CrB", "" },
				{ "Crt", "" },
				{ "Cru", "" },
				{ "Crv", "" },
				{ "CVn", "" },
				{ "Cyg", "Cygnus" },
				{ "Del", "" },
				{ "Dor", "" },
				{ "Dra", "Dragonis" },
				{ "Equ", "Equus" },
				{ "Eri", "" },
				{ "For", "" },
				{ "Gem", "Gemini" },
				{ "Gru", "" },
				{ "Her", "" },
				{ "Hya", "" },
				{ "Hyi", "" },
				{ "Hor", "" },
				{ "Ind", "" },
				{ "Lac", "" },
				{ "Leo", "Leo" },
				{ "Lep", "" },
				{ "Lib", "Libra" },
				{ "LMi", "" },
				{ "Lup", "Lupus" },
				{ "Lyn", "" },
				{ "Lyr", "Lyra" },
				{ "Men", "Mensa" },
				{ "Mic", "" },
				{ "Mon", "" },
				{ "Mus", "" },
				{ "Nor", "Norma" },
				{ "Oct", "" },
				{ "Oph", "" },
				{ "Ori", "" },
				{ "Pav", "" },
				{ "Peg", "Pegasus" },
				{ "Per", "" },
				{ "Phe", "" },
				{ "Pic", "" },
				{ "PsA", "Piscis Austrinus" },
				{ "Psc", "Pisces" },
				{ "Pyx", "" },
				{ "Pup", "Puppis" },
				{ "Ret", "Reticulum" },
				{ "Scl", "Sculptor" },
				{ "Sco", "Scorpio" },
				{ "Sct", "" },
				{ "Ser", "Serpens" },
				{ "Sex", "Sextant" },
				{ "Sge", "" },
				{ "Sgr", "Sagittarius" },
				{ "Tau", "Taurus" },
				{ "Tel", "Telescopium" },
				{ "TrA", "Triangulum Australe" },
				{ "Tri", "Triangulum" },
				{ "Tuc", "Tucana" },
				{ "UMa", "Ursa Major" },
				{ "UMi", "Ursa Minor" },
				{ "Vel", "Vela" },
				{ "Vir", "Virgo" },
				{ "Vol", "Volans" },
				{ "Vul", "" },
			};
			auto itn = names.find( constellation );

			if ( itn != names.end() )
			{
				if ( !itn->second.empty() )
				{
					auto itc = constellations.emplace( itn->second
						, std::make_unique< Constellation >( itn->second ) ).first;
					uint32_t id{ 0u };
					uint32_t index{ 0u };
					std::string letter;

					if ( !doExtractFromBF( bf, id, index, letter )
						|| index == 0 )
					{
						doExtractFromBayer( bayer, index, letter );
					}

					if ( !letter.empty()
						|| id != 0 )
					{
						auto itl = letters.find( letter );

						if ( itl != letters.end() )
						{
							if ( index != 0 )
							{
								std::stringstream stream;
								stream << index;
								itc->second->addStar( id, itl->second + stream.str(), star );
							}
							else
							{
								itc->second->addStar( id, itl->second, star );
							}
						}
						else
						{
							std::stringstream stream;
							stream << id;
							itc->second->addStar( id, stream.str(), star );
						}
					}
				}
			}
		}
	}

	void loadStarsFromXml( StarMap & starmap, std::string content )
	{
		rapidxml::xml_document<> doc;
		doc.parse< rapidxml::parse_non_destructive >( &content[0] );
		ConstellationMap constellations;

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
				starmap.add( Star{ name, mag, gl::Vec2{ ra, dec },colour } );
				auto bf = doParseString( sNode->first_attribute( "bf" ) );
				auto bayer = doParseString( sNode->first_attribute( "bayer" ) );
				auto con = doParseString( sNode->first_attribute( "con" ) );

				if ( !bf.empty()
					&& !con.empty()
					&& bf.find( con ) != std::string::npos )
				{
					doPreAddConstellation( bf, bayer, con, name, constellations );
				}
			}
		}

		for ( auto & cons : constellations )
		{
			starmap.add( *cons.second );
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
				auto constellation = starmap.findConstellation( name );

				if ( constellation )
				{
					doLoadLinks( *constellation, cNode );
				}
			}
		}
	}
}
