#pragma once

#include "../camera/camera.h"

namespace playstate
{
	//
	// Structure which defines a search query used by the game engine. 
	// Use it to find things like "all visible renderable items"
	struct FindQuery
	{
		//
		// Search for items from the supplied cameras perspective
		const Camera* Camera;

		// 
		// Specifies what type of filter 
		type_mask Filter;
	};
}
