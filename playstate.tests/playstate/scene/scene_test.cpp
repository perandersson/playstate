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

	class MockUpdateProcessor : public IUpdateProcessor
	{
	public:
		virtual void AttachUpdatable(IUpdatable* updatable)
		{
		}

		virtual void DetachUpdatable(IUpdatable* updatable)
		{
		}

		virtual void AttachTickable(ITickable* tickable)
		{
		}

		virtual void DetachTickable(ITickable* tickable)
		{
		}

		virtual void Update()
		{
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

		virtual void PreRender(const RenderState& state, RenderBlockResultSet* builder)
		{
		}
	};

	UNIT_TEST(Scene_FindNonExistingItems)
	{
		Scene scene;
		const FindQuery query = { NULL, 0 };

		RenderBlockResultSet resultSet;
		ASSERT_FALSE(scene.Find(query, &resultSet));
	}

	UNIT_TEST(Scene_FindRenderableItems)
	{
		std::auto_ptr<IUpdateProcessor> up(new MockUpdateProcessor());
		std::auto_ptr<IRenderProcessor> rp(new MockRenderProcessor());
		std::auto_ptr<ILightSourceProcessor> lp(new MockLightSourceProcessor());

		Scene scene;
		SceneGroup* group = new SceneGroup(up, rp, lp);
		SceneNode* node1 = new SceneNode();
		group->AddChild(node1);
		node1->AddComponent(new MockRenderableComponent());
		SceneNode* node2 = new SceneNode();
		group->AddChild(node2);
		scene.AddSceneGroup(group);
	
		const Camera camera;
		const FindQuery query = { &camera, 0 };
	
		RenderBlockResultSet resultSet;
		ASSERT_TRUE(scene.Find(query, &resultSet));
	}
}
