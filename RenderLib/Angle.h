/**
*\file
*	Angle.h
*\author
*	Sylvain Doremus
*/
#ifndef ___DesktopUtils_Angle_HPP___
#define ___DesktopUtils_Angle_HPP___
#pragma once

#include "RenderLibPrerequisites.h"

#include <cmath>
#include <limits>

namespace render
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
	class Radians
	{
	public:
		/**
		*\name Construction
		*/
		/**@{*/
		explicit inline Radians( float value )noexcept
			: m_value{ value }
		{
		}

		explicit inline Radians( double value )noexcept
			: Radians{ float( value ) }
		{
		}

		explicit inline Radians( long double value )noexcept
			: Radians{ float( value ) }
		{
		}
		/**@}*/
		/**
		*\name Conversion
		*/
		/**@{*/
		explicit inline operator float()const noexcept
		{
			return m_value;
		}
		/**@}*/
		/**
		*\name Opérateurs arithmétiques membres.
		*/
		/**@{*/
		inline Radians & operator-()noexcept
		{
			m_value = -m_value;
			return *this;
		}

		inline Radians & operator+=( Radians const & rhs )noexcept
		{
			m_value += rhs.m_value;
			return *this;
		}

		inline Radians & operator-=( Radians const & rhs )noexcept
		{
			m_value -= rhs.m_value;
			return *this;
		}

		inline Radians & operator*=( float rhs )noexcept
		{
			m_value *= rhs;
			return *this;
		}

		inline Radians & operator/=( float rhs )noexcept
		{
			m_value /= rhs;
			return *this;
		}
		/**@}*/

	private:
		float m_value;
	};
	/**
	*\name Opérateurs de comparaison.
	*/
	/**@{*/
	inline bool operator==( Radians const & lhs, Radians const & rhs )noexcept
	{
		return std::abs( float( lhs ) - float( rhs ) ) < std::numeric_limits< float >::epsilon();
	}

	inline bool operator!=( Radians const & lhs, Radians const & rhs )noexcept
	{
		return !( lhs == rhs );
	}

	inline bool operator>( Radians const & lhs, Radians const & rhs )noexcept
	{
		return float( lhs ) > float( rhs );
	}

	inline bool operator<( Radians const & lhs, Radians const & rhs )noexcept
	{
		return float( lhs ) < float( rhs );
	}

	inline bool operator>=( Radians const & lhs, Radians const & rhs )noexcept
	{
		return !( lhs < rhs );
	}

	inline bool operator<=( Radians const & lhs, Radians const & rhs )noexcept
	{
		return !( lhs < rhs );
	}
	/**@}*/
	/**
	*\name Opérateurs arithmétiques.
	*/
	/**@{*/
	inline Radians operator+( Radians const & lhs, Radians const & rhs )noexcept
	{
		Radians tmp{ lhs };
		return tmp += rhs;
	}

	inline Radians operator-( Radians const & lhs, Radians const & rhs )noexcept
	{
		Radians tmp{ lhs };
		return tmp -= rhs;
	}

	inline Radians operator*( Radians const & lhs, float rhs )noexcept
	{
		Radians tmp{ lhs };
		return tmp *= rhs;
	}

	inline Radians operator/( Radians const & lhs, float rhs )noexcept
	{
		Radians tmp{ lhs };
		return tmp /= rhs;
	}
	/**@}*/
}
/**
*\brief
*	Définition de litérale pour les angles en radians.
*\param[in] value
*	La valeur en radians.
*\return
*	L'angle.
*/
inline render::Radians operator "" _radians( long double value )
{
	return render::Radians{ float( value ) };
}
namespace render
{
	/**
	*\brief
	*	Classe d'angle exprimé en degrés.
	*\remark
	*	Permet de renforcer les unités des angles à la compilation.\n
	*	Se construit à partir de valeurs en degrés, et retourne des valeurs en
	*	degrés.
	*/
	class Degrees
	{
	public:
		/**
		*\name Construction
		*/
		/**@{*/
		explicit inline Degrees( float value )noexcept
			: m_value{ value }
		{
		}

		explicit inline Degrees( double value )noexcept
			: Degrees{ float( value ) }
		{
		}

		explicit inline Degrees( long double value )noexcept
			: Degrees{ float( value ) }
		{
		}
		/**@}*/
		/**
		*\name Conversion
		*/
		/**@{*/
		explicit inline operator float()const noexcept
		{
			return m_value;
		}
		/**@}*/
		/**
		*\name Opérateurs arithmétiques membres.
		*/
		/**@{*/
		inline Degrees & operator-()noexcept
		{
			m_value = -m_value;
			return *this;
		}

		inline Degrees & operator+=( Degrees const & rhs )noexcept
		{
			m_value += rhs.m_value;
			return *this;
		}

		inline Degrees & operator-=( Degrees const & rhs )noexcept
		{
			m_value -= rhs.m_value;
			return *this;
		}

		inline Degrees & operator*=( float rhs )noexcept
		{
			m_value *= rhs;
			return *this;
		}

		inline Degrees & operator/=( float rhs )noexcept
		{
			m_value /= rhs;
			return *this;
		}
		/**@}*/

	private:
		float m_value;
	};
	/**
	*\name Opérateurs de comparaison.
	*/
	/**@{*/
	inline bool operator==( Degrees const & lhs, Degrees const & rhs )noexcept
	{
		return std::abs( float( lhs ) - float( rhs ) ) < std::numeric_limits< float >::epsilon();
	}

	inline bool operator!=( Degrees const & lhs, Degrees const & rhs )noexcept
	{
		return !( lhs == rhs );
	}

	inline bool operator>( Degrees const & lhs, Degrees const & rhs )noexcept
	{
		return float( lhs ) > float( rhs );
	}

	inline bool operator<( Degrees const & lhs, Degrees const & rhs )noexcept
	{
		return float( lhs ) < float( rhs );
	}

	inline bool operator>=( Degrees const & lhs, Degrees const & rhs )noexcept
	{
		return !( lhs < rhs );
	}

	inline bool operator<=( Degrees const & lhs, Degrees const & rhs )noexcept
	{
		return !( lhs < rhs );
	}
	/**@}*/
	/**
	*\name Opérateurs arithmétiques.
	*/
	/**@{*/
	inline Degrees operator+( Degrees const & lhs, Degrees const & rhs )noexcept
	{
		Degrees tmp{ lhs };
		return tmp += rhs;
	}

	inline Degrees operator-( Degrees const & lhs, Degrees const & rhs )noexcept
	{
		Degrees tmp{ lhs };
		return tmp -= rhs;
	}

	inline Degrees operator*( Degrees const & lhs, float rhs )noexcept
	{
		Degrees tmp{ lhs };
		return tmp *= rhs;
	}

	inline Degrees operator/( Degrees const & lhs, float rhs )noexcept
	{
		Degrees tmp{ lhs };
		return tmp /= rhs;
	}
	/**@}*/
}
/**
*\brief
*	Définition de litérale pour les angles en degrés.
*\param[in] value
*	La valeur en degrés.
*\return
*	L'angle.
*/
inline render::Degrees operator "" _degrees( long double value )
{
	return render::Degrees{ float( value ) };
}
namespace render
{
	/**
	*\brief
	*	Calcule la valeur en radians de l'angle en degrés donné.
	*\param[in] value
	*	L'angle en degrés.
	*\return
	*	L'angle en radians.
	*/
	inline Radians to_radians( Degrees const & value )noexcept
	{
		return Radians{ float( value ) * DegreeToRadian };
	}
	/**
	*\brief
	*	Calcule la valeur en degrés de l'angle en radians donné.
	*\param[in] value
	*	L'angle en radians.
	*\return
	*	L'angle en degrés.
	*/
	inline Degrees to_degrees( Radians const & value )noexcept
	{
		return Degrees{ float( value ) * RadianToDegree };
	}
	/**
	*\brief
	*	Classe d'angle.
	*\remark
	*	Utilisée pour gérer les angles sans avoir à se préoccuper des
	*	conversions degré / radian.
	*	Sa valeur sera toujours comprise entre 0 et Pi.
	*/
	class Angle
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
		explicit inline Angle( Degrees const & value )noexcept
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
		inline Angle( Radians const & value )noexcept
			: m_radians{ value }
		{
			doNormalise();
		}
		/**
		*\brief
		*	Constructeur par défaut, l'angle vaut 0 radians.
		*/
		inline Angle()noexcept
			: m_radians{ 0.0_radians }
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
		explicit inline operator Degrees()const noexcept
		{
			return to_degrees( m_radians );
		}
		/**
		*\return
		*	La valeur de l'angle, en radians.
		*/
		inline operator Radians()const noexcept
		{
			return m_radians;
		}
		/**
		*\return
		*	La valeur de l'angle, en radians.
		*/
		explicit inline operator float()const noexcept
		{
			return float( m_radians );
		}
		/**@}*/
		/**
		*\name Opérateurs arithmétiques membres.
		*/
		/**@{*/
		inline Angle & operator+=( Angle const & rhs )noexcept
		{
			m_radians += rhs;
			return *this;
		}

		inline Angle & operator-=( Angle const & rhs )noexcept
		{
			m_radians -= rhs;
			return *this;
		}

		inline Angle & operator+=( Radians const & rhs )noexcept
		{
			m_radians += rhs;
			return *this;
		}

		inline Angle & operator-=( Radians const & rhs )noexcept
		{
			m_radians -= rhs;
			return *this;
		}

		inline Angle & operator*=( float rhs )noexcept
		{
			m_radians *= rhs;
			return *this;
		}

		inline Angle & operator/=( float rhs )noexcept
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
			m_radians = Radians{ float( float( m_radians ) - PiMult2 * std::floor( float( m_radians ) / PiMult2 ) ) };

			if ( float( m_radians ) >= PiMult2 )
			{
				m_radians = 0.0_radians;
			}
			else if ( m_radians < 0.0_radians )
			{
				if ( PiMult2 + float( m_radians ) == PiMult2 )
				{
					m_radians = 0.0_radians;
				}
				else
				{
					m_radians = Radians{ float( PiMult2 + float( m_radians ) ) };
				}
			}
		}

	private:
		//! La valeur de l'angle.
		Radians m_radians;
	};
	/**
	*\name Opérateurs de comparaison.
	*/
	/**@{*/
	inline bool operator==( Angle const & lhs, Angle const & rhs )noexcept
	{
		return Radians( lhs ) == Radians( rhs );
	}

	inline bool operator!=( Angle const & lhs, Angle const & rhs )noexcept
	{
		return !( lhs == rhs );
	}

	inline bool operator<( Angle const & lhs, Angle const & rhs )noexcept
	{
		return Radians( lhs ) < Radians( rhs );
	}

	inline bool operator>=( Angle const & lhs, Angle const & rhs )noexcept
	{
		return !( lhs < rhs );
	}

	inline bool operator>( Angle const & lhs, Angle const & rhs )noexcept
	{
		return Radians( lhs ) > Radians( rhs );
	}

	inline bool operator<=( Angle const & lhs, Angle const & rhs )noexcept
	{
		return !( lhs > rhs );
	}
	/**@}*/
	/**
	*\name Opérateurs arithmétiques.
	*/
	/**@{*/
	inline Angle operator+( Angle const & lhs, Angle const & rhs )noexcept
	{
		Angle tmp{ lhs };
		return tmp += rhs;
	}

	inline Angle operator-( Angle const & lhs, Angle const & rhs )noexcept
	{
		Angle tmp{ lhs };
		return tmp -= rhs;
	}

	inline render::Angle operator*( render::Angle const & lhs, float rhs )noexcept
	{
		Angle tmp{ lhs };
		return tmp *= rhs;
	}

	inline render::Angle operator/( render::Angle const & lhs, float rhs )noexcept
	{
		Angle tmp{ lhs };
		return tmp /= rhs;
	}
	/**@}*/
	/**
	*\name Surcharges des fonctions trigonométriques de base.
	*/
	/**@{*/
	inline float cos( Radians const & angle )noexcept
	{
		return std::cos( float( angle ) );
	}

	inline float sin( Radians const & angle )noexcept
	{
		return std::sin( float( angle ) );
	}

	inline float tan( Radians const & angle )noexcept
	{
		return std::tan( float( angle ) );
	}

	inline float cosh( Radians const & angle )noexcept
	{
		return std::cosh( float( angle ) );
	}

	inline float sinh( Radians const & angle )noexcept
	{
		return std::sinh( float( angle ) );
	}

	inline float tanh( Radians const & angle )noexcept
	{
		return std::tanh( float( angle ) );
	}

	inline Radians acos( float value )noexcept
	{
		return Radians{ std::acos( value ) };
	}

	inline Radians asin( float value )noexcept
	{
		return Radians{ std::asin( value ) };
	}

	inline Radians atan( float value )noexcept
	{
		return Radians{ std::atan( value) };
	}
	/**@}*/
}

#endif
