#pragma once

#include "../linked_list.h"
#include "../script/scriptable.h"

namespace playstate
{
	class CanvasGroup;

	//
	// 
	class GuiControl : public Scriptable
	{
	public:
		LinkedListLink<GuiControl> GuiControlLink;

	public:
		GuiControl(CanvasGroup* group);
		virtual ~GuiControl();

		//
		// @return The canvas group
		CanvasGroup* GetCanvasGroup();

		// 
		// @return The canvas group
		const CanvasGroup* GetCanvasGroup() const;

	private:
		CanvasGroup* mCanvasGroup;
	};

}
