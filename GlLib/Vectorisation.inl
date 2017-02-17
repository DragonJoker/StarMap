namespace gl
{
	template< typename R, typename T >
	struct Caller< R, T, Vec2T< T > >
	{
		static Vec2T< T > call( R( *func )( T ), Vec2T< T > const & value )
		{
			return Vec2T< T >
			{
				func( value.x ),
					func( value.y )
			};
		}
	};

	template< typename R, typename T >
	struct CallerConstRef< R, T, Vec2T< T > >
	{
		static Vec2T< T > call( R( *func )( T const & ), Vec2T< T > const & value )
		{
			return Vec2T< T >
			{
				func( value.x ),
					func( value.y )
			};
		}
	};

	template< typename R, typename T >
	struct Caller< R, T, Vec3T< T > >
	{
		static Vec3T< T > call( R( *func )( T ), Vec3T< T > const & value )
		{
			return Vec3T< T >
			{
				func( value.x ),
					func( value.y ),
					func( value.z )
			};
		}
	};

	template< typename R, typename T >
	struct CallerConstRef< R, T, Vec3T< T > >
	{
		static Vec3T< T > call( R( *func )( T const & ), Vec3T< T > const & value )
		{
			return Vec3T< T >
			{
				func( value.x ),
					func( value.y ),
					func( value.z )
			};
		}
	};

	template< typename R, typename T >
	struct Caller< R, T, Vec4T< T > >
	{
		static Vec4T< T > call( R( *func )( T ), Vec4T< T > const & value )
		{
			return Vec4T< T >
			{
				func( value.x ),
					func( value.y ),
					func( value.z ),
					func( value.w )
			};
		}
	};

	template< typename R, typename T >
	struct CallerConstRef< R, T, Vec4T< T > >
	{
		static Vec4T< T > call( R( *func )( T const & ), Vec4T< T > const & value )
		{
			return Vec4T< T >
			{
				func( value.x ),
					func( value.y ),
					func( value.z ),
					func( value.w )
			};
		}
	};

	template< typename R, typename T, typename V >
	inline V call( R( *func )( T ), V const & value )
	{
		return Caller< R, T, V >::call( func, value );
	}

	template< typename R, typename T, typename V >
	inline V call( R( *func )( T const & ), V const & value )
	{
		return CallerConstRef< R, T, V >::call( func, value );
	}
}
