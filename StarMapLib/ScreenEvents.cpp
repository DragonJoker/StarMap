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

	void ScreenEvents::onScreenTap( glm::vec2 const & coord )
	{
		onPick( coord );
	}

	void ScreenEvents::onScreenDoubleTap( glm::vec2 const & coord )
	{
		onReset();
	}

	void ScreenEvents::onScreenSingleMove( glm::vec2 const & diff )
	{
		onSetVelocity( diff );
	}

	void ScreenEvents::onScreenDoubleMove( glm::vec2 const & diff
		, float distanceOffset )
	{
		onSetZoomVelocity( -distanceOffset );
	}
}
