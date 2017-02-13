#include "Star.h"

namespace starmap
{
	namespace
	{
		glm::vec3 doSphericalToCartesian( glm::vec2 const & coord
			, float distance = 100.0f )
		{
			auto const sint = sin( coord.x );
			auto const cost = cos( coord.x );
			auto const sinp = sin( coord.y );
			auto const cosp = cos( coord.y );
			return glm::vec3
			{
				distance * sinp * cost,
				distance * sinp * sint,
				distance * cosp
			};
		}
	}

	Star::Star( std::string const & name
		, float magnitude
		, glm::vec2 const & position
		, glm::vec3 const & colour )
		: m_id{ std::hash< std::string >()( name ) }
		, m_name{ name }
		, m_magnitude{ magnitude }
		, m_position{ doSphericalToCartesian( position ) }
		, m_colour{ colour }
	{
	}
}
