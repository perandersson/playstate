#pragma once
#include "../singleton.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "render_target_2d.h"
#include "gfx_program.h"
#include "render_context.h"
#include "../math/vector3.h"
#include "../math/vector2.h"
#include "../math/color.h"
#include "exception/rendering_exception.h"
#include "../version.h"
#include "../window/window_resized_listener.h"
#include "window_render_context.h"
#include "../window/window.h"
#include "factories/gfx_program_factory.h"

#include <gl/glew.h>

#ifndef OFFSET
#define OFFSET(x) ((char *)NULL + x)
#endif

namespace playstate
{

	//
	// 
	class RenderSystem : public Singleton<RenderSystem>, public IWindowResizedListener
	{
		friend class GfxProgram;

	public:
		RenderSystem(IWindow& window, ScriptSystem& scriptSystem);
		~RenderSystem();

		//
		// Loads a gfx program and returns the result. 
		// @param fileName
		// @return
		GfxProgram* LoadGfxProgram(const std::string& fileName);

	// IWindowResizedListener
	public:
		virtual void OnWindowResized(uint32 width, uint32 height);

	public:
		// Read-only property where the uniform [-1, 1] vertex buffer is located
		VertexBuffer*& const UniformVertexBuffer;

		// Read-only property of the shader version on the current computer.
		const Version& ShaderVersion;

	private:
		//
		// Sets the supplied render target to the given attachment index number.
		// This is used by GLSL to know where to render the result.
		// @param renderTarget
		// @param attachmentIndex
		void SetRenderTarget(RenderTarget2D* renderTarget, GLenum attachmentIndex);

		//
		// Sets the depth render target
		// @param renderTarget
		void SetDepthRenderTarget(RenderTarget2D* renderTarget);

		//
		// Applies the render targets into the OpenGL driver
		void ApplyRenderTargets();

		//
		// @return TRUE if the current computer maches the minimal requirements for this engine to work. (3.1)
		bool IsValidVersion() const;

	private:
		IWindow& mWindow;

		VertexBuffer* mUniformVertexBuffer;

		Version mShaderVersion;

		uint32 mScreenWidth;
		uint32 mScreenHeight;

		GfxProgramFactory* mProgramFactory;

		GLuint mFrameBufferId;
		RenderTarget2D* mRenderTargets[MaxDrawBuffers];
		RenderTarget2D* mDepthRenderTarget;
	};
}
