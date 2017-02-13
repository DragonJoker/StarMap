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

	void Constellation::addStar( std::string const & letter
		, std::string const & name )
	{
		static std::hash< std::string > hash;

		if ( !letter.empty()
			&& !name.empty() )
		{
			auto id = hash( name );
			auto it = std::find_if( std::begin( m_stars )
				, std::end( m_stars )
				, [&id]( Star const & star )
			{
				return star.id() == id;
			} );

			if ( it != std::end( m_stars ) )
			{
				m_letters.insert( { letter, &( *it ) } );
			}
			else
			{
				std::cerr << "Star [" << name << "] was not found." << std::endl;
			}
		}
	}

	void Constellation::addLink( std::string const & a
		, std::string const & b )
	{
		auto sa = m_letters.find( a );
		auto sb = m_letters.find( b );

		if ( sa != m_letters.end()
			&& sb != m_letters.end() )
		{
			m_links.push_back( { sa->second, sb->second } );
		}
	}
}
