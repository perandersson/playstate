#pragma once
#include "../types.h"
#include "../linked_list.h"

namespace playstate
{
	class IUpdateProcessor;

	//
	// 
	class IUpdatable
	{
	public:
		virtual ~IUpdatable() {}

	public:
		//
		// Method called when the current item is being updated. Is called once every frame.
		virtual void Update() = 0;
	};

	//
	// Base class for any item in the game engine that wants to receive update events.
	class Updatable : public IUpdatable
	{
	public:
		LinkedListLink<Updatable> UpdatableLink;

	public:
		Updatable();
		virtual ~Updatable();
	};

	//
	// Global property containing the time since last update
	extern float32 GameDeltaTime;

	//
	// Global property containing the time since beginning of the game
	extern float64 GameTotalTime;
}
