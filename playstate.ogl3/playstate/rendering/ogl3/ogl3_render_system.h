#pragma once

#include <playstate/rendering/render_system.h>
#include <playstate/window/window_resized_listener.h>
#include <playstate/window/window.h>
#include <playstate/script/script_system.h>
#include "ogl3_gfx_program.h"
#include "gfx_program_factory.h"
#include "ogl3_vertex_declaration_factory.h"

namespace playstate
{
	class OGL3RenderTarget2D;

	class OGL3RenderSystem : public IRenderSystem, public IWindowResizedListener
	{
	public:
		OGL3RenderSystem(IWindow& window, ScriptSystem& scriptSystem);
		virtual ~OGL3RenderSystem();

	// RenderSystem
	public:
		virtual IGfxProgram* LoadGfxProgram(const playstate::string& fileName);
		virtual const Version& GetShaderVersion() const;
		virtual IVertexBuffer* GetUniformVertexBuffer() const;
		virtual IIndexBuffer* CreateStaticBuffer(const uint32* indices, uint32 numIndices);		
		virtual IVertexBuffer* CreateStaticBuffer(const PositionData* vertices, uint32 numVertices);
		virtual IVertexBuffer* CreateStaticBuffer(const PositionTexCoordData* vertices, uint32 numVertices);
		virtual IVertexBuffer* CreateStaticBuffer(const PositionTexCoordColorData* vertices, uint32 numVertices);
		virtual IVertexBuffer* CreateStaticBuffer(const PositionNormalData* vertices, uint32 numVertices);
		virtual IVertexBuffer* CreateStaticBuffer(const PositionColorData* vertices, uint32 numVertices);
		virtual IVertexBuffer* CreateStaticBuffer(const PositionNormalTextureData* vertices, uint32 numVertices);
		virtual IVertexBuffer* CreateStaticBuffer(const void* vertices, uint32 vertexSize, const VertexDeclaration& vertexDeclaration, uint32 numVertices);
		virtual IVertexBuffer* CreateDynamicBuffer(const void* vertices, uint32 vertexSize, const VertexDeclaration& vertexDeclaration, uint32 numVertices);
		virtual IRenderTarget2D* CreateRenderTarget2D(const Size& size, TextureFormat::Enum format);
		virtual void SetRenderTarget(IRenderTarget2D* renderTarget, GLenum attachmentIndex);
		virtual void SetDepthRenderTarget(IRenderTarget2D* renderTarget);
		virtual ITexture2D* CreateTexture2D(const Size& size, TextureFormat::Enum format, const byte* bytes);
		virtual void ApplyRenderTargets();

	// IWindowResizedListener
	public:
		virtual void OnWindowResized(const Point& newSize);
		
	private:

		//
		// @return The version of the type specified by the name parameter
		Version getVersion(GLenum name) const;

		GLenum GetFormat(TextureFormat::Enum format);
		GLenum GetInternalFormat(TextureFormat::Enum format);

	private:
		IWindow& mWindow;

		IVertexBuffer* mUniformVertexBuffer;
		Version mVersion;
		Version mShaderVersion;
		Rect mScreenViewport;

		GfxProgramFactory* mProgramFactory;
		VertexDeclarationArrayObjectFactory* mVAOFactory;

		LinkedList<OGL3GfxProgram> mGfxPrograms;

		GLuint mFrameBufferId;
		OGL3RenderTarget2D* mRenderTargets[MaxDrawBuffers];
		OGL3RenderTarget2D* mDepthRenderTarget;
	};
}