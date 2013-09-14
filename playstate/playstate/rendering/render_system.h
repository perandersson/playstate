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
#include "../math/rect.h"
#include "../math/point.h"
#include "exception/rendering_exception.h"
#include "../version.h"
#include "datatypes/standard_vertex_buffer_types.h"

#include <gl/glew.h>

namespace playstate
{
	//
	// 
	class IRenderSystem : public Singleton<IRenderSystem>
	{
	public:
		//
		// Loads a gfx program and returns the result. 
		//
		// @param fileName
		// @throw playstate::GfxProgramException Exception thrown if the supplied gfx program could not load correctly.
		//		If the configuration flag: "graphics.developmentmode" = true then this method will 
		//		return a default instance instead. (So that you can, while running the game, fix the problems with the shaders)
		// @return
		virtual IGfxProgram* LoadGfxProgram(const playstate::string& fileName) = 0;

		//
		// @return The shader-version on the current computer.
		virtual const Version& GetShaderVersion() const = 0;

		//
		// @return A vertex buffer representing a uniform [-1, 1] buffer.
		virtual IVertexBuffer* GetUniformVertexBuffer() const = 0;

		//
		// Creates an index buffer based on a supplied of indices
		// @return A bindable and renderable index buffer
		// @throws RenderingException
		virtual IIndexBuffer* CreateStaticBuffer(const uint32* indices, uint32 numIndices) = 0;

		
		virtual IVertexBuffer* CreateStaticBuffer(const PositionData* vertices, uint32 numVertices) = 0;
		virtual IVertexBuffer* CreateStaticBuffer(const PositionTexCoordData* vertices, uint32 numVertices) = 0;
		virtual IVertexBuffer* CreateStaticBuffer(const PositionTexCoordColorData* vertices, uint32 numVertices) = 0;
		virtual IVertexBuffer* CreateStaticBuffer(const PositionNormalData* vertices, uint32 numVertices) = 0;
		virtual IVertexBuffer* CreateStaticBuffer(const PositionColorData* vertices, uint32 numVertices) = 0;
		virtual IVertexBuffer* CreateStaticBuffer(const PositionNormalTextureData* vertices, uint32 numVertices) = 0;

		//
		// Creates a static (non-updatable) buffer based on a supplied data stream
		//
		// @param vertices
		// @param vertexSize The size of one data element. {@code sizeof(PositionData)} is one such size
		// @param vertexDeclaration
		// @param numVertices
		virtual IVertexBuffer* CreateStaticBuffer(const void* vertices, uint32 vertexSize, const VertexDeclaration& vertexDeclaration, uint32 numVertices) = 0;

		//
		// Creates a dynamic buffer based on the supplied data stream. You are allowed to update the data contained inside the returned
		// vertex buffer instance. Note that you are not allowed to change the type of data stored in the buffer.
		//
		// @param data
		// @param dataTypeSize The size of one data element. {@code sizeof(PositionData)} is one such size
		// @param vertexDeclaration
		// @param numVertices
		virtual IVertexBuffer* CreateDynamicBuffer(const void* vertices, uint32 vertexSize, const VertexDeclaration& vertexDeclaration, uint32 numVertices) = 0;
		
		//
		// Creates a new render target based on the supplied width, height and format.
		//
		// @param size The render targets size in pixels (width and height)
		// @param format What types of components should be in use for this render target
		// @throws RenderingException If an error occured while creating the render target
		virtual IRenderTarget2D* CreateRenderTarget2D(const Size& size, TextureFormat::Enum format) = 0;
		
		//
		// Sets the supplied render target to the given attachment index number.
		// This is used by GLSL to know where to render the result.
		// @param renderTarget
		// @param attachmentIndex
		virtual void SetRenderTarget(IRenderTarget2D* renderTarget, GLenum attachmentIndex) = 0;

		//
		// Creates a texture based on the supplied data
		// 
		// @param size The size of the texture
		// @param format The format of the texture
		// @param bytes The actual raw texture data.
		virtual ITexture2D* CreateTexture2D(const Size& size, TextureFormat::Enum format, const byte* bytes) = 0;

		//
		// Sets the depth render target
		// @param renderTarget
		virtual void SetDepthRenderTarget(IRenderTarget2D* renderTarget) = 0;

		virtual void ApplyRenderTargets() = 0;
	};
}
