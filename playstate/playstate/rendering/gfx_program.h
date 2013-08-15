#pragma once

#include "../types.h"
#include "../linked_list.h"
#include "../math/matrix4x4.h"
#include "gfx_program_component.h"
#include "components/gfx_program_component_not_found.h"
#include "state/state_policy.h"
#include "../script/script_collection.h"

#include <string>

namespace playstate
{
	class Texture2D;
	class VertexBuffer;
	class IndexBuffer;
	class RenderTarget2D;
	class RenderSystem;
	
	const uint32 MaxDrawBuffers = 6;

	class GfxProgram
	{
		typedef std::hash_map<std::string, IGfxProgramComponent*> ComponentMap;

	public:
		LinkedListLink<GfxProgram> Link;

	public:
		GfxProgram(RenderSystem& renderSystem);
		GfxProgram(GLuint programId, GLuint vertexShader, GLuint pixelShader, GLuint geometryShader, RenderSystem& renderSystem,
			const ScriptCollection& collection);
		~GfxProgram();

		//
		// Sets this graphics program as the active one the graphics card.
		void Apply();

		//
		// Marks this graphics program as dirty. This is used by the game engine to re-bind all this program's resources and components.
		// @remark It's not neccessary for you to do this by you're self. This is done by the game engine. This is usually done on a window- resize.
		void MarkAsDirty();

		//
		// @return TRUE if this program is the one active on the graphics card; FALSE otherwise.
		bool IsApplied() const;

		//
		// Clear the view-port based on this programs assigned clear colors.
		// @param clearBits
		void Clear(uint32 clearBits);
	
		//
		// Retrieves a component based on it's uniform variable name in one of the shaders.
		//
		// @param name The name of the component
		// @return The component if found; A pointer to an GfxProgramComponentNotFound instance (to prevent crashes and unneccessary null checks) if
		//	no component is found.
		IGfxProgramComponent* FindComponent(const char* name);
		IGfxProgramComponent* FindComponent(const std::string& name);

		//
		// Draw the supplied buffer object using this graphics program.
		//
		// @param buffer
		void Render(VertexBuffer* buffer);

		//
		// Draw the supplied buffer object using the supplied index buffer using this graphics program.
		//
		// @param buffer
		// @param indexBuffer
		void Render(VertexBuffer* buffer, IndexBuffer* indexBuffer);
		
		//
		// Draw the supplied buffer object using the supplied index buffer using this graphics program.
		//
		// @param buffer The vertex buffer we want to draw. Cannot be {@code NULL}.
		// @param indexBuffer The indices of the supplied buffer we want to draw. Can be {@code NULL}
		// @param startElement The first element we want to draw
		void Render(VertexBuffer* buffer, IndexBuffer* indexBuffer, uint32 startElement);
		
		//
		// Draw the supplied buffer object using the supplied index buffer using this graphics program.
		//
		// @param buffer The vertex buffer we want to draw. Cannot be {@code NULL}.
		// @param indexBuffer The indices of the supplied buffer we want to draw. Can be {@code NULL}
		// @param startElement The first element we want to draw
		// @param numElements how many elements we want to draw
		void Render(VertexBuffer* buffer, IndexBuffer* indexBuffer, uint32 startElement, uint32 numElements);

		//
		// Flags if this program should enable or disable Z-writing.
		//
		// @default true
		// @param enable
		void EnableDepthTest(bool enable);

		//
		// Sets the depth function used by the shader during rendering
		// @param depthFunc
		void SetDepthFunc(DepthFunc::Enum depthFunc);

		//
		// Flags that this program should enable blending when render. Blending if disabled by default.
		// If the program isnt used then you notify it to use it the next time it's applied
		// @param enable
		void EnableBlend(bool enable);

		//
		// Glags that this program should use the supplied source blend function while render.
		// If the program isnt used then you notify it to use it the next time it's applied
		//
		// @default SrcFactor::ONE, DestFactor::ZERO
		// @param sfactor
		// @param dfactor.
		void SetBlendFunc(SrcFactor::Enum sfactor, DestFactor::Enum dfactor);

		//
		// Enable and set cull faces for the geometry drawn by this shader.
		//
		// @default CullFaces::CCW
		// @param cullFaces
		void SetCullFaces(CullFaces::Enum cullFaces);

		//
		// Sets the color this shader should use then clearing the screen.
		//
		// @default [0, 0, 0, 0]
		// @param color
		void SetClearColor(const Color& color);
		
		//
		// Enables rendering to the supplied render target.
		// @param renderTarget
		void SetDepthRenderTarget(RenderTarget2D* renderTarget);

		//
		// Enables rendering to the supplied render target at the supplied index value.
		// @param renderTarget
		// @param index
		void SetRenderTarget(RenderTarget2D* renderTarget, uint32 index);

	private:
		void ApplyComponents();
		void ApplyBuffers(VertexBuffer* buffer, IndexBuffer* indexBuffer);
		void Prepare(const ScriptCollection& collection);

	private:
		RenderSystem& mRenderSystem;
		
		GLuint mProgramId;
		GLuint mGeometryShader;
		GLuint mVertexShader;
		GLuint mPixelShader;

		ComponentMap mComponents;
		GfxProgramComponentNotFound mComponentNotFound;
		bool mApplied;

	// States
	private:
		bool mDepthTest;
		DepthFunc::Enum mDepthFunc;

		bool mBlend;
		SrcFactor::Enum mSrcFactor; 
		DestFactor::Enum mDestFactor;

		Color mClearColor;
		float32 mClearDepth;

		CullFaces::Enum mCullFaces;

		RenderTarget2D* mDepthRenderTarget;
		RenderTarget2D* mRenderTargets[MaxDrawBuffers];
		bool mApplyRenderTarget;
	};
}
