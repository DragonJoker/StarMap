#include "OverlayCategory.h"

#include "BorderPanelOverlay.h"
#include "OverlayRenderer.h"
#include "PanelOverlay.h"
#include "TextOverlay.h"

#include <GlLib/Transform.h>

namespace render
{
	Overlay::Overlay( Type p_type )
		: m_type( p_type )
	{
	}

	Overlay::~Overlay()
	{
	}

	void Overlay::update()
	{
		if ( m_positionChanged )
		{
			doUpdatePosition();
		}

		doUpdate();
	}

	void Overlay::doUpdatePosition()
	{
		if ( m_positionChanged )
		{
			m_transform = gl::Matrix4x4{ 1 };
			m_transform = gl::translate( m_transform
				, gl::Vector3D{ m_position.x, m_position.y, 0 } );
			m_positionChanged = false;
		}
	}
}
