#include "../../memory/memory.h"
#include "quadtree_render_processor.h"
using namespace playstate;

QuadTreeRenderProcessor::QuadTreeRenderProcessor(const AABB& boundingBox, uint32 maxDepth) 
	: Scriptable(), mQuadTree(boundingBox, maxDepth), mRenderables(offsetof(Renderable, RenderableLink))
{
}

QuadTreeRenderProcessor::QuadTreeRenderProcessor() 
	: Scriptable(), mQuadTree(AABB(Vector3(0, 0, 0), 1100.0f, 1100.0f, 1100.0f), 3), mRenderables(offsetof(Renderable, RenderableLink))
{
}

QuadTreeRenderProcessor::~QuadTreeRenderProcessor()
{
	mRenderables.DeleteAll();
}

void QuadTreeRenderProcessor::AttachRenderable(Renderable* renderable)
{
	assert_not_null(renderable);

	mRenderables.AddLast(renderable);
	mQuadTree.Add(renderable);
}

void QuadTreeRenderProcessor::DetachRenderable(Renderable* renderable)
{
	assert_not_null(renderable);

	mRenderables.Remove(renderable);

	QuadTree* tree = static_cast<QuadTree*>(renderable->GetTree());
	if(tree != NULL)
		tree->Remove(renderable);
}

class QuadTreeRenderableEventHandlerVisitor : public ISpatialTreeVisitor
{
public:
	QuadTreeRenderableEventHandlerVisitor(RenderState state, RenderBlockResultSet* target)
		: mRenderState(state), mResultSetTarget(target), mFoundResults(false)
	{
		assert_not_null(target);
	}
	
	virtual ~QuadTreeRenderableEventHandlerVisitor()
	{
	}
	
	bool HasFoundResults() const {
		return mFoundResults;
	}

// IOctreeVisitor
public:
	virtual void Visit(SpatialNode* item)
	{
		static_cast<Renderable*>(item)->PreRender(mRenderState, mResultSetTarget);
		mFoundResults = true;
	}

private:
	RenderState mRenderState;
	RenderBlockResultSet* mResultSetTarget;
	bool mFoundResults;
};

bool QuadTreeRenderProcessor::Find(const FindQuery& query, RenderBlockResultSet* target) const
{
	RenderState state;
	state.Camera = query.Camera;
	state.Filter = query.Filter;

	QuadTreeRenderableEventHandlerVisitor visitor(state, target);
	mQuadTree.Find(state.Camera->GetViewFrustum(), &visitor);
	return visitor.HasFoundResults();
}

int playstate::QuadTreeRenderProcessor_Factory(lua_State* L)
{
	if(lua_gettop(L) < 4)
	{
		luaM_printerror(L, "Expected: QuadTreeRenderProcessor(Vector3, Width, Depth, MaxDepth)");
		lua_pushnil(L);
		return 1;
	}

	uint32 maxDepth = lua_tointeger(L, -1); lua_pop(L, 1);
	float32 depth = luaM_popfloat(L);
	float32 width = luaM_popfloat(L);
	Vector3 position = luaM_popvector3(L);

	const AABB boundingBox(position, width, 10000.0f, depth);
	QuadTreeRenderProcessor* processor = new QuadTreeRenderProcessor(boundingBox, maxDepth);
	luaM_pushobject(L, "LinkedListRenderProcessor", processor);
	return 1;
}
