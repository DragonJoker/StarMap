#include "Star.h"

namespace starmap
{
	namespace
	{
		gl::Vector3D doSphericalToCartesian( gl::Vector2D const & coord
			, float distance = 100.0f )
		{
			auto const & ra = coord.x;
			auto const & dec = coord.y;
			auto const sinra = sin( ra );
			auto const cosra = cos( ra );
			auto const sindec = sin( dec );
			auto const cosdec = cos( dec );
			return gl::Vector3D
			{
				distance * cosdec * cosra,
				distance * cosdec * sinra,
				distance * sindec
			};
		}
	}

	Star::Star( std::string const & name
		, float magnitude
		, gl::Vector2D const & position
		, gl::RgbColour const & colour )
		: m_id{ std::hash< std::string >()( name ) }
		, m_name{ name }
		, m_magnitude{ magnitude }
		, m_position{ doSphericalToCartesian( position ) }
		, m_colour{ colour }
	{
	}

	Star::Star( std::string const & name
		, float magnitude
		, gl::Vector3D const & position
		, gl::RgbColour const & colour )
		: m_id{ std::hash< std::string >()( name ) }
		, m_name{ name }
		, m_magnitude{ magnitude }
		, m_position{ glm::normalize( position ) * 100.0f }
		, m_colour{ colour }
	{
	}
}
