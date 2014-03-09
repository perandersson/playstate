#pragma once

#include <gl/glew.h>
#include "ogl3_gfx_program.h"

namespace playstate
{
	static const GLenum OGL3SrcFactor[SrcFactor::SIZE] = {
		GL_ZERO,
		GL_ONE,
		GL_SRC_COLOR,
		GL_ONE_MINUS_SRC_COLOR,
		GL_SRC_ALPHA,
		GL_ONE_MINUS_SRC_ALPHA,
		GL_DST_ALPHA,
		GL_ONE_MINUS_DST_ALPHA,
		GL_DST_COLOR,
		GL_ONE_MINUS_DST_COLOR,
		GL_CONSTANT_COLOR,
		GL_ONE_MINUS_CONSTANT_COLOR,
		GL_CONSTANT_ALPHA,
		GL_ONE_MINUS_CONSTANT_ALPHA
	};
	
	static const GLenum OGL3DestFactor[DestFactor::SIZE] = {
		GL_ZERO,
		GL_ONE,
		GL_SRC_COLOR,
		GL_ONE_MINUS_SRC_COLOR,
		GL_SRC_ALPHA,
		GL_ONE_MINUS_SRC_ALPHA,
		GL_DST_ALPHA,
		GL_ONE_MINUS_DST_ALPHA,
		GL_DST_COLOR,
		GL_ONE_MINUS_DST_COLOR,
		GL_CONSTANT_COLOR,
		GL_ONE_MINUS_CONSTANT_COLOR,
		GL_CONSTANT_ALPHA,
		GL_ONE_MINUS_CONSTANT_ALPHA
	};

	static const GLenum OGL3CullFaces[CullFaces::SIZE] = {
		0,
		GL_CW,
		GL_CCW
	};

	static const GLenum OGL3DepthFunc[DepthFunc::SIZE] = {
		GL_NEVER,
		GL_LESS,
		GL_EQUAL,
		GL_LEQUAL,
		GL_GREATER,
		GL_NOTEQUAL,
		GL_GEQUAL,
		GL_ALWAYS
	};

	class OGL3VertexBuffer;
	class OGL3IndexBuffer;

	class OGL3StatePolicy
	{
	public:
		static void BindIndexBuffer(OGL3IndexBuffer* indexBuffer);
		static void BindVertexBuffer(OGL3VertexBuffer* vertexBuffer);

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
		
		static void Viewport(const Rect& viewport);	
	};

	class OGL3StatePolicyGuard
	{
	public:
		static void BindIndexBuffer(OGL3IndexBuffer* indexBuffer);
		static void BindVertexBuffer(OGL3VertexBuffer* vertexBuffer);

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

		//
		// Mark all states as dirty.
		static void MarkAsDirty();
	};
}
