#pragma once

#include "../scene/scene.h"
#include "../gui/canvas.h"
#include "../camera/camera.h"

namespace playstate
{
	//
	// An interface which defines how a scene should be rendered on the screen. 
	class IRenderPipeline
	{
	public:
		virtual ~IRenderPipeline() {}

	public:
		//
		// Method called when the the supplied scene should be drawn.
		//
		// @param scene 
		// @param canvas
		// @param camera
		virtual void Render(Scene& scene, Canvas& canvas, const Camera& camera) = 0;
	};
}

