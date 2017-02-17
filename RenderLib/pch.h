#include <ctime>
#include <functional>
#include <iomanip>
#include <sstream>

#include <GlLib/GlArrayUniform.h>
#include <GlLib/GlAttribute.h>
#include <GlLib/GlBuffer.h>
#include <GlLib/GlDebug.h>
#include <GlLib/GlFrameBuffer.h>
#include <GlLib/GlRenderBuffer.h>
#include <GlLib/GlSampler.h>
#include <GlLib/GlShaderProgram.h>
#include <GlLib/GlTexture.h>
#include <GlLib/GlUniform.h>
#include <GlLib/OpenGL.h>

#include <GlLib/glm/gtc/matrix_transform.hpp>
#include <GlLib/glm/gtx/quaternion.hpp>

#include"RenderLibPrerequisites.h"

#if defined( PLATFORM_ANDROID )
#	include <EGL/egl.h>
#	include <GLES3/gl3.h>
#elif defined( PLATFORM_IPHONE ) || defined( PLATFORM_IPHONE_SIMULATOR )
#	include <OpenGLES/gltypes.h>
#	include <OpenGLES/EAGL.h>
#	include <OpenGLES/EAGLDrawable.h>
#	include <OpenGLES/ES3/gl.h>
#	include <OpenGLES/ES3/glext.h>
#elif defined( PLATFORM_WINDOWS )
#	ifndef GLEW_STATIC
#		define GLEW_STATIC
#	endif
#	include <GlLib/GL/glew.h>
#endif
