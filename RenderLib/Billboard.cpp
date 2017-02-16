#include "Billboard.h"

namespace render
{
	Billboard::Billboard( std::string const & name, BillboardBuffer & buffer )
		: Movable{ name }
		, m_buffer{ buffer }
	{
	}
}
