#include "../../memory/memory.h"
#include "octree_render_processor.h"
using namespace playstate;

// TODO: Make sure that the octree's bounding box is not a static value.
OctreeRenderProcessor::OctreeRenderProcessor() : mOctree(AABB(Vector3(0, 0, 0), 1000.0f, 1000.0f, 1000.0f), 0, 4)
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
	RenderableEventHandlerVisitor(RenderState state)
		: mRenderState(state)
	{
	}
	
	virtual ~RenderableEventHandlerVisitor()
	{
	}

	bool SortAndSave(FindResultSet<RenderBlock>* target)
	{
		// TODO: Make sure that the block builder doesn't use a global pointer list.
		// because we want to be able to use this from multiple threads at the same time.
		// Example = g-buffer rendering and shadow mapping in different threads.
		bool result = mBlockBuilder.SortAndSave(target);
		mBlockBuilder.Clean();
		return result;
	}

// IOctreeVisitor
public:
	virtual void Visit(OctreeNode* item)
	{
		static_cast<Renderable*>(item)->CollectBuildingBlocks(mBlockBuilder, mRenderState);
	}

private:
	RenderState mRenderState;
	RenderBlockBuilder mBlockBuilder;
};

bool OctreeRenderProcessor::Find(const FindQuery& query, FindResultSet<RenderBlock>* target) const
{
	RenderState state;
	state.Camera = query.Camera;

	RenderableEventHandlerVisitor visitor(state);
	mOctree.FindItems(state.Camera->ViewFrustum, &visitor);
	return visitor.SortAndSave(target);
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
