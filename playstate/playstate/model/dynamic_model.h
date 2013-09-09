#pragma once

#include "../resources/resource_object.h"
#include "model_mesh.h"

namespace playstate
{
	class DynamicModel : public ResourceObject
	{
	public:
		DynamicModel(uint32 numMeshes, ModelMesh* meshes);
		virtual ~DynamicModel();

		//
		// @return
		uint32 GetNumMeshes() const;

		//
		// @return
		const ModelMesh* GetMeshes() const;
		ModelMesh* GetMeshes();

	private:
		uint32 mNumMeshes;
		ModelMesh* mMeshes;
	};
}
