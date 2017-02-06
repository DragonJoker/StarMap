#include "CameraState.h"

namespace starmap
{
	namespace
	{
		float toRadians( float degrees )
		{
			return float( degrees * M_PI / 180.0f );
		}

		float clamp( float value, float min, float max )
		{
			if ( value < min )
			{
				return min;
			}
			else if ( value > max )
			{
				return max;
			}

			return value;
		}

		float doUpdateVelocity( float velocity )
		{
			float ret = velocity / 2;

			if ( std::abs( ret ) < 0.2 )
			{
				ret = 0;
			}

			return ret;
		}

		glm::vec2 doUpdateVelocity( glm::vec2 const & velocity )
		{
			return glm::vec2
			{
				doUpdateVelocity( velocity.x ),
				doUpdateVelocity( velocity.y )
			};
		}
	}

	void CameraState::reset()
	{
		m_zoomVelocity = 0.0f;
		m_velocity = glm::vec2{};
		m_zoom = 0.0f;
		m_angle = glm::vec2{};
	}

	void CameraState::update()
	{
		m_velocity.x = clamp( m_velocity.x, -5, 5 );
		m_velocity.y = clamp( m_velocity.y, -5, 5 );

		m_zoomVelocity = clamp( m_zoomVelocity, -2, 2 );

		m_angle.x += toRadians( m_velocity.y ) / 2.0f;
		m_angle.y += toRadians( m_velocity.x ) / 2.0f;

		m_angle.x = clamp( m_angle.x, -M_PI / 6, M_PI / 6 );
		m_angle.y = clamp( m_angle.y, -M_PI / 6, M_PI / 6 );

		m_velocity = doUpdateVelocity( m_velocity );
		m_zoomVelocity = doUpdateVelocity( m_zoomVelocity );
		m_zoom += m_zoomVelocity;
	}
}
