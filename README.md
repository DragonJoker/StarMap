AndroidOpenGLTest
=================

Tiny 3D engine written in C++ 14, using OpenGL ES 2/3.

Building AndroidOpenGLTest
--------------------------

This project is built with Visual Studio 2015 Update 3.

It contains the following projects:
- GlLib: a simple wrapper of OpenGL functions and objects.
- RenderLib: scene and objects management library.
- FreeType: integration of freetype, to build it on Android.
- AndroidUtils: an intermediate library that holds Android specific functions and objects.
- RenderLibTestApp: The test application, to validate RenderLib and GlLib.

Features
--------

- Objects: They can be manually defined, or imported using a small OBJ loader I've written (in AndroidUtils).
- Billboards: implemented without instancing, to keep them compatible with OpenGL ES2.
- PolyLines: Implemented using quads, to make them look nicer, and platform independant.
- Texturing: Diffuse and opacity textures.
- Text overlays: They use a common font texture which is loaded using FreeType.
- Scene management: add objects (billboards, objects or lines) to the Scene, they will be drawn respectively to their material.
