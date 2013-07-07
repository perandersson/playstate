#pragma once
#include "../scene/scene_node.h"
#include "../math/color.h"

namespace playstate
{
	//
	// Class which defines a point light inside a scene group.
	class PointLight : public SceneNode
	{
	public:
		//
		// @param group
		PointLight(SceneGroup* group);

		//
		// @param group
		// @param typeMask
		PointLight(SceneGroup* group, type_mask typeMask);

		//
		//
		void SetLightColor(const Color& color);

		void SetRadius(float radius);

		void SetConstantAttenuation(float constantAttenuation);

		void SetLinearAttenuation(float linearAttenuation);

		void SetQuadricAttenuation(float quadricAttenuation);

	public:
		Color& const LightColor;

		const float& Radius;

		const float& ConstantAttenuation;

		const float& LinearAttenuation;

		const float& QuadricAttenuation;

	private:
		Color mLightColor;
		float mRadius;
		float mConstantAttenuation;
		float mLinearAttenuation;
		float mQuadricAttenuation;
	};

	//
	// Script integration
	//

	extern int PointLight_Factory(lua_State* L);
	extern int PointLight_SetLightColor(lua_State* L);
	extern int PointLight_SetRadius(lua_State* L);
	extern int PointLight_SetConstantAttenuation(lua_State* L);
	extern int PointLight_SetLinearAttenuation(lua_State* L);
	extern int PointLight_SetQuadricAttenuation(lua_State* L);
	static luaL_Reg PointLight_Methods[] = {
		{ LUA_CONSTRUCTOR, PointLight_Factory },
		{ "SetLightColor", PointLight_SetLightColor },
		{ "SetRadius", PointLight_SetRadius },
		{ "SetConstantAttenuation", PointLight_SetConstantAttenuation },
		{ "SetLinearAttenuation", PointLight_SetLinearAttenuation },
		{ "SetQuadricAttenuation", PointLight_SetQuadricAttenuation },
		{ NULL, NULL }
	};
}
