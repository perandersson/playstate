#pragma once

#include "../rendering/gfx_program.h"
#include "../camera/camera.h"

namespace playstate
{
	class RenderStateFilter
	{
	public:
		enum Enum {
			GEOMETRY = BIT(0),
			DIFFUSE_TEXTURE = BIT(1),
			ALL = GEOMETRY | DIFFUSE_TEXTURE
		};

		static const Enum Default = ALL;
	};

	//
	// The current state for the rendering engine.
	struct RenderState
	{
		// The camera
		const Camera* Camera;

		// Filter
		uint32 Filter;
	};
}
