#pragma once

#include "../scene/scene.h"
#include "../camera/camera.h"
#include "../gui/canvas.h"

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
		// @param camera
		virtual void Render(const Scene& scene, const Camera& camera) = 0;

		//
		// Method called when the supplied canvas should be drawn.
		//
		// @param canvas
		virtual void Render(const Canvas& canvas) = 0;
	};
}

