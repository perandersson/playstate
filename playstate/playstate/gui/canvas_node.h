#pragma once
#include "../linked_list.h"
#include "../script/scriptable.h"

namespace playstate
{
	class CanvasGroup;

	class CanvasNode : public Scriptable
	{
	public:
		LinkedListLink<CanvasNode> CanvasNodeLink;

	public:
		CanvasNode(CanvasGroup* group);
		virtual ~CanvasNode();

		//
		// @return The canvas group this node is attached to
		CanvasGroup* GetCanvasGroup();

	private:
		CanvasGroup* mCanvasGroup;
	};
}
