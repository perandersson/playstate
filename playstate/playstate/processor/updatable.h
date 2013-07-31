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
		LinkedListLink<IUpdatable> UpdatableLink;

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
		Updatable();
		virtual ~Updatable();

	protected:
		//
		// Attach this updatable instance to an update processor. 
		// This must be done if we want the updatable instance to receive update events.
		// @param processor
		void Attach(IUpdateProcessor* processor);

		//
		// Detaches this instance from it's attached update processor. This can be done to prevent
		// this item from receiving any update events.
		void Detach();
	};

	//
	// Global property containing the time since last update
	extern float32 GameDeltaTime;

	//
	// Global property containing the time since beginning of the game
	extern float64 GameTotalTime;
}
