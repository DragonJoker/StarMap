#include "Star.h"

namespace starmap
{
	Star::Star( std::string const & name
		, float magnitude
		, glm::vec2 const & position
		, glm::vec3 const & colour )
		: m_id{ std::hash< std::string >()( name ) }
		, m_name{ name }
		, m_magnitude{ magnitude }
		, m_position{ position }
		, m_colour{ colour }
	{
	}
}
