#pragma once

#include "../camera/camera.h"

namespace playstate
{
	//
	// Structure which defines a search query used by the game engine. 
	// Use it to find things like "all visible renderable items"
	struct FindQuery
	{
		const Camera* Camera;
		type_mask Filter;
	};
}
