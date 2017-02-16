#include <ctime>
#include <functional>
#include <iomanip>
#include <sstream>

#if !defined( DESKTOP )
#	include <GLES3/gl3.h>
#else
#	define GLEW_STATIC
#	include <GlLib/GL/glew.h>
#endif

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
