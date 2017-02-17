#include "Frustum.h"

#include "Viewport.h"

namespace render
{
	//*************************************************************************

	Frustum::Frustum( Viewport & viewport )
		: m_viewport{ viewport }
	{
	}

	void Frustum::update( gl::Vector3D const & position
		, gl::Vector3D const & x
		, gl::Vector3D const & y
		, gl::Vector3D const & z )
	{
		// Retrieve near and far planes' dimensions
		float farHeight{ 0.0f };
		float farWidth{ 0.0f };
		float nearHeight{ 0.0f };
		float nearWidth{ 0.0f };
		float ratio{ float( m_viewport.size().x ) / m_viewport.size().y };
		float near{ 0.1f };
		float far{ 10000.0f };

		float tan = float( gl::tan( gl::Radians{ m_viewport.fovY() } ) );
		nearHeight = tan * near;
		nearWidth = nearHeight * ratio;
		farHeight = tan * far;
		farWidth = farHeight * ratio;

		// Compute planes' points
		// N => Near
		// F => Far
		// C => Center
		// T => Top
		// L => Left
		// R => Right
		// B => Bottom
		gl::Vector3D rn{ x * nearWidth };
		gl::Vector3D rf{ x * farWidth };
		gl::Vector3D tn{ y * nearHeight };
		gl::Vector3D tf{ y * farHeight };
		gl::Vector3D nc{ position + z * near };
		gl::Vector3D ntl{ nc + tn - rn };
		gl::Vector3D ntr{ nc + tn + rn };
		gl::Vector3D nbl{ nc - tn - rn };
		gl::Vector3D nbr{ nc - tn + rn };
		gl::Vector3D fc{ position + z * far };
		gl::Vector3D ftl{ fc + tf - rf };
		gl::Vector3D ftr{ fc + tf + rf };
		gl::Vector3D fbl{ fc - tf - rf };
		gl::Vector3D fbr{ fc - tf + rf };

		// Fill planes
		m_planes[size_t( FrustumPlane::eNear )].set( ntl, ntr, nbr );
		m_planes[size_t( FrustumPlane::eFar )].set( ftr, ftl, fbl );
		m_planes[size_t( FrustumPlane::eLeft )].set( ntl, nbl, fbl );
		m_planes[size_t( FrustumPlane::eRight )].set( nbr, ntr, fbr );
		m_planes[size_t( FrustumPlane::eTop )].set( ntr, ntl, ftl );
		m_planes[size_t( FrustumPlane::eBottom )].set( nbl, nbr, fbr );
	}

	bool Frustum::visible( gl::Vector3D const & point )const
	{
		//see http://www.lighthouse3d.com/tutorials/view-frustum-culling/
		auto it = std::find_if( m_planes.begin()
			, m_planes.end()
			, [&point]( auto const & plane )
			{
				return plane.distance( point ) < 0;
			} );

		return it == m_planes.end();
	}

	//*************************************************************************
}
