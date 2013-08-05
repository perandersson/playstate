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
#include "factories/vertex_array_object_factories.h"

#include <gl/glew.h>

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

		//
		// @return The shader-version on the current computer.
		const Version& GetShaderVersion() const;

		//
		// @return A vertex buffer representing a uniform [-1, 1] buffer.
		VertexBuffer* GetUniformVertexBuffer() const;

		//
		// Creates an index buffer based on a supplied of indices
		// @return A bindable and renderable index buffer
		// @throws RenderingException
		IndexBuffer* CreateStaticBuffer(const uint32* indices, uint32 numIndices);

		
		VertexBuffer* CreateStaticBuffer(const PositionData* elements, uint32 numElements);
		VertexBuffer* CreateStaticBuffer(const PositionTexCoordData* elements, uint32 numElements);
		VertexBuffer* CreateStaticBuffer(const PositionTexCoordColorData* elements, uint32 numElements);
		VertexBuffer* CreateStaticBuffer(const PositionNormalData* elements, uint32 numElements);
		VertexBuffer* CreateStaticBuffer(const PositionColorData* elements, uint32 numElements);
		VertexBuffer* CreateStaticBuffer(const PositionNormalTextureData* elements, uint32 numElements);

		//
		// Creates a static (non-updatable) buffer based on a supplied data stream
		//
		// @param data
		// @param dataTypeSize The size of one data element. {@code sizeof(PositionData)} is one such size
		// @param arrayFactory
		// @param numElements
		VertexBuffer* CreateStaticBuffer(const void* data, uint32 dataTypeSize, const IVertexArrayObjectFactory& arrayFactory, uint32 numElements);
		
		//
		// Creates a new render target based on the supplied width, height and format.
		//
		// @param width The render targets width
		// @param height The render targets height
		// @param format What types of components should be in use for this render target
		// @throws RenderingException If an error occured while creating the render target
		RenderTarget2D* CreateRenderTarget2D(uint32 width, uint32 height, TextureFormat::Enum format);

	// IWindowResizedListener
	public:
		virtual void OnWindowResized(uint32 width, uint32 height);
		
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
		LinkedList<GfxProgram, &GfxProgram::Link> mGfxPrograms;

		GLuint mFrameBufferId;
		RenderTarget2D* mRenderTargets[MaxDrawBuffers];
		RenderTarget2D* mDepthRenderTarget;

		PositionVAOFactory mPositionVAOFactory;
		PositionTexCoordVAOFactory mPositionTexCoordVAOFactory;
		PositionTexCoordColorVAOFactory mPositionTexCoordColorVAOFactory;
		PositionNormalVAOFactory mPositionNormalVAOFactory;
		PositionColorVAOFactory mPositionColorVAOFactory;
		PositionNormalTextureVAOFactory mPositionNormalTextureVAOFactory;
	};
}
