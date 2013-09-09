#include "../memory/memory.h"
#include "dynamic_model.h"
using namespace playstate;

DynamicModel::DynamicModel(uint32 numMeshes, ModelMesh* meshes)
	: mMeshes(meshes), mNumMeshes(numMeshes)
{
}

DynamicModel::~DynamicModel()
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

uint32 DynamicModel::GetNumMeshes() const
{
	return mNumMeshes;
}

const ModelMesh* DynamicModel::GetMeshes() const
{
	return mMeshes;
}

ModelMesh* DynamicModel::GetMeshes()
{
	return mMeshes;
}
