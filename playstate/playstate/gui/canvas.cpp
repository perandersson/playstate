#include "../memory/memory.h"
#include "canvas.h"
#include "../game/game_runner.h"
using namespace playstate;

Canvas::Canvas()
{
}

Canvas::~Canvas()
{
	mGroups.DeleteAll();
}


void Canvas::AddCanvasGroup(CanvasGroup* group)
{
	assert_not_null(group);
	mGroups.AddLast(group);
}

void Canvas::RemoveCanvasGroup(CanvasGroup* group)
{
	assert_not_null(group);
	delete group;
}

bool Canvas::Find(GuiBlockResultSet* target) const
{


	return false;
}

bool Canvas::Find(GuiBlockResultSet* target, IArraySorter<GuiRenderBlock*>* sorter) const
{


	target->Sort(sorter);
	return false;
}

// Script

namespace playstate
{
	int Canvas_AddCanvasGroup(lua_State* L)
	{
		CanvasGroup* group = luaM_popobject<CanvasGroup>(L);
		if(group != NULL)
			GameRunner::Get().GetCanvas().AddCanvasGroup(group);

		return 0;
	}

	int Canvas_RemoveCanvasGroup(lua_State* L)
	{
		CanvasGroup* group = luaM_popobject<CanvasGroup>(L);
		if(group != NULL)
			GameRunner::Get().GetCanvas().RemoveCanvasGroup(group);

		return 0;
	}
}
