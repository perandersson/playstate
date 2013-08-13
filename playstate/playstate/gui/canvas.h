#pragma once

#include "canvas_group.h"
#include "../linked_list.h"

namespace playstate
{
	class Canvas
	{
	public:
		Canvas();
		~Canvas();

		//
		// Adds the supplied canvas group
		//
		// @param group
		void AddCanvasGroup(CanvasGroup* group);

		//
		// Removes the supplied canvas group - but doesn't delete it
		//
		// @param group
		void RemoveCanvasGroup(CanvasGroup* group);

		//
		// Update this canvas and all it's canvas groups.
		void Update();

	private:
		LinkedList<CanvasGroup, &CanvasGroup::CanvasGroupLink> mGroups;
	};
}
