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
			TEXTURES = BIT(1),
			USER_INTERFACE = BIT(2)
		};
	};

	//
	// The current state for the rendering engine.
	struct RenderState
	{
		//
		// Search for items from the supplied cameras perspective
		const Camera* Camera;

		//
		// Filter
		type_mask Filter;
	};
}
