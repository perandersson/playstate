#include <playstate/scene/scene.h>
#include "../test.h"
using namespace playstate;

TestCase(Scene_FindNonExistingItems, "Scene.Find(...) == false")
{
	const Scene scene;
	const FindQuery query = { NULL, 0 };

	FindResultSet<RenderBlock> resultSet;
	AssertFalse(scene.Find(query, &resultSet));
}
