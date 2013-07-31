#pragma once

#include "../resources/resource_object.h"
#include "../collision/aabb.h"
#include "model_mesh.h"

namespace playstate
{
	class Model : public ResourceObject
	{
	public:
		//
		// Constructor
		// @param boundingBox
		Model(const AABB& boundingBox, uint32 numMeshes, ModelMesh* meshes);

		//
		// Destructor
		virtual ~Model();

		//
		// @return This model's bounding box
		const AABB& GetBoundingBox() const;

		//
		// @return
		uint32 GetNumMeshes() const;

		//
		// @return
		const ModelMesh* GetMeshes() const;
	
		ModelMesh* GetMeshes();	

	private:
		AABB mBoundingBox;
		uint32 mNumMeshes;
		ModelMesh* mMeshes;
	};
	
	//
	// Script integration
	//

	extern int Model_Load(lua_State* L);
	static luaL_Reg Model_Methods[] = {
		{ "Load", Model_Load },
		{ NULL, NULL }
	};
}
