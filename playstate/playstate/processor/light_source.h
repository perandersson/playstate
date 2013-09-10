#pragma once
#include "../linked_list.h"
#include "../scene/spatial_node.h"

namespace playstate
{
	class ILightSourceProcessor;

	//
	// A LightSource is represented as an invisible mesh located in a SceneGroup's Octree. This makes it possible
	// to find visible lights using the camera.
	class LightSource : public SpatialNode
	{
	public:
		LinkedListLink<LightSource> LightSourceLink;

	public:
		LightSource();
		~LightSource();

		//
		// Attaches this light source so that it can affect renderable items
		// @param processor
		void Attach(ILightSourceProcessor* processor);

		//
		// Detaches this light source from the scene
		void Detach();

	private:
		ILightSourceProcessor* mAttachedToProcessor;
	};
}
