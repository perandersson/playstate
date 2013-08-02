#pragma once

#include "canvas_group.h"

namespace playstate
{
	//
	//
	class Canvas
	{
	public:
		Canvas();
		~Canvas();

		//
		// @param group
		void AddCanvasGroup(CanvasGroup* group);

		//
		// @param group
		void RemoveCanvasGroup(CanvasGroup* group);

		//
		// Update the canvas groups
		void Update();

		//void Find(

	private:
		LinkedList<CanvasGroup, &CanvasGroup::CanvasGroupLink> mGroups;
	};

	extern int Canvas_AddCanvasGroup(lua_State* L);
	extern int Canvas_RemoveCanvasGroup(lua_State* L);

	static luaL_Reg Canvas_Methods[] = {
		{ "AddCanvasGroup", Canvas_AddCanvasGroup },
		{ "RemoveCanvasGroup", Canvas_RemoveCanvasGroup },
		{ NULL, NULL }
	};
}
