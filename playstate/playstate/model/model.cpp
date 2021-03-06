#include "../memory/memory.h"
#include "model.h"
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
