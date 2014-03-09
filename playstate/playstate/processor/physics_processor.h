#pragma once

#include "../types.h"

namespace playstate
{
	class IPhysicsProcessor
	{
	public:
		virtual ~IPhysicsProcessor() {}

	public:
		//
		// Update the physics processor
		//
		// @param timeStep
		virtual void Step(float32 timeStep) = 0;
	};

}
