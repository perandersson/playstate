#pragma once
#include "../render_processor.h"
#include "../../scene/tree/quadtree.h"
#include "../../script/scriptable.h"

namespace playstate
{
	//
	// Container for handling renderable objects
	class QuadTreeRenderProcessor : public IRenderProcessor, public Scriptable
	{
	public:
		QuadTreeRenderProcessor(const AABB& boundingBox, uint32 maxDepth);
		QuadTreeRenderProcessor();
		virtual ~QuadTreeRenderProcessor();
				
	// IRenderProcessor
	public:
		virtual void AttachRenderable(SceneNode* node);
		virtual void DetachRenderable(SceneNode* node);
		virtual bool Find(const FindQuery& query, RenderBlockResultSet* target) const;

	private:
		LinkedList<SceneNode> mNodes;
		QuadTree mQuadTree;
	};
	
	//
	// Script integration
	//

	extern int QuadTreeRenderProcessor_Factory(lua_State* L);
	static luaL_Reg QuadTreeRenderProcessor_Methods[] = {
		{ LUA_CONSTRUCTOR, QuadTreeRenderProcessor_Factory },
		{ NULL, NULL }
	};
}
