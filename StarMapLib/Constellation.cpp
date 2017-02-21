#include "Constellation.h"

#include "ConstellationStar.h"

namespace starmap
{
	Constellation::Constellation( std::string const & name )
		: m_name{ name }
	{
	}

	void Constellation::addStar( uint32_t id
		, std::string const & letter
		, std::string const & name )
	{
		m_stars.emplace_back( id, letter, name, *this );
	}

	void Constellation::fill( StarSet & stars )
	{
		gl::Vec3 position;
		uint32_t count{ 0u };

		for ( auto & star : m_stars )
		{
			auto it = std::find_if( std::begin( stars )
				, std::end( stars )
				, [&star]( Star const & mapStar )
				{
					return star.name() == mapStar.name();
				} );

			if ( it != stars.end() )
			{
				position += it->position();
				star.fill( *it );
				++count;
			}
			else
			{
				std::cerr << "Star [" << star.name() << "] not found." << std::endl;
			}
		}

		m_position = position / count;
	}

	ConstellationStar const * Constellation::findStar( std::string const & name )const
	{
		ConstellationStar const * result = nullptr;
		auto it = std::find_if( std::begin( m_stars )
			, std::end( m_stars )
			, [&name]( ConstellationStar const & star )
			{
				return star.name() == name;
			} );

		if ( it != std::end( m_stars ) )
		{
			result = &( *it );
		}

		return result;
	}

	void Constellation::addLink( std::string const & a
		, std::string const & b )
	{
		auto ita = std::find_if( std::begin( m_stars )
			, std::end( m_stars )
			, [&a]( ConstellationStar const & star )
			{
				return star.letter() == a;
			} );
		auto itb = std::find_if( std::begin( m_stars )
			, std::end( m_stars )
			, [&b]( ConstellationStar const & star )
			{
				return star.letter() == b;
			} );

		if ( ita != m_stars.end()
			&& itb != m_stars.end() )
		{
			m_links.push_back( { &( *ita ), &( *itb ) } );
		}
		else if ( ita == m_stars.end() )
		{
			std::cerr << "Star [" << a << "] not found in constellation [" << m_name << "], can't create a link." << std::endl;

			for ( auto & star : m_stars )
			{
				std::cerr << star.letter() << " - " << star.name() << std::endl;
			}
		}
		else
		{
			std::cerr << "Star [" << b << "] not found in constellation [" << m_name << "], can't create a link." << std::endl;

			for ( auto & star : m_stars )
			{
				std::cerr << star.letter() << " - " << star.name() << std::endl;
			}
		}
	}
}
