#include"GlLibPrerequisites.h"

#include <iomanip>
#include <algorithm>

#if !defined( DESKTOP )
#	include <GLES3/gl3.h>
#endif

#if defined( __ANDROID__ )
#	include <EGL/egl.h>
#elif defined( DESKTOP )
#	ifndef GLEW_STATIC
#		define GLEW_STATIC
#	endif
#	include "GL/glew.h"
#endif
