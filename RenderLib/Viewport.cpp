#include "Viewport.h"

#include <GlLib/glm/gtc/matrix_transform.hpp>

namespace render
{
	Viewport::Viewport( gl::Size2D const & size )noexcept
		: m_size{ size }
	{
	}

	void Viewport::resize( gl::Size2D const & size )noexcept
	{
		m_size = size;
	}

	void Viewport::ortho( float left
		, float right
		, float top
		, float bottom
		, float near
		, float far )noexcept
	{
		m_projection = glm::ortho( left, right, bottom, top, near, far );
	}

	void Viewport::fovY( Angle const & fovy )noexcept
	{
		m_changed = m_fovy != fovy;

		if ( m_changed )
		{
			m_fovy = fovy;
			float aspect = float( m_size.x ) / m_size.y;
			m_projection = glm::infinitePerspective( float( Radians( m_fovy ) ), aspect, 0.1f );
		}
	}

	void Viewport::apply()const noexcept
	{
		glViewport( 0, 0, GLsizei( m_size.x ), GLsizei( m_size.y ) );
	}
}
