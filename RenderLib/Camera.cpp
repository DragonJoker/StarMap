#include "Camera.h"

#include <GlLib/Transform.h>

namespace render
{
	Camera::Camera( gl::Size2D const & size )
		: m_viewport{ size }
		, m_frustum{ m_viewport }
	{
	}

	bool Camera::update()noexcept
	{
		bool result = m_changed || m_viewport.changed();

		if ( result )
		{
			gl::Vector3D right{ 1, 0, 0 };
			gl::Vector3D front{ 0, 0, 1 };
			gl::Vector3D up{ 0, 1, 0 };
			right = gl::rotate( m_orientation, right );
			front = gl::rotate( m_orientation, front );
			up = gl::rotate( m_orientation, up );
			m_transform = gl::lookAt( m_position
				, m_position + front
				, up );
			m_frustum.update( m_position, right, up, front );
			m_changed = false;
		}

		return result;
	}

	void Camera::resize( gl::Size2D const & size )noexcept
	{
		m_viewport.resize( size );
	}

	bool Camera::visible( gl::Vector3D const & position )const
	{
		return m_frustum.visible( position );
	}
}
