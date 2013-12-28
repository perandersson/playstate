#include <playstate/scene/scene.h>
#include "../test/test.h"
#include <vector>
using namespace playstate;

TEST_SUITE(Scene)
{
	class MockRenderProcessor : public IRenderProcessor
	{
	public:
		virtual void AttachRenderable(SceneNode* node)
		{
			items.push_back(node);
		}
	
		virtual void DetachRenderable(SceneNode* node)
		{
			std::vector<SceneNode*>::iterator it = std::find(items.begin(), items.end(), node);
			items.erase(it);
		}

		virtual bool Find(const FindQuery& query, RenderBlockResultSet* target) const
		{
			return true;
		}

		std::vector<SceneNode*> items;
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
		SceneNode* node2 = new SceneNode();
		group->AddChild(node2);
		scene.AddSceneGroup(group);
	
		const Camera camera;
		const FindQuery query = { &camera, 0 };
	
		RenderBlockResultSet resultSet;
		ASSERT_TRUE(scene.Find(query, &resultSet));
	}
}
