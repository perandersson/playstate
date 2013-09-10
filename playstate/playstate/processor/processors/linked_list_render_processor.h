#pragma once
#include "../render_processor.h"
#include "../../linked_list.h"
#include "../../script/scriptable.h"

namespace playstate
{
	//
	// A render processor where all the renderable items are put in a first -> last linked list.
	// Useful for those SceneGroups with very few nodes.
	class LinkedListRenderProcessor : public IRenderProcessor, public Scriptable
	{
	public:
		LinkedListRenderProcessor();
		virtual ~LinkedListRenderProcessor();
				
	// IRenderProcessor
	public:
		virtual void AttachRenderable(Renderable* renderable);
		virtual void DetachRenderable(Renderable* renderable);
		virtual bool Find(const FindQuery& query, RenderBlockResultSet* target) const;

	private:
		LinkedList<Renderable> mRenderables;
	};
	
	//
	// Script integration
	//

	extern int LinkedListRenderProcessor_Factory(lua_State* L);
	static luaL_Reg LinkedListRenderProcessor_Methods[] = {
		{ LUA_CONSTRUCTOR, LinkedListRenderProcessor_Factory },
		{ NULL, NULL }
	};
}
