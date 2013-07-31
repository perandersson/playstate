#include <playstate/scene/scene.h>
#include "../test/test.h"
#include <vector>
using namespace playstate;

TEST_SUITE(Scene)
{
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

		virtual bool Find(const FindQuery& query, RenderBlockResultSet* target) const
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

	class MockLightSourceProcessor : public ILightSourceProcessor
	{
	public:
		virtual void AttachLightSource(LightSource* lightSource)
		{
		}

		virtual void DetachLightSource(LightSource* lightSource)
		{
		}

		virtual bool Find(const FindQuery& query, LightSourceResultSet* target) const
		{
			return true;
		}
	};

	class MockLightSourceProcessorFactory : public ILightSourceProcessorFactory
	{
	public:
		virtual ILightSourceProcessor* Create() const
		{
			return new MockLightSourceProcessor();
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
			Renderable::Attach(GetNode()->GetGroup());
		}

		virtual void OnComponentRemoved()
		{
			Renderable::Detach();
		}

		virtual void CollectBuildingBlocks(RenderBlockResultSet& builder, RenderState& state)
		{
		}
	};

	UNIT_TEST(Scene_FindNonExistingItems)
	{
		const Scene scene;
		const FindQuery query = { NULL, 0 };

		RenderBlockResultSet resultSet;
		ASSERT_FALSE(scene.Find(query, &resultSet));
	}

	UNIT_TEST(Scene_FindRenderableItems)
	{
		MockUpdateProcessFactory updateProcessorFactory;
		MockRenderProcessorFactory renderProcessorFactory;
		MockLightSourceProcessorFactory lightSourceProcessorFactory;

		Scene scene;
		SceneGroup* group = new SceneGroup(updateProcessorFactory, renderProcessorFactory, lightSourceProcessorFactory);
		SceneNode* node1 = new SceneNode(group);
		node1->AddComponent(new MockRenderableComponent());
		SceneNode* node2 = new SceneNode(group);
		scene.AddSceneGroup(group);
	
		const FindQuery query = { NULL, 0 };
	
		RenderBlockResultSet resultSet;
		ASSERT_TRUE(scene.Find(query, &resultSet));
	}
}
