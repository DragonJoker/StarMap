#include <cmath>

namespace gl
{
	template< typename T >
	Mat4T< T > translate( Mat4T< T > const & m, Vec3T< T > const & v )
	{
		Mat4T< T > result{ m };
		result[3] = m[0] * v[0] + m[1] * v[1] + m[2] * v[2] + m[3];
		return result;
	}

	template< typename T >
	Mat4T< T > scale( Mat4T< T > const & m, Vec3T< T > const & v )
	{
		Mat4T< T > result{ noInit };
		result[0] = m[0] * v[0];
		result[1] = m[1] * v[1];
		result[2] = m[2] * v[2];
		result[3] = m[3];
		return result;
	}

	template< typename T >
	Mat4T< T > rotate( Mat4T< T > const & m, T angle, Vec3T< T > const & v )
	{
		T const a = angle;
		T const c = cos( a );
		T const s = sin( a );

		Vec3T< T > axis( normalize( v ) );
		Vec3T< T > temp( ( T( 1 ) - c ) * axis );

		Mat4T< T > rotate{ noInit };
		rotate[0][0] = c + temp[0] * axis[0];
		rotate[0][1] = temp[0] * axis[1] + s * axis[2];
		rotate[0][2] = temp[0] * axis[2] - s * axis[1];

		rotate[1][0] = temp[1] * axis[0] - s * axis[2];
		rotate[1][1] = c + temp[1] * axis[1];
		rotate[1][2] = temp[1] * axis[2] + s * axis[0];

		rotate[2][0] = temp[2] * axis[0] + s * axis[1];
		rotate[2][1] = temp[2] * axis[1] - s * axis[0];
		rotate[2][2] = c + temp[2] * axis[2];

		Mat4T< T > result( noInit );
		result[0] = m[0] * rotate[0][0] + m[1] * rotate[0][1] + m[2] * rotate[0][2];
		result[1] = m[0] * rotate[1][0] + m[1] * rotate[1][1] + m[2] * rotate[1][2];
		result[2] = m[0] * rotate[2][0] + m[1] * rotate[2][1] + m[2] * rotate[2][2];
		result[3] = m[3];
		return result;
	}
}
