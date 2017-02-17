#include "CameraState.h"

namespace render
{
	namespace
	{
		template< typename T >
		RangedValue< T > doUpdateVelocity( RangedValue< T > velocity )
		{
			auto ret = velocity.value() / 2.0f;

			if ( std::abs( float( ret ) ) < 0.2f )
			{
				ret = 0.0_degrees;
			}

			return RangedValue< T >( ret, velocity.range() );
		}
	}

	void CameraState::reset()
	{
		m_zoomVelocity = 0.0_degrees;
		m_velocityX = 0.0_degrees;
		m_velocityY = 0.0_degrees;
		m_fovy = gl::Angle{ 45.0_degrees };
		m_angleX = 0.0_radians;
		m_angleY = 0.0_radians;
	}

	void CameraState::update()
	{
#if !defined( NDEBUG )

		m_angleRange = makeRange( gl::Radians( -gl::PiMult2 ), gl::Radians( gl::PiMult2 ) );
		m_angleX.updateRange( m_angleRange );
		m_angleY.updateRange( m_angleRange );

#endif

		auto scale = m_fovy.percent() + 0.1f;
		m_angleX += to_radians( m_velocityY.value() ) * scale / 2.0f;
		m_angleY += to_radians( m_velocityX.value() ) * scale / 2.0f;
		m_velocityX = doUpdateVelocity( m_velocityX );
		m_velocityY = doUpdateVelocity( m_velocityY );
		m_zoomVelocity = doUpdateVelocity( m_zoomVelocity );
		auto zoom = to_radians( m_zoomVelocity.value() / 2.0f );

		if ( gl::Radians( m_fovy.value() ) > zoom )
		{
			m_fovy = gl::Radians{ m_fovy.value() } - zoom;
		}
	}
}
