/**
*\file
*	Vectorisation.h
*\author
*	Sylvain Doremus
*/
#ifndef ___GlLib_Vectorisation_HPP___
#define ___GlLib_Vectorisation_HPP___
#pragma once

#include "Vec4.h"

namespace gl
{
	template< typename R, typename T, typename V >
	struct Caller;
	template< typename R, typename T, typename V >
	struct CallerConstRef;
	template< typename R, typename T, typename V >
	inline V call( R( *func )( T ), V const & value );
	template< typename R, typename T, typename V >
	inline V call( R( *func )( T const & ), V const & value );
}

#include "Vectorisation.inl"

#endif
