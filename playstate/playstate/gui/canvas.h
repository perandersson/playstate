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

		void AddCanvasGroup(CanvasGroup* group);

		void RemoveCanvasGroup(CanvasGroup* group);

		//
		// Update this canvas
		void Update();

	// IGuiRenderProcessor
	public:


	private:
		LinkedList<CanvasGroup, &CanvasGroup::CanvasGroupLink> mGroups;
	};
}
