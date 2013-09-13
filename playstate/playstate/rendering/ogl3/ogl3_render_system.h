#pragma once

#include "../render_system.h"
#include "../render_target_2d.h"
#include "../../window/window_resized_listener.h"
#include "../../window/window.h"
#include "../../script/script_system.h"

namespace playstate
{
	class OGL3RenderSystem : public IRenderSystem, public IWindowResizedListener
	{
	public:
		OGL3RenderSystem(IWindow& window, ScriptSystem& scriptSystem);
		virtual ~OGL3RenderSystem();

	// RenderSystem
	public:
		virtual GfxProgram* LoadGfxProgram(const playstate::string& fileName);
		virtual const Version& GetShaderVersion() const;
		virtual VertexBuffer* GetUniformVertexBuffer() const;
		virtual IndexBuffer* CreateStaticBuffer(const uint32* indices, uint32 numIndices);		
		virtual VertexBuffer* CreateStaticBuffer(const PositionData* vertices, uint32 numVertices);
		virtual VertexBuffer* CreateStaticBuffer(const PositionTexCoordData* vertices, uint32 numVertices);
		virtual VertexBuffer* CreateStaticBuffer(const PositionTexCoordColorData* vertices, uint32 numVertices);
		virtual VertexBuffer* CreateStaticBuffer(const PositionNormalData* vertices, uint32 numVertices);
		virtual VertexBuffer* CreateStaticBuffer(const PositionColorData* vertices, uint32 numVertices);
		virtual VertexBuffer* CreateStaticBuffer(const PositionNormalTextureData* vertices, uint32 numVertices);
		virtual VertexBuffer* CreateStaticBuffer(const void* vertices, uint32 vertexSize, const IVertexArrayObjectFactory& arrayFactory, uint32 numVertices);
		virtual VertexBuffer* CreateDynamicBuffer(const void* vertices, uint32 vertexSize, const IVertexArrayObjectFactory& arrayFactory, uint32 numVertices);
		virtual RenderTarget2D* CreateRenderTarget2D(const Size& size, TextureFormat::Enum format);
		virtual void SetRenderTarget(RenderTarget2D* renderTarget, GLenum attachmentIndex);
		virtual void SetDepthRenderTarget(RenderTarget2D* renderTarget);
		virtual void ApplyRenderTargets();

	// IWindowResizedListener
	public:
		virtual void OnWindowResized(const Point& newSize);
		
	private:

		//
		// @return The version of the type specified by the name parameter
		Version getVersion(GLenum name) const;

	private:
		IWindow& mWindow;

		VertexBuffer* mUniformVertexBuffer;
		Version mVersion;
		Version mShaderVersion;
		Rect mScreenViewport;

		GfxProgramFactory* mProgramFactory;
		LinkedList<GfxProgram> mGfxPrograms;

		GLuint mFrameBufferId;
		RenderTarget2D* mRenderTargets[MaxDrawBuffers];
		RenderTarget2D* mDepthRenderTarget;
	};
}