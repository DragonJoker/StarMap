#include <jni.h>
#include <errno.h>

#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <sys/resource.h>

#include <android/asset_manager.h>
#include <android/configuration.h>
#include <android/log.h>
#include <android/native_activity.h>
#include <android/native_window_jni.h>
#include <android/sensor.h>

#if defined( __GNUG__ )
#	define COMPILER_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#	if COMPILER_VERSION > 40900
#		pragma GCC diagnostic ignored "-Wdeprecated-register"
#	endif
#endif
#include <ft2build.h>

#include <GlLib/Converter.h>

#include <RenderLib/ElementsList.h>
#include <RenderLib/FontLoader.h>
#include <RenderLib/Glyph.h>
#include <RenderLib/Material.h>
#include <RenderLib/Mesh.h>
#include <RenderLib/Object.h>
#include <RenderLib/StringUtils.h>
#include <RenderLib/Texture.h>

#include "AndroidUtilsPrerequisites.h"

#include <EGL/egl.h>
#include <GLES/gl.h>
