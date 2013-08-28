#pragma once

#include "../../types.h"
#include "../../math/color.h"
#include "../../math/rect.h"
#include "../index_buffer.h"
#include "../vertex_buffer.h"

#include <gl/glew.h>

namespace playstate
{
	//
	// Source factor used by blending
	// @see http://www.opengl.org/sdk/docs/man3/xhtml/glBlendFunc.xml
	class SrcFactor
	{
	public:
		enum Enum {
			ZERO = GL_ZERO,
			ONE = GL_ONE,
			SRC_COLOR = GL_SRC_COLOR,
			ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
			SRC_ALPHA = GL_SRC_ALPHA,
			ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
			DST_ALPHA = GL_DST_ALPHA,
			ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
			DST_COLOR = GL_DST_COLOR,
			ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
			CONSTANT_COLOR = GL_CONSTANT_COLOR,
			ONE_MINUS_CONSTANT_COLOR = GL_ONE_MINUS_CONSTANT_COLOR,
			CONSTANT_ALPHA = GL_CONSTANT_ALPHA,
			ONE_MINUS_CONSTANT_ALPHA = GL_ONE_MINUS_CONSTANT_ALPHA
		};

		static const Enum Default = ONE;
	};

	//
	// Destination factor used by blending
	// @see http://www.opengl.org/sdk/docs/man3/xhtml/glBlendFunc.xml
	class DestFactor
	{
	public:
		enum Enum {
			ZERO = GL_ZERO,
			ONE = GL_ONE,
			SRC_COLOR = GL_SRC_COLOR,
			ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
			SRC_ALPHA = GL_SRC_ALPHA,
			ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
			DST_ALPHA = GL_DST_ALPHA,
			ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
			DST_COLOR = GL_DST_COLOR,
			ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
			CONSTANT_COLOR = GL_CONSTANT_COLOR,
			ONE_MINUS_CONSTANT_COLOR = GL_ONE_MINUS_CONSTANT_COLOR,
			CONSTANT_ALPHA = GL_CONSTANT_ALPHA,
			ONE_MINUS_CONSTANT_ALPHA = GL_ONE_MINUS_CONSTANT_ALPHA
		};

		static const Enum Default = ZERO;
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
		static void SetBlendFunc(SrcFactor::Enum sfactor, DestFactor::Enum dfactor);

		static void SetCullFaces(CullFaces::Enum cullFaces);

		static void SetClearColor(const Color& color);
		static void SetClearDepth(float32 depth);
		
		static void EnableScissorTest(bool enable);
		static void SetScissorRect(const Rect& rect);

		static void SetActiveTexture(uint32 id);
		static void BindTexture(GLenum type, GLuint texture);
		static void InvalidateTexture(GLuint texture);
		
		static void Viewport(const Rect& viewport);
	
		static void BindVertexBuffer(VertexBuffer* vertexBuffer);
		static void BindIndexBuffer(IndexBuffer* indexBuffer);
	
		//
		// Mark all states as dirty.
		static void MarkAsDirty();
	};
}

