#include "ConstellationStar.h"

namespace starmap
{
	ConstellationStar::ConstellationStar( uint32_t id
		, std::string const & letter
		, std::string const & name
		, Constellation const & constellation )
		: m_id{ id }
		, m_letter{ letter }
		, m_name{ name }
		, m_constellation{ constellation }
	{
	}
}
