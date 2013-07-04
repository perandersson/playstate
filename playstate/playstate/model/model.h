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
		
	public:
		// Read-only property for this objects bounding box
		const AABB& BoundingBox;
		
		// Read-only property about the number of meshes
		const uint32& Size;

		// Read-only array for the actual mesh data
		ModelMesh*& const Meshes;

	private:
		AABB mBoundingBox;

		uint32 mSize;
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
