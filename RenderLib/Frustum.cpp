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
		, gl::Vector3D const & right
		, gl::Vector3D const & up
		, gl::Vector3D const & front )
	{
		// Retrieve near and far planes' dimensions
		float const ratio{ float( m_viewport.size().x ) / m_viewport.size().y };
		float const tan = float( gl::tan( gl::Radians{ m_viewport.fovY() / 2 } ) );
		float const near{ 0.1f };
		float const far{ 10000.0f };
		float const nearH = 2 * tan * near;
		float const nearW = nearH * ratio;
		float const farH = 2 * tan * far;
		float const farW = farH * ratio;

		// Compute planes' points
		// N => Near
		// F => Far
		// C => Center
		// T => Top
		// L => Left
		// R => Right
		// B => Bottom
		gl::Vector3D const rn{ right * nearW / 2 };
		gl::Vector3D const rf{ right * farW / 2 };
		gl::Vector3D const tn{ up * nearH / 2 };
		gl::Vector3D const tf{ up * farH / 2 };
		gl::Vector3D const nc{ position + front * near };
		gl::Vector3D const ntl{ nc + tn - rn };
		gl::Vector3D const ntr{ nc + tn + rn };
		gl::Vector3D const nbl{ nc - tn - rn };
		gl::Vector3D const nbr{ nc - tn + rn };
		gl::Vector3D const fc{ position + front * far };
		gl::Vector3D const ftl{ fc + tf - rf };
		gl::Vector3D const ftr{ fc + tf + rf };
		gl::Vector3D const fbl{ fc - tf - rf };
		gl::Vector3D const fbr{ fc - tf + rf };

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
