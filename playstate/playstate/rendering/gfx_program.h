#pragma once

#include "../types.h"
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
		GfxProgram(GLuint programId, GLuint vertexShader, GLuint pixelShader, GLuint geometryShader, RenderSystem& renderSystem,
			const ScriptCollection& collection);
		~GfxProgram();

		//
		// 
		void Apply();

		//
		// Clear the view-port based on this programs assigned clear colors.
		// @param clearBits
		void Clear(uint32 clearBits);
	
		//
		// Retrieves a component based on it's variable name in a program.
		// @param name
		// @return 
		IGfxProgramComponent* GetComponent(const char* name);
		IGfxProgramComponent* GetComponent(const std::string& name);

		//
		// Draw the supplied buffer object using this graphics program.
		void Render(VertexBuffer* buffer);
		void Render(VertexBuffer* buffer, IndexBuffer* indexBuffer);

		//
		// Flags if this program should enable or disable Z-writing.
		// Default = true.
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
		// Glags that this program should use the supplied source blend function while render. SOURCE_SRC_ALPHA is used by default.
		// If the program isnt used then you notify it to use it the next time it's applied
		// @param srcFunc
		// @param destFunc.
		void SetBlendFunc(SrcBlend::Enum srcFunc, DestBlend::Enum destFunc);

		//
		// Enable and set cull faces for the geometry drawn by this shader.
		// @param cullFaces
		void SetCullFaces(CullFaces::Enum cullFaces);

		//
		// Sets the color this shader should use then clearing the screen.
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

	public:
		// Read-only property which tells if this program is active or not
		const bool& Applied;

	private:
		void ApplyComponents();
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
		SrcBlend::Enum mSrcFunc; 
		DestBlend::Enum mDestFunc;

		Color mClearColor;
		float32 mClearDepth;

		CullFaces::Enum mCullFaces;

		RenderTarget2D* mDepthRenderTarget;
		RenderTarget2D* mRenderTargets[MaxDrawBuffers];
		bool mApplyRenderTarget;
	};
}
