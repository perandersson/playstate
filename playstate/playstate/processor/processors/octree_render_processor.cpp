#include "../../memory/memory.h"
#include "octree_render_processor.h"
using namespace playstate;

OctreeRenderProcessor::OctreeRenderProcessor() : mOctree(3)
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

class RenderableEventHandlerVisitor : public IOctreeVisitor
{
public:
	RenderableEventHandlerVisitor(RenderState state, RenderBlockResultSet* target)
		: mRenderState(state), mResultSetTarget(target)
	{
		assert_not_null(target);
	}
	
	virtual ~RenderableEventHandlerVisitor()
	{
	}

// IOctreeVisitor
public:
	virtual void Visit(OctreeNode* item)
	{
		static_cast<Renderable*>(item)->CollectBuildingBlocks(*mResultSetTarget, mRenderState);
	}

private:
	RenderState mRenderState;
	RenderBlockResultSet* mResultSetTarget;
};

bool OctreeRenderProcessor::Find(const FindQuery& query, RenderBlockResultSet* target) const
{
	RenderState state;
	state.Camera = query.Camera;

	RenderableEventHandlerVisitor visitor(state, target);
	mOctree.FindItems(state.Camera->ViewFrustum, &visitor);
	return target->Size > 0;
}

OctreeRenderProcessorFactory::OctreeRenderProcessorFactory()
{
}

OctreeRenderProcessorFactory::~OctreeRenderProcessorFactory()
{
}

IRenderProcessor* OctreeRenderProcessorFactory::Create() const
{
	return new OctreeRenderProcessor();
}
