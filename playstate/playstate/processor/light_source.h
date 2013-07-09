#pragma once
#include "../linked_list.h"

namespace playstate
{
	class ILightSourceProcessor;

	class LightSource
	{
	public:
		LinkedListLink<LightSource> LightSourceLink;

	public:
		//
		// Constructor
		LightSource();

		//
		// Destructor
		~LightSource();

		//
		// Attaches this light source so that it can affect renderable items
		// @param processor
		void Attach(ILightSourceProcessor* processor);

		//
		// Detaches this light source from the scene
		void Detach();
	};
}
