#include "Constellation.h"

#include "Star.h"

namespace starmap
{
	Constellation::Constellation( std::string const & name
		, StarSet const & stars )
		: m_name{ name }
		, m_stars{ stars }
	{
	}

	void Constellation::AddStar( std::string const & letter
		, std::string const & name )
	{
		if ( !letter.empty()
			&& !name.empty() )
		{
			m_letters.insert( { letter, name } );
		}
	}

	void Constellation::AddLink( std::string const & a
		, std::string const & b )
	{
		static std::hash< std::string > hash;
		auto na = m_letters.find( a );
		auto nb = m_letters.find( b );

		if ( na != m_letters.end()
			&& nb != m_letters.end() )
		{
			auto ida = hash( na->second );
			auto idb = hash( nb->second );
			auto ita = std::find_if( std::begin( m_stars )
				, std::end( m_stars )
				, [&ida]( Star const & star )
			{
				return star.getId() == ida;
			} );
			auto itb = std::find_if( std::begin( m_stars )
				, std::end( m_stars )
				, [&idb]( Star const & star )
			{
				return star.getId() == idb;
			} );

			if ( ita != std::end( m_stars )
				&& itb != std::end( m_stars ) )
			{
				m_links.push_back( { &( *ita ), &( *itb ) } );
			}
			else if ( ita == std::end( m_stars ) )
			{
				std::cerr << "Star [" << m_letters[a] << "] not found." << std::endl;
			}
			else
			{
				std::cerr << "Star [" << m_letters[b] << "] not found." << std::endl;
			}
		}
	}
}
