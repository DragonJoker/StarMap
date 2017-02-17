#include "GestureDetector.h"

#include <RenderLib/RenderLibPrerequisites.h>

namespace utils
{
	//*****************************************************************************

	namespace
	{
		constexpr int32_t DOUBLE_TAP_TIMEOUT = 300 * 1000000;
		constexpr int32_t TAP_TIMEOUT = 180 * 1000000;
		constexpr int32_t DOUBLE_TAP_SLOP = 100;
		constexpr int32_t TOUCH_SLOP = 8;
	}


	//*****************************************************************************

	GestureDetector::GestureDetector( AConfiguration * config )noexcept
		: m_dpFactor{ 160.f / AConfiguration_getDensity( config ) }
	{
	}

	//*****************************************************************************

	TapDetector::TapDetector( AConfiguration * config )noexcept
		: GestureDetector{ config }
	{
	}

	ResultType TapDetector::detect( const AInputEvent * event )
	{
		ResultType ret{ ResultType::eNone };

		if ( AMotionEvent_getPointerCount( event ) == 1 )
		{
			auto action = AMotionEvent_getAction( event );
			uint32_t flags = action & AMOTION_EVENT_ACTION_MASK;

			switch ( flags )
			{
			case AMOTION_EVENT_ACTION_DOWN:
				m_downPointerID = AMotionEvent_getPointerId( event, 0 );
				m_down.x = AMotionEvent_getX( event, 0 );
				m_down.y = AMotionEvent_getY( event, 0 );
				break;

			case AMOTION_EVENT_ACTION_UP:
				{
					int64_t eventTime = AMotionEvent_getEventTime( event );
					int64_t downTime = AMotionEvent_getDownTime( event );
					if ( eventTime - downTime <= TAP_TIMEOUT )
					{
						if ( m_downPointerID == AMotionEvent_getPointerId( event, 0 ) )
						{
							gl::Vector2D position
							{
								AMotionEvent_getX( event, 0 ),
								AMotionEvent_getY( event, 0 ),
							};

							if ( gl::distance( position, m_down ) < TOUCH_SLOP * TOUCH_SLOP * m_dpFactor )
							{
								m_result = position;
								ret = ResultType::eTap;
							}
						}
					}
					break;
				}
			}
		}

		return ret;
	}

	//*****************************************************************************

	DoubleTapDetector::DoubleTapDetector( AConfiguration * config )noexcept
		: GestureDetector{ config }
		, m_tapDetector{ config }
	{
	}

	ResultType DoubleTapDetector::detect( const AInputEvent* event )
	{
		ResultType ret{ ResultType::eNone };

		if ( AMotionEvent_getPointerCount( event ) == 1 )
		{
			auto detectedTap = m_tapDetector.detect( event );
			int32_t action = AMotionEvent_getAction( event );
			uint32_t flags = action & AMOTION_EVENT_ACTION_MASK;

			switch ( flags )
			{
			case AMOTION_EVENT_ACTION_DOWN:
				{
					int64_t eventTime = AMotionEvent_getEventTime( event );

					if ( eventTime - m_lastTapTime <= DOUBLE_TAP_TIMEOUT )
					{
						gl::Vector2D position
						{
							AMotionEvent_getX( event, 0 ),
							AMotionEvent_getY( event, 0 ),
						};

						if ( gl::distance( position, m_lastTap ) < DOUBLE_TAP_SLOP * DOUBLE_TAP_SLOP * m_dpFactor )
						{
							m_result = position;
							ret = ResultType::eDoubleTap;
						}
					}

					break;
				}
			case AMOTION_EVENT_ACTION_UP:
				if ( detectedTap == ResultType::eTap )
				{
					m_lastTapTime = AMotionEvent_getEventTime( event );
					m_lastTap.x = AMotionEvent_getX( event, 0 );
					m_lastTap.y = AMotionEvent_getY( event, 0 );
					ret = ResultType::eTap;
				}
				break;
			}
		}

		return ret;
	}

	//*****************************************************************************

	MoveDetector::MoveDetector( AConfiguration * config, uint32_t index )noexcept
		: GestureDetector{ config }
		, m_index{ index }
	{
	}

	ResultType MoveDetector::detect( const AInputEvent * event )
	{
		ResultType ret{ ResultType::eNone };

		if ( AMotionEvent_getPointerCount( event ) > m_index )
		{
			auto action = AMotionEvent_getAction( event );
			uint32_t flags = action & AMOTION_EVENT_ACTION_MASK;

			switch ( flags )
			{
			case AMOTION_EVENT_ACTION_DOWN:
				m_downPointerID = AMotionEvent_getPointerId( event, m_index );
				m_down.x = AMotionEvent_getX( event, m_index );
				m_down.y = AMotionEvent_getY( event, m_index );
				break;

			case AMOTION_EVENT_ACTION_MOVE:
				{
					auto x = AMotionEvent_getX( event, m_index );
					auto y = AMotionEvent_getY( event, m_index );
					auto action = AMotionEvent_getAction( event );
					auto flags = action & AMOTION_EVENT_ACTION_MASK;

					switch ( flags )
					{
					case AMOTION_EVENT_ACTION_MOVE:
						m_result.x = x - m_down.x;
						m_result.y = y - m_down.y;
						break;
					}

					m_down.x = x;
					m_down.y = y;
					ret = ResultType::eMove;
				}
				break;
			}
		}

		return ret;
	}

	//*****************************************************************************

	DoubleMoveDetector::DoubleMoveDetector( AConfiguration * config )noexcept
		: GestureDetector{ config }
		, m_main{ config, 0u }
		, m_secondary{ config, 1u }
	{
	}

	ResultType DoubleMoveDetector::detect( const AInputEvent * event )
	{
		using render::operator<<;
		ResultType ret{ ResultType::eNone };

		if ( AMotionEvent_getPointerCount( event ) == 2 )
		{
			auto main = m_main.detect( event );
			auto second = m_secondary.detect( event );

			if ( main != ResultType::eNone
				&& second != ResultType::eNone )
			{
				gl::Vector2D diff{ ( m_secondary.position() - m_main.position() ) * m_dpFactor };
				float distance{ gl::length( diff ) };

				if ( main == ResultType::eMove
					&& second == ResultType::eMove )
				{
					if ( m_down.x != 0 || m_down.y != 0 )
					{
						m_result = m_down - diff;
						m_offset = m_distance - distance;
						ret = ResultType::eMove;
					}
				}
				else if ( second == ResultType::eDown )
				{
					m_result = gl::Vector2D{};
					m_offset = 0.0f;
					ret = ResultType::eDown;
				}

				m_down = diff;
				m_distance = distance;
			}
		}

		return ret;
	}

	//*****************************************************************************
}
