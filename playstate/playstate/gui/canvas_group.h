#pragma once

#include "../linked_list.h"
#include "../script/scriptable.h"
#include "gui_control.h"
#include "../processor/update_processor_factory.h"

namespace playstate
{
	//
	// 
	class CanvasGroup : public Scriptable, public IUpdateProcessor
	{
	public:
		LinkedListLink<CanvasGroup> CanvasGroupLink;

	public:
		CanvasGroup();
		CanvasGroup(IUpdateProcessorFactory& updateProcessFactory);
		virtual ~CanvasGroup();

		//
		// Adds a new control to this canvas group. If the control is already added in 
		//	another canvas group then it will be moved to this group instead.
		//
		// @param control The control we want to add
		void AddControl(GuiControl* control);

		//
		// Removes a control from this canvas group
		// 
		// @param control The control we want to remove
		void RemoveControl(GuiControl* control);

	// IUpdateProcessor
	public:
		virtual void AttachUpdatable(IUpdatable* updatable);
		virtual void DetachUpdatable(IUpdatable* updatable);
		virtual void Update();

	private:
		IUpdateProcessor* mUpdateProcessor;

	private:
		LinkedList<GuiControl, &GuiControl::GuiControlLink> mControls;
	};
	
	extern int CanvasGroup_Factory(lua_State* L);
	extern int CanvasGroup_Init(lua_State* L);
	static luaL_Reg CanvasGroup_Methods[] = {
		{ LUA_CONSTRUCTOR, CanvasGroup_Factory },
		{ LUA_INHERIT_CONSTRUCTOR, CanvasGroup_Init },
		{ NULL, NULL }
	};
}

