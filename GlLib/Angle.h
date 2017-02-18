/**
*\file
*	AngleT< T >.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlLib_Angle_HPP___
#define ___GlLib_Angle_HPP___
#pragma once

#include <math.h>
#include <limits>

namespace gl
{
	//!Constante pi.
	static constexpr long double Pi = 3.1415926535897932384626433832795028841968;
	//!Constante pi * 2.
	static constexpr long double PiMult2 = Pi * 2;
	//!Constante pi / 2.
	static constexpr long double PiDiv2 = Pi / 2;
	//!Constante de conversion de radian vers degré.
	static constexpr long double RadianToDegree = 57.295779513082320876798154814105;
	//!Constante de conversion de degré vers radian.
	static constexpr long double DegreeToRadian = 1 / RadianToDegree;
	/**
	*\brief
	*	Classe d'angle exprimé en radians.
	*\remarks
	*	Permet de renforcer les unités des angles à la compilation.\n
	*	Se construit à partir de valeurs en radians, et retourne des valeurs en
	*	radians.
	*/
	template< typename T >
	class RadiansT
	{
	public:
		/**
		*\name Construction
		*/
		/**@{*/
		explicit inline RadiansT( float value )noexcept
			: m_value{ T( value ) }
		{
		}

		explicit inline RadiansT( double value )noexcept
			: m_value{ T( value ) }
		{
		}

		explicit inline RadiansT( long double value )noexcept
			: m_value{ T( value ) }
		{
		}
		/**@}*/
		/**
		*\name Conversion
		*/
		/**@{*/
		explicit inline operator T()const noexcept
		{
			return m_value;
		}
		/**@}*/
		/**
		*\name Opérateurs arithmétiques membres.
		*/
		/**@{*/
		inline RadiansT & operator-()noexcept
		{
			m_value = -m_value;
			return *this;
		}

		inline RadiansT & operator+=( RadiansT< T > const & rhs )noexcept
		{
			m_value += rhs.m_value;
			return *this;
		}

		inline RadiansT & operator-=( RadiansT< T > const & rhs )noexcept
		{
			m_value -= rhs.m_value;
			return *this;
		}

		inline RadiansT & operator*=( T rhs )noexcept
		{
			m_value *= rhs;
			return *this;
		}

		inline RadiansT & operator/=( T rhs )noexcept
		{
			m_value /= rhs;
			return *this;
		}
		/**@}*/

	private:
		T m_value;
	};
	/**
	*\name Opérateurs de comparaison.
	*/
	/**@{*/
	template< typename T >
	inline bool operator==( RadiansT< T > const & lhs, RadiansT< T > const & rhs )noexcept
	{
		return std::abs( T( lhs ) - T( rhs ) ) < std::numeric_limits< T >::epsilon();
	}

	template< typename T >
	inline bool operator!=( RadiansT< T > const & lhs, RadiansT< T > const & rhs )noexcept
	{
		return !( lhs == rhs );
	}

	template< typename T >
	inline bool operator>( RadiansT< T > const & lhs, RadiansT< T > const & rhs )noexcept
	{
		return T( lhs ) > T( rhs );
	}

	template< typename T >
	inline bool operator<( RadiansT< T > const & lhs, RadiansT< T > const & rhs )noexcept
	{
		return T( lhs ) < T( rhs );
	}

	template< typename T >
	inline bool operator>=( RadiansT< T > const & lhs, RadiansT< T > const & rhs )noexcept
	{
		return !( lhs < rhs );
	}

	template< typename T >
	inline bool operator<=( RadiansT< T > const & lhs, RadiansT< T > const & rhs )noexcept
	{
		return !( lhs < rhs );
	}
	/**@}*/
	/**
	*\name Opérateurs arithmétiques.
	*/
	/**@{*/
	template< typename T >
	inline RadiansT< T > operator+( RadiansT< T > const & lhs, RadiansT< T > const & rhs )noexcept
	{
		RadiansT< T > tmp{ lhs };
		return tmp += rhs;
	}

	template< typename T >
	inline RadiansT< T > operator-( RadiansT< T > const & lhs, RadiansT< T > const & rhs )noexcept
	{
		RadiansT< T > tmp{ lhs };
		return tmp -= rhs;
	}

	template< typename T >
	inline RadiansT< T > operator*( RadiansT< T > const & lhs, T rhs )noexcept
	{
		RadiansT< T > tmp{ lhs };
		return tmp *= rhs;
	}

	template< typename T >
	inline RadiansT< T > operator/( RadiansT< T > const & lhs, T rhs )noexcept
	{
		RadiansT< T > tmp{ lhs };
		return tmp /= rhs;
	}
	/**@}*/
	using Radians = RadiansT< float >;
}
/**
*\brief
*	Définition de litérale pour les angles en radians.
*\param[in] value
*	La valeur en radians.
*\return
*	L'angle.
*/
inline gl::Radians operator "" _radians( long double value )
{
	return gl::Radians{ float( value ) };
}
namespace gl
{
	/**
	*\brief
	*	Classe d'angle exprimé en degrés.
	*\remark
	*	Permet de renforcer les unités des angles à la compilation.\n
	*	Se construit à partir de valeurs en degrés, et retourne des valeurs en
	*	degrés.
	*/
	template< typename T >
	class DegreesT
	{
	public:
		/**
		*\name Construction
		*/
		/**@{*/
		explicit inline DegreesT( float value )noexcept
			: m_value{ T( value ) }
		{
		}

		explicit inline DegreesT( double value )noexcept
			: m_value{ T( value ) }
		{
		}

		explicit inline DegreesT( long double value )noexcept
			: m_value{ T( value ) }
		{
		}
		/**@}*/
		/**
		*\name Conversion
		*/
		/**@{*/
		explicit inline operator T()const noexcept
		{
			return m_value;
		}
		/**@}*/
		/**
		*\name Opérateurs arithmétiques membres.
		*/
		/**@{*/
		inline DegreesT & operator-()noexcept
		{
			m_value = -m_value;
			return *this;
		}

		inline DegreesT & operator+=( DegreesT< T > const & rhs )noexcept
		{
			m_value += rhs.m_value;
			return *this;
		}

		inline DegreesT & operator-=( DegreesT< T > const & rhs )noexcept
		{
			m_value -= rhs.m_value;
			return *this;
		}

		inline DegreesT & operator*=( T rhs )noexcept
		{
			m_value *= rhs;
			return *this;
		}

		inline DegreesT & operator/=( T rhs )noexcept
		{
			m_value /= rhs;
			return *this;
		}
		/**@}*/

	private:
		T m_value;
	};
	/**
	*\name Opérateurs de comparaison.
	*/
	/**@{*/
	template< typename T >
	inline bool operator==( DegreesT< T > const & lhs, DegreesT< T > const & rhs )noexcept
	{
		return std::abs( T( lhs ) - T( rhs ) ) < std::numeric_limits< T >::epsilon();
	}

	template< typename T >
	inline bool operator!=( DegreesT< T > const & lhs, DegreesT< T > const & rhs )noexcept
	{
		return !( lhs == rhs );
	}

	template< typename T >
	inline bool operator>( DegreesT< T > const & lhs, DegreesT< T > const & rhs )noexcept
	{
		return T( lhs ) > T( rhs );
	}

	template< typename T >
	inline bool operator<( DegreesT< T > const & lhs, DegreesT< T > const & rhs )noexcept
	{
		return T( lhs ) < T( rhs );
	}

	template< typename T >
	inline bool operator>=( DegreesT< T > const & lhs, DegreesT< T > const & rhs )noexcept
	{
		return !( lhs < rhs );
	}

	template< typename T >
	inline bool operator<=( DegreesT< T > const & lhs, DegreesT< T > const & rhs )noexcept
	{
		return !( lhs < rhs );
	}
	/**@}*/
	/**
	*\name Opérateurs arithmétiques.
	*/
	/**@{*/
	template< typename T >
	inline DegreesT< T > operator+( DegreesT< T > const & lhs, DegreesT< T > const & rhs )noexcept
	{
		DegreesT< T > tmp{ lhs };
		return tmp += rhs;
	}

	template< typename T >
	inline DegreesT< T > operator-( DegreesT< T > const & lhs, DegreesT< T > const & rhs )noexcept
	{
		DegreesT< T > tmp{ lhs };
		return tmp -= rhs;
	}

	template< typename T >
	inline DegreesT< T > operator*( DegreesT< T > const & lhs, T rhs )noexcept
	{
		DegreesT< T > tmp{ lhs };
		return tmp *= rhs;
	}

	template< typename T >
	inline DegreesT< T > operator/( DegreesT< T > const & lhs, T rhs )noexcept
	{
		DegreesT< T > tmp{ lhs };
		return tmp /= rhs;
	}
	/**@}*/
	using Degrees = DegreesT< float >;
}
/**
*\brief
*	Définition de litérale pour les angles en degrés.
*\param[in] value
*	La valeur en degrés.
*\return
*	L'angle.
*/
inline gl::Degrees operator "" _degrees( long double value )
{
	return gl::Degrees{ float( value ) };
}
namespace gl
{
	/**
	*\brief
	*	Calcule la valeur en radians de l'angle en degrés donné.
	*\param[in] value
	*	L'angle en degrés.
	*\return
	*	L'angle en radians.
	*/
	template< typename T >
	inline RadiansT< T > to_radians( DegreesT< T > const & value )noexcept
	{
		return RadiansT< T >{ T( value ) * DegreeToRadian };
	}
	/**
	*\brief
	*	Calcule la valeur en degrés de l'angle en radians donné.
	*\param[in] value
	*	L'angle en radians.
	*\return
	*	L'angle en degrés.
	*/
	template< typename T >
	inline DegreesT< T > to_degrees( RadiansT< T > const & value )noexcept
	{
		return DegreesT< T >{ T( value ) * RadianToDegree };
	}
	/**
	*\brief
	*	Classe d'angle.
	*\remark
	*	Utilisée pour gérer les angles sans avoir à se préoccuper des
	*	conversions degré / radian.
	*	Sa valeur sera toujours comprise entre 0 et Pi.
	*/
	template< typename T >
	class AngleT
	{
	public:
		/**
		*\name Construction
		*/
		/**@{*/
		/**
		*\brief
		*	Constructeur, à partir d'un angle en degrés.
		*\param[in] value
		*	L'angle, exprimé en degrés.
		*\return
		*	L'angle construit.
		*/
		explicit inline AngleT( DegreesT< T > const & value )noexcept
			: m_radians{ to_radians( value ) }
		{
			doNormalise();
		}
		/**
		*\brief
		*	Constructeur, à partir d'un angle en radians.
		*\param[in] value
		*	L'angle, exprimé en radians.
		*\return
		*	L'angle construit.
		*/
		inline AngleT( RadiansT< T > const & value )noexcept
			: m_radians{ value }
		{
			doNormalise();
		}
		/**
		*\brief
		*	Constructeur par défaut, l'angle vaut 0 radians.
		*/
		inline AngleT()noexcept
			: m_radians{ T{ 0.0 } }
		{
		}
		/**@}*/
		/**
		*\name Conversion
		*/
		/**@{*/
		/**
		*\return
		*	La valeur de l'angle, en degrés.
		*/
		explicit inline operator DegreesT< T >()const noexcept
		{
			return to_degrees( m_radians );
		}
		/**
		*\return
		*	La valeur de l'angle, en radians.
		*/
		inline operator RadiansT< T >()const noexcept
		{
			return m_radians;
		}
		/**
		*\return
		*	La valeur de l'angle, en radians.
		*/
		explicit inline operator T()const noexcept
		{
			return T( m_radians );
		}
		/**@}*/
		/**
		*\name Opérateurs arithmétiques membres.
		*/
		/**@{*/
		inline AngleT & operator+=( AngleT< T > const & rhs )noexcept
		{
			m_radians += rhs;
			return *this;
		}

		inline AngleT & operator-=( AngleT< T > const & rhs )noexcept
		{
			m_radians -= rhs;
			return *this;
		}

		inline AngleT & operator+=( RadiansT< T > const & rhs )noexcept
		{
			m_radians += rhs;
			return *this;
		}

		inline AngleT & operator-=( RadiansT< T > const & rhs )noexcept
		{
			m_radians -= rhs;
			return *this;
		}

		inline AngleT & operator*=( T rhs )noexcept
		{
			m_radians *= rhs;
			return *this;
		}

		inline AngleT & operator/=( T rhs )noexcept
		{
			m_radians /= rhs;
			return *this;
		}
		/**@}*/

	private:
		/**
		*\brief
		*	Remet les valeurs de l'angle entre 0 et PI * 2.
		*/
		inline void doNormalise()noexcept
		{
			static RadiansT< T > zero{ T { 0 } };
			m_radians = RadiansT< T >{ T( T( m_radians ) - PiMult2 * std::floor( T( m_radians ) / PiMult2 ) ) };

			if ( T( m_radians ) >= PiMult2 )
			{
				m_radians = zero;
			}
			else if ( m_radians < zero )
			{
				if ( PiMult2 + T( m_radians ) == PiMult2 )
				{
					m_radians = zero;
				}
				else
				{
					m_radians = RadiansT< T >{ T( PiMult2 + T( m_radians ) ) };
				}
			}
		}

	private:
		//! La valeur de l'angle.
		RadiansT< T > m_radians;
	};
	/**
	*\name Opérateurs de comparaison.
	*/
	/**@{*/
	template< typename T >
	inline bool operator==( AngleT< T > const & lhs, AngleT< T > const & rhs )noexcept
	{
		return RadiansT< T >( lhs ) == RadiansT< T >( rhs );
	}

	template< typename T >
	inline bool operator!=( AngleT< T > const & lhs, AngleT< T > const & rhs )noexcept
	{
		return !( lhs == rhs );
	}

	template< typename T >
	inline bool operator<( AngleT< T > const & lhs, AngleT< T > const & rhs )noexcept
	{
		return RadiansT< T >( lhs ) < RadiansT< T >( rhs );
	}

	template< typename T >
	inline bool operator>=( AngleT< T > const & lhs, AngleT< T > const & rhs )noexcept
	{
		return !( lhs < rhs );
	}

	template< typename T >
	inline bool operator>( AngleT< T > const & lhs, AngleT< T > const & rhs )noexcept
	{
		return RadiansT< T >( lhs ) > RadiansT< T >( rhs );
	}

	template< typename T >
	inline bool operator<=( AngleT< T > const & lhs, AngleT< T > const & rhs )noexcept
	{
		return !( lhs > rhs );
	}
	/**@}*/
	/**
	*\name Opérateurs arithmétiques.
	*/
	/**@{*/
	template< typename T >
	inline AngleT< T > operator+( AngleT< T > const & lhs, AngleT< T > const & rhs )noexcept
	{
		AngleT< T > tmp{ lhs };
		return tmp += rhs;
	}

	template< typename T >
	inline AngleT< T > operator-( AngleT< T > const & lhs, AngleT< T > const & rhs )noexcept
	{
		AngleT< T > tmp{ lhs };
		return tmp -= rhs;
	}

	template< typename T >
	inline AngleT< T > operator*( AngleT< T > const & lhs, float rhs )noexcept
	{
		AngleT< T > tmp{ lhs };
		return tmp *= rhs;
	}

	template< typename T >
	inline AngleT< T > operator/( AngleT< T > const & lhs, float rhs )noexcept
	{
		AngleT< T > tmp{ lhs };
		return tmp /= rhs;
	}
	/**@}*/
	/**
	*\name Surcharges des fonctions trigonométriques de base.
	*/
	/**@{*/
	template< typename T >
	inline T cos( RadiansT< T > const & angle )noexcept
	{
		return std::cos( T( angle ) );
	}

	template< typename T >
	inline T sin( RadiansT< T > const & angle )noexcept
	{
		return std::sin( T( angle ) );
	}

	template< typename T >
	inline T tan( RadiansT< T > const & angle )noexcept
	{
		return std::tan( T( angle ) );
	}

	template< typename T >
	inline T cosh( RadiansT< T > const & angle )noexcept
	{
		return std::cosh( T( angle ) );
	}

	template< typename T >
	inline T sinh( RadiansT< T > const & angle )noexcept
	{
		return std::sinh( T( angle ) );
	}

	template< typename T >
	inline T tanh( RadiansT< T > const & angle )noexcept
	{
		return std::tanh( T( angle ) );
	}

	template< typename T >
	inline RadiansT< T > acos( T value )noexcept
	{
		return RadiansT< T >{ std::acos( value ) };
	}

	template< typename T >
	inline RadiansT< T > asin( T value )noexcept
	{
		return RadiansT< T >{ std::asin( value ) };
	}

	template< typename T >
	inline RadiansT< T > atan( T value )noexcept
	{
		return RadiansT< T >{ std::atan( value) };
	}
	/**@}*/
	using Angle = AngleT< float >;
}

#endif
