#include "pch.h"
#include "GlLibPrerequisites.h"

#include <string>

namespace gl
{
	uint32_t pixelSize( PixelFormat format )noexcept
	{
		switch ( format )
		{
		case gl::PixelFormat::eL8:
		case gl::PixelFormat::eS8:
			return 1;
			break;

		case gl::PixelFormat::eL8A8:
		case gl::PixelFormat::eRGB565:
		case gl::PixelFormat::eRGBA5551:
		case gl::PixelFormat::eRGBA4444:
		case gl::PixelFormat::eD16:
			return 2;
			break;

		case gl::PixelFormat::eR8G8B8:
			return 3;
			break;

		case gl::PixelFormat::eR8G8B8A8:
			return 4;
			break;

		default:
			assert( false && "Unsupported pixel format" );
			return 0;
		}
	}

	GLenum glFormat( PixelFormat format )noexcept
	{
		switch ( format )
		{
		case PixelFormat::eL8:
			return GL_LUMINANCE;

		case PixelFormat::eL8A8:
			return GL_LUMINANCE_ALPHA;

		case PixelFormat::eR8G8B8:
		case PixelFormat::eRGB565:
			return GL_RGB;

		case PixelFormat::eR8G8B8A8:
		case PixelFormat::eRGBA5551:
		case PixelFormat::eRGBA4444:
			return GL_RGBA;

		default:
			assert( false && "Unsupported pixel format for texture" );
			return 0;
		}
	}

	GLenum glType( PixelFormat format )noexcept
	{
		switch ( format )
		{
		case PixelFormat::eL8:
		case PixelFormat::eL8A8:
		case PixelFormat::eR8G8B8:
		case PixelFormat::eR8G8B8A8:
			return GL_UNSIGNED_BYTE;

		case PixelFormat::eRGB565:
			return GL_UNSIGNED_SHORT_5_6_5;

		case PixelFormat::eRGBA5551:
			return GL_UNSIGNED_SHORT_5_5_5_1;

		case PixelFormat::eRGBA4444:
			return GL_UNSIGNED_SHORT_4_4_4_4;

		default:
			assert( false && "Unsupported pixel format for texture" );
			return 0;
		}
	}
}
