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
#include "vertex_array_object_factory.h"
#include "factories/gfx_program_factory.h"

#include <gl/glew.h>

#ifndef OFFSET
#define OFFSET(x) ((char *)NULL + x)
#endif

namespace playstate
{
	const uint32 PositionAttribLocation = 0;
	const uint32 NormalAttribLocation = 1;
	const uint32 TexCoordAttribLocation = 2;
	const uint32 ColorAttribLocation = 3;

	struct PositionData
	{
		Vector3 Position; // always at location = 0
	};

	struct PositionTexCoordData
	{
		Vector3 Position; // always at location = 0
		Vector2 TexCoord; // always at location = 2
	};

	struct PositionNormalData
	{
		Vector3 Position; // always at location = 0
		Vector3 Normal; // always at location = 1
	};

	struct PositionNormalTextureData
	{
		Vector3 Position; // always at location = 0
		Vector3 Normal;  // always at location = 1
		Vector2 TexCoord; // always at location = 2
	};

	// TODO Add struct for color as well - color location = 3.

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

		//
		// Creates an index buffer based on a supplied of indices
		// @return A bindable and renderable index buffer
		IndexBuffer* CreateIndexBuffer(uint32* indices, uint32 numIndices);

		//
		//
		VertexBuffer* CreateStaticBufferObject(PositionData* elements, uint32 numElements);
		VertexBuffer* CreateStaticBufferObject(PositionTexCoordData* elements, uint32 numElements);
		VertexBuffer* CreateStaticBufferObject(PositionNormalData* elements, uint32 numElements);
		VertexBuffer* CreateStaticBufferObject(PositionNormalTextureData* elements, uint32 numElements);

		//
		// Creates a render target
		// @param width
		// @param height
		// @param format 
		RenderTarget2D* CreateRenderTarget2D(uint32 width, uint32 height, TextureFormat format);
		
	// IWindowResizedListener
	public:
		virtual void OnWindowResized(IWindow& window, uint32 width, uint32 height);

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

		IVertexArrayObjectFactory* mPositionVAOFactory;
		IVertexArrayObjectFactory* mPositionNormalVAOFactory;
		IVertexArrayObjectFactory* mPositionTextureVAOFactory;
		IVertexArrayObjectFactory* mPositionNormalTextureVAOFactory;
	};
}
