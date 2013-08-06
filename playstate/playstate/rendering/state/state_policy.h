#pragma once

#include "../../types.h"
#include "../../math/color.h"

#include <gl/glew.h>

#ifdef _DEBUG
#ifndef CHECK_GL_ERROR
#define CHECK_GL_ERROR() \
	{ \
		GLenum err = glGetError(); \
		assert(err == GL_NO_ERROR); \
	}
#endif
#else
#ifndef CHECK_GL_ERROR
#define CHECK_GL_ERROR()
#endif
#endif

namespace playstate
{
	class SrcBlend
	{
	public:
		enum Enum {
			SRC_ALPHA = GL_SRC_ALPHA,
			ONE = GL_ONE,
			ZERO = GL_ZERO,
		};

		static const Enum Default = SRC_ALPHA;
	};

	class DestBlend
	{
	public:
		enum Enum {
			ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
			ONE = GL_ONE
		};

		static const Enum Default = ONE_MINUS_DST_ALPHA;
	};

	class CullFaces
	{
	public:
		enum Enum {
			DISABLED = 0,
			CW = GL_CW,
			CCW = GL_CCW
		};

		static const Enum Default = CCW;
	};
	
	class DepthFunc
	{
	public:
		enum Enum {
			NEVER = GL_NEVER,
			LESS = GL_LESS,
			EQUAL = GL_EQUAL,
			LEQUAL = GL_LEQUAL,
			GREATER = GL_GREATER,
			NOTEQUAL = GL_NOTEQUAL,
			GEQUAL = GL_GEQUAL,
			ALWAYS = GL_ALWAYS
		};

		static const Enum Default = LEQUAL;
	};

	class ClearTypes
	{
	public:
		enum Enum {
			COLOR = BIT(1),
			DEPTH = BIT(2)
		};
	};

	class MinFilter
	{
	public:
		enum Enum {
			NEAREST = GL_NEAREST,
			LINEAR = GL_LINEAR,
			NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
			LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
			NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
			LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR,
			UNKNOWN = 0
		};

		static const Enum Default = LINEAR;
	};

	class MagFilter
	{
	public:
		enum Enum {
			NEAREST = GL_NEAREST,
			LINEAR = GL_LINEAR,
			UNKNOWN = 0
		};

		static const Enum Default = LINEAR;
	};

	class TextureWrap
	{
	public:
		enum Enum {
			CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
			CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
			MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
			REPEAT = GL_REPEAT,
			UNKNOWN = 0
		};

		static const Enum Default = REPEAT;
	};

	static const uint32 MaxActiveTextures = 16; // 

	//
	// State holder for the OpenGL driver
	// TODO: Add ThreadLocal since the states are context-dependent (thread-dependent)
	class StatePolicy
	{
	public:
		static void UseProgram(GLuint programID);

		static void EnableDepthTest(bool enable);
		static void SetDepthFunc(DepthFunc::Enum depthFunc);

		static void EnableBlend(bool enable);
		static void SetBlendFunc(SrcBlend::Enum srcFunc, DestBlend::Enum destFunc);

		static void SetCullFaces(CullFaces::Enum cullFaces);

		static void SetClearColor(const Color& color);
		static void SetClearDepth(float32 depth);

		static void SetActiveTexture(uint32 id);
		static void BindTexture(GLenum type, GLuint texture);

		static void Viewport(GLint x, GLint y, GLsizei width, GLsizei height);
		
		//
		// Mark all states as dirty.
		static void MarkAsDirty();
	};
}

