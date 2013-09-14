#pragma once

#include "../gfx_program.h"
#include "../../linked_list.h"

namespace playstate
{
	class OGL3GfxProgram : public IGfxProgram
	{
		typedef std::hash_map<playstate::string, IGfxProgramComponent*> ComponentMap;

	public:
		LinkedListLink<OGL3GfxProgram> Link;

	public:
		OGL3GfxProgram(IRenderSystem& renderSystem);
		OGL3GfxProgram(GLuint programId, IRenderSystem& renderSystem, const ScriptCollection& collection);
		virtual ~OGL3GfxProgram();

		//
		// Marks this graphics program as dirty. This is used by the game engine to re-bind all this program's resources and components.
		// @remark It's not neccessary for you to do this by you're self. This is done by the game engine. This is usually done on a window- resize.
		void MarkAsDirty();
	
		void ApplyComponents();
		void ApplyBuffers(VertexBuffer* buffer, IndexBuffer* indexBuffer);
		void Prepare(const ScriptCollection& collection);
		
		//
		// @return TRUE if this program is the one active on the graphics card on the current context; FALSE otherwise.
		inline bool IsApplied() const {
			return mApplied;
		}

	// IGfxProgram
	public:
		virtual void Apply();
		virtual void Clear(uint32 clearBits);
		virtual IGfxProgramComponent* FindComponent(const char* name);
		virtual IGfxProgramComponent* FindComponent(const playstate::string& name);
		virtual void Render(VertexBuffer* buffer);
		virtual void Render(VertexBuffer* buffer, IndexBuffer* indexBuffer);
		virtual void Render(VertexBuffer* buffer, IndexBuffer* indexBuffer, uint32 startIndex);
		virtual void Render(VertexBuffer* buffer, IndexBuffer* indexBuffer, uint32 startIndex, uint32 numVertices);
		virtual void EnableDepthTest(bool enable);
		virtual void SetDepthFunc(DepthFunc::Enum depthFunc);
		virtual void EnableBlend(bool enable);
		virtual void SetBlendFunc(SrcFactor::Enum sfactor, DestFactor::Enum dfactor);
		virtual void SetCullFaces(CullFaces::Enum cullFaces);
		virtual void SetClearColor(const Color& color);
		virtual void EnableScissorTest(bool enable);
		virtual void SetScissorRect(const Rect& rect);
		virtual void SetDepthRenderTarget(RenderTarget2D* renderTarget);
		virtual void SetRenderTarget(RenderTarget2D* renderTarget, uint32 index);

	private:
		IRenderSystem& mRenderSystem;
		
		GLuint mProgramId;

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

		bool mScissorTest;
		Rect mScissorRect;

		RenderTarget2D* mDepthRenderTarget;
		RenderTarget2D* mRenderTargets[MaxDrawBuffers];
		bool mApplyRenderTarget;
	};
}