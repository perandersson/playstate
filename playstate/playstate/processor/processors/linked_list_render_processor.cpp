#include "../../memory/memory.h"
#include "linked_list_render_processor.h"
using namespace playstate;

LinkedListRenderProcessor::LinkedListRenderProcessor() 
	: Scriptable(), mRenderables(offsetof(Renderable, RenderableLink))
{
}

LinkedListRenderProcessor::~LinkedListRenderProcessor()
{
	mRenderables.DeleteAll();
}

void LinkedListRenderProcessor::AttachRenderable(Renderable* renderable)
{
	assert_not_null(renderable);

	mRenderables.AddLast(renderable);
}

void LinkedListRenderProcessor::DetachRenderable(Renderable* renderable)
{
	assert_not_null(renderable);

	mRenderables.Remove(renderable);
}

bool LinkedListRenderProcessor::Find(const FindQuery& query, RenderBlockResultSet* target) const
{
	RenderState state;
	state.Camera = query.Camera;
	state.Filter = query.Filter;

	const Frustum& frustum = query.Camera->GetViewFrustum();
	const uint32 numResults = target->GetNumResults();
	Renderable* renderable = mRenderables.First();
	while(renderable != NULL) {
		Renderable* next = renderable->RenderableLink.Tail;
		AABB::CollisionResult result = frustum.IsColliding(renderable->GetBoundingBox());
		if(result != AABB::OUTSIDE) {
			renderable->PreRender(state, target);
		}
		renderable = next;
	}

	return target->GetNumResults() > numResults;
}

int playstate::LinkedListRenderProcessor_Factory(lua_State* L)
{
	LinkedListRenderProcessor* processor = new LinkedListRenderProcessor();
	luaM_pushobject(L, "LinkedListRenderProcessor", processor);
	return 1;
}
