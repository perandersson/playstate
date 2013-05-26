#include <playstate/scene/scene.h>
#include "../test.h"
#include <vector>
using namespace playstate;

class MockRenderProcessor : public IRenderProcessor
{
public:
	virtual void AttachRenderable(Renderable* renderable)
	{
		items.push_back(renderable);
	}
	
	virtual void DetachRenderable(Renderable* renderable)
	{
		std::vector<Renderable*>::iterator it = std::find(items.begin(), items.end(), renderable);
		items.erase(it);
	}

	virtual bool Find(const FindQuery& query, FindResultSet<RenderBlock>* target) const
	{
		
		return true;
	}

	std::vector<Renderable*> items;
};

class MockRenderProcessorFactory : public IRenderProcessorFactory
{
public:
	virtual IRenderProcessor* Create() const
	{
		return new MockRenderProcessor();
	}
};

class MockUpdateProcessor : public IUpdateProcessor
{
public:
	virtual void AttachUpdatable(IUpdatable* updatable)
	{
	}

	virtual void DetachUpdatable(IUpdatable* updatable)
	{
	}

	virtual void Update()
	{
	}
};

class MockUpdateProcessFactory : public IUpdateProcessorFactory
{
public:
	virtual IUpdateProcessor* Create() const
	{
		return new MockUpdateProcessor();
	}
};

class MockRenderableComponent : public Component, public Renderable
{
public:
	MockRenderableComponent()
	{
	}

	virtual ~MockRenderableComponent()
	{
	}

	virtual void OnComponentAdded()
	{
		Renderable::Attach(Owner->Group);
	}

	virtual void OnComponentRemoved()
	{
		Renderable::Detach();
	}

	virtual void CollectBuildingBlocks(RenderBlockBuilder& builder, RenderState& state)
	{
	}
};

TestCase(Scene_FindNonExistingItems, "Scene.Find(...) == false")
{
	const Scene scene;
	const FindQuery query = { NULL, 0 };

	FindResultSet<RenderBlock> resultSet;
	AssertFalse(scene.Find(query, &resultSet));
}

TestCase(Scene_FindRenderableItems, "Scene.Find(NULL, TempType) == true")
{
	MockUpdateProcessFactory updateProcessorFactory;
	MockRenderProcessorFactory renderProcessorFactory;

	Scene scene;
	SceneGroup* group = new SceneGroup(updateProcessorFactory, renderProcessorFactory);
	SceneNode* node1 = new SceneNode(group);
	node1->AddComponent(new MockRenderableComponent());
	SceneNode* node2 = new SceneNode(group);
	scene.AddSceneGroup(group);
	
	const FindQuery query = { NULL, 0 };
	
	FindResultSet<RenderBlock> resultSet;
	AssertTrue(scene.Find(query, &resultSet));
}
