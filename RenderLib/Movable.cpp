#include "Movable.h"

#include <GlLib/glm/gtc/matrix_transform.hpp>
#include <GlLib/glm/gtx/quaternion.hpp>

namespace render
{
	Movable::Movable( std::string const & name )noexcept
		: m_name{ name }
	{
	}

	void Movable::update()noexcept
	{
		gl::Matrix4x4 translate{ 1.0f };
		translate = glm::translate( translate, m_position );
		gl::Matrix4x4 scale{ 1.0f };
		scale = glm::scale( scale, m_scale );
		gl::Matrix4x4 rotate = glm::toMat4( m_orientation );
		m_transform = translate * rotate * scale;
	}
}
