//
// pch.h
// En-tête pour fichiers Include système standard.
//
// Utilisé par le système de génération pour générer l'en-tête précompilé. Notez qu'aucun(e)
// pch.cpp n'est nécessaire et que pch.h est automatiquement inclus dans tous les fichiers cpp
// qui font partie du projet.
//

#include <jni.h>
#include <errno.h>

#include <string.h>
#include <unistd.h>
#include <sys/resource.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/log.h>

#include "android_native_app_glue.h"

#include <RenderLib/Scene.h>
#include <RenderLib/Material.h>
#include <RenderLib/Mesh.h>
#include <RenderLib/Object.h>
#include <RenderLib/StringUtils.h>
#include <RenderLib/Submesh.h>
#include <RenderLib/Texture.h>

#include <GlLib/GlAttribute.h>
#include <GlLib/GlBuffer.h>
#include <GlLib/GlUniform.h>
