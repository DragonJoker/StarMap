#include "Star.h"

namespace starmap
{
	namespace
	{
		gl::Vector3D doSphericalToCartesian( gl::Vector2D const & coord
			, float distance = 100.0f )
		{
			auto const sint = sin( coord.x );
			auto const cost = cos( coord.x );
			auto const sinp = sin( coord.y );
			auto const cosp = cos( coord.y );
			return gl::Vector3D
			{
				distance * sinp * cost,
				distance * sinp * sint,
				distance * cosp
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
}
