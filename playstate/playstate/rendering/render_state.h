#pragma once

#include "../rendering/gfx_program.h"
#include "../camera/camera.h"

namespace playstate
{
	enum RenderStateFilter
	{
		Geometry = BIT(1),
		DiffuseTexture = BIT(2),

		AllRenderStateFilters = Geometry | DiffuseTexture
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
