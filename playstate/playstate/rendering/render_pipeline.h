#pragma once

#include "../script/scriptable.h"

namespace playstate
{
	class Scene;
	class Camera;

	//
	// An interface which defines how a scene should be rendered on the screen. 
	class IRenderPipeline : public Scriptable
	{
	public:
		//
		// Method called when the the supplied scene should be drawn.
		//
		// @param scene 
		// @param camera
		virtual void Render(const Scene& scene, const Camera& camera) = 0;
	};
}

