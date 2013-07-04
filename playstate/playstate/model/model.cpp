#include "../memory/memory.h"
#include "model.h"
#include "../resources/resource_manager.h"

#include <cassert>

using namespace playstate;

Model::Model(const AABB& boundingBox, uint32 numMeshes, ModelMesh* meshes)
	: mBoundingBox(boundingBox), mMeshes(meshes), mSize(numMeshes), 
	BoundingBox(mBoundingBox), Size(mSize), Meshes(mMeshes)
{
}

Model::~Model()
{
	if(mSize == 0)
		return;

	delete[] mMeshes;
	mMeshes = NULL;
	mSize = 0;
}

namespace playstate
{
	int Model_Load(lua_State* L)
	{
		std::string path = lua_tostring(L, -1); lua_pop(L, 1);
		Resource<Model> model = ResourceManager::Get().GetResource<Model>(path);
		luaM_pushobject(L, "Model", model.GetResourceData());
		return 1;
	}
}
