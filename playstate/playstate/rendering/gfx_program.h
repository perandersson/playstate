#pragma once

#include "../types.h"
#include "../math/matrix4x4.h"
#include "../math/rect.h"
#include "gfx_program_component.h"

#include <string>

namespace playstate
{
	class ITexture2D;
	class IVertexBuffer;
	class IIndexBuffer;
	class IRenderTarget2D;
	class IRenderSystem;

	//
	// Source factor used by blending
	// @see http://www.opengl.org/sdk/docs/man3/xhtml/glBlendFunc.xml
	class SrcFactor
	{
	public:
		enum Enum {
			ZERO = 0,
			ONE,
			SRC_COLOR,
			ONE_MINUS_SRC_COLOR,
			SRC_ALPHA,
			ONE_MINUS_SRC_ALPHA,
			DST_ALPHA,
			ONE_MINUS_DST_ALPHA,
			DST_COLOR,
			ONE_MINUS_DST_COLOR,
			CONSTANT_COLOR,
			ONE_MINUS_CONSTANT_COLOR,
			CONSTANT_ALPHA,
			ONE_MINUS_CONSTANT_ALPHA,

			NUM_SRC_FACTORS
		};

		static const Enum Default = ONE;
	};

	//
	// Destination factor used by blending
	// @see http://www.opengl.org/sdk/docs/man3/xhtml/glBlendFunc.xml
	class DestFactor
	{
	public:
		enum Enum {
			ZERO = 0,
			ONE,
			SRC_COLOR,
			ONE_MINUS_SRC_COLOR,
			SRC_ALPHA,
			ONE_MINUS_SRC_ALPHA,
			DST_ALPHA,
			ONE_MINUS_DST_ALPHA,
			DST_COLOR,
			ONE_MINUS_DST_COLOR,
			CONSTANT_COLOR,
			ONE_MINUS_CONSTANT_COLOR,
			CONSTANT_ALPHA,
			ONE_MINUS_CONSTANT_ALPHA,

			NUM_DEST_FACTORS
		};

		static const Enum Default = ZERO;
	};

	class CullFaces
	{
	public:
		enum Enum {
			DISABLED = 0,
			CW,
			CCW,

			NUM_CULL_FACES
		};

		static const Enum Default = CCW;
	};
	
	class DepthFunc
	{
	public:
		enum Enum {
			NEVER = 0,
			LESS,
			EQUAL,
			LEQUAL,
			GREATER,
			NOTEQUAL,
			GEQUAL,
			ALWAYS,

			NUM_DEPTH_FUNCS
		};

		static const Enum Default = LEQUAL;
	};

	class ClearTypes
	{
	public:
		enum Enum {
			COLOR = BIT(1),
			DEPTH = BIT(2)
		};
	};
	
	static const uint32 MaxDrawBuffers = 6;
	static const uint32 MaxActiveTextures = 16; 

	class IGfxProgram
	{
	public:
		virtual ~IGfxProgram() {}

		//
		// Sets this graphics program as the active one the graphics card.
		virtual void Apply() = 0;

		//
		// Clear the view-port based on this programs assigned clear colors.
		// @param clearBits
		virtual void Clear(uint32 clearBits) = 0;
	
		//
		// Retrieves a component based on it's uniform variable name in one of the shaders.
		//
		// @param name The name of the component
		// @return The component if found; A pointer to an GfxProgramComponentNotFound instance (to prevent crashes and unneccessary null checks) if
		//	no component is found.
		virtual IGfxProgramComponent* FindComponent(const char* name) = 0;
		virtual IGfxProgramComponent* FindComponent(const playstate::string& name) = 0;

		//
		// Draw the supplied buffer object using this graphics program.
		//
		// @param buffer
		virtual void Render(IVertexBuffer* buffer) = 0;

		//
		// Draw the supplied buffer object using the supplied index buffer using this graphics program.
		//
		// @param buffer
		// @param IIndexBuffer
		virtual void Render(IVertexBuffer* buffer, IIndexBuffer* IIndexBuffer) = 0;
		
		//
		// Draw the supplied buffer object using the supplied index buffer using this graphics program.
		//
		// @param buffer The vertex buffer we want to draw. Cannot be {@code NULL}.
		// @param indexBuffer The indices of the supplied buffer we want to draw. Can be {@code NULL}
		// @param startIndex The index of the first vertex we want to draw
		virtual void Render(IVertexBuffer* buffer, IIndexBuffer* indexBuffer, uint32 startIndex) = 0;
		
		//
		// Draw the supplied buffer object using the supplied index buffer using this graphics program.
		//
		// @param buffer The vertex buffer we want to draw. Cannot be {@code NULL}.
		// @param indexBuffer The indices of the supplied buffer we want to draw. Can be {@code NULL}
		// @param startIndex The index of the first vertex we want to draw
		// @param numVertices how many vertices we want to draw
		virtual void Render(IVertexBuffer* buffer, IIndexBuffer* indexBuffer, uint32 startIndex, uint32 numVertices) = 0;

		//
		// Flags if this program should enable or disable Z-writing.
		//
		// @default true
		// @param enable
		virtual void EnableDepthTest(bool enable) = 0;

		//
		// Sets the depth function used by the shader during rendering
		// @param depthFunc
		virtual void SetDepthFunc(DepthFunc::Enum depthFunc) = 0;

		//
		// Flags that this program should enable blending when render. Blending if disabled by default.
		// If the program isnt used then you notify it to use it the next time it's applied
		// @param enable
		virtual void EnableBlend(bool enable) = 0;

		//
		// Glags that this program should use the supplied source blend function while render.
		// If the program isnt used then you notify it to use it the next time it's applied
		//
		// @default SrcFactor::ONE, DestFactor::ZERO
		// @param sfactor
		// @param dfactor.
		virtual void SetBlendFunc(SrcFactor::Enum sfactor, DestFactor::Enum dfactor) = 0;

		//
		// Enable and set cull faces for the geometry drawn by this shader.
		//
		// @default CullFaces::CCW
		// @param cullFaces
		virtual void SetCullFaces(CullFaces::Enum cullFaces) = 0;

		//
		// Sets the color this shader should use then clearing the screen.
		//
		// @default [0, 0, 0, 0]
		// @param color
		virtual void SetClearColor(const Color& color) = 0;
		
		//
		// Enables/Disables sicssor test.
		//
		// @param enable
		virtual void EnableScissorTest(bool enable) = 0;

		//
		// Set
		virtual void SetScissorRect(const Rect& rect) = 0;

		//
		// Enables rendering to the supplied render target.
		// @param renderTarget
		virtual void SetDepthRenderTarget(IRenderTarget2D* renderTarget) = 0;

		//
		// Enables rendering to the supplied render target at the supplied index value.
		// @param renderTarget
		// @param index
		virtual void SetRenderTarget(IRenderTarget2D* renderTarget, uint32 index) = 0;
	};
}
