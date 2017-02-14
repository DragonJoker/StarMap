#include "ScreenEvents.h"

#include <RenderLib/RenderWindow.h>

#include "StarMap.h"

namespace starmap
{
	ScreenEvents::ScreenEvents( OnScreenTap & onScreenTap
		, OnScreenDoubleTap & onScreenDoubleTap
		, OnScreenSingleMove & onScreenSingleMove
		, OnScreenDoubleMove & onScreenDoubleMove )
		: m_onScreenTap{ onScreenTap.connect( std::bind
			( &ScreenEvents::onScreenTap
			, this
			, std::placeholders::_1 ) ) }
		, m_onScreenDoubleTap{ onScreenDoubleTap.connect( std::bind
			( &ScreenEvents::onScreenDoubleTap
			, this
			, std::placeholders::_1 ) ) }
		, m_onScreenSingleMove{ onScreenSingleMove.connect( std::bind
			( &ScreenEvents::onScreenSingleMove
			, this
			, std::placeholders::_1 ) ) }
		, m_onScreenDoubleMove{ onScreenDoubleMove.connect( std::bind
			( &ScreenEvents::onScreenDoubleMove
			, this
			, std::placeholders::_1
			, std::placeholders::_2 ) ) }
	{
	}

	void ScreenEvents::onScreenTap( gl::Position2D const & coord )
	{
		onPick( coord );
	}

	void ScreenEvents::onScreenDoubleTap( gl::Position2D const & coord )
	{
		onReset();
	}

	void ScreenEvents::onScreenSingleMove( gl::Position2D const & diff )
	{
		onSetVelocity( diff );
	}

	void ScreenEvents::onScreenDoubleMove( gl::Position2D const & diff
		, float distanceOffset )
	{
		onSetZoomVelocity( -distanceOffset );
	}
}
