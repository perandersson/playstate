#include "../../memory/memory.h"
#include "octree_render_processor.h"
using namespace playstate;

OctreeRenderProcessor::OctreeRenderProcessor() : mOctree(3), mRenderables(offsetof(Renderable, RenderableLink))
{
}

OctreeRenderProcessor::~OctreeRenderProcessor()
{
	mRenderables.DeleteAll();
}

void OctreeRenderProcessor::AttachRenderable(Renderable* renderable)
{
	assert_not_null(renderable);

	mRenderables.AddLast(renderable);
	mOctree.Add(renderable);
}

void OctreeRenderProcessor::DetachRenderable(Renderable* renderable)
{
	assert_not_null(renderable);

	mRenderables.Remove(renderable);
	mOctree.Remove(renderable);
}

class RenderableEventHandlerVisitor : public ISpatialTreeVisitor
{
public:
	RenderableEventHandlerVisitor(RenderState state, RenderBlockResultSet* target)
		: mRenderState(state), mResultSetTarget(target), mFoundResults(false)
	{
		assert_not_null(target);
	}
	
	virtual ~RenderableEventHandlerVisitor()
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

bool OctreeRenderProcessor::Find(const FindQuery& query, RenderBlockResultSet* target) const
{
	RenderState state;
	state.Camera = query.Camera;
	state.Filter = query.Filter;

	RenderableEventHandlerVisitor visitor(state, target);
	mOctree.Find(state.Camera->GetViewFrustum(), &visitor);
	return visitor.HasFoundResults();
}
