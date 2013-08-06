#include "../memory/memory.h"
#include "model.h"
#include "../resources/resource_manager.h"

#include <cassert>

using namespace playstate;

Model::Model(const AABB& boundingBox, uint32 numMeshes, ModelMesh* meshes)
	: mBoundingBox(boundingBox), mMeshes(meshes), mNumMeshes(numMeshes)
{
}

Model::~Model()
{
	if(mNumMeshes == 0)
		return;

	for(uint32 i = 0; i < mNumMeshes; ++i) {
		if(mMeshes[i].Vertices != NULL)
			delete mMeshes[i].Vertices;

		if(mMeshes[i].Indices != NULL)
			delete mMeshes[i].Indices;
	}

	delete[] mMeshes;
	mMeshes = NULL;
	mNumMeshes = 0;
}

const AABB& Model::GetBoundingBox() const
{
	return mBoundingBox;
}

uint32 Model::GetNumMeshes() const
{
	return mNumMeshes;
}

const ModelMesh* Model::GetMeshes() const
{
	return mMeshes;
}

ModelMesh* Model::GetMeshes()
{
	return mMeshes;
}

namespace playstate
{
	int Model_Load(lua_State* L)
	{
		std::string path = lua_tostring(L, -1); lua_pop(L, 1);
		Resource<Model> model = IResourceManager::Get().GetResource<Model>(path);
		luaM_pushobject(L, "Model", model.GetResourceData());
		return 1;
	}
}
