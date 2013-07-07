#include "../memory/memory.h"
#include "point_light.h"
#include "../scene/scene_group.h"
using namespace playstate;

PointLight::PointLight(SceneGroup* group)
	: SceneNode(group),
	mRadius(0), mConstantAttenuation(0), mLinearAttenuation(0), mQuadricAttenuation(0),
	LightColor(mLightColor), Radius(mRadius), ConstantAttenuation(mConstantAttenuation), LinearAttenuation(mLinearAttenuation), QuadricAttenuation(mQuadricAttenuation)
{
}

PointLight::PointLight(SceneGroup* group, type_mask typeMask)
	: SceneNode(group, typeMask),
	mRadius(0), mConstantAttenuation(0), mLinearAttenuation(0), mQuadricAttenuation(0),
	LightColor(mLightColor), Radius(mRadius), ConstantAttenuation(mConstantAttenuation), LinearAttenuation(mLinearAttenuation), QuadricAttenuation(mQuadricAttenuation)
{
}

void PointLight::SetLightColor(const Color& color)
{
	mLightColor = color;
}

void PointLight::SetRadius(float radius)
{
	mRadius = radius;
}

void PointLight::SetConstantAttenuation(float constantAttenuation)
{
	mConstantAttenuation = constantAttenuation;
}

void PointLight::SetLinearAttenuation(float linearAttenuation)
{
	mLinearAttenuation = linearAttenuation;
}

void PointLight::SetQuadricAttenuation(float quadricAttenuation)
{
	mQuadricAttenuation = quadricAttenuation;
}

namespace playstate
{
	int PointLight_Factory(lua_State* L)
	{
		SceneGroup* group = luaM_popobject<SceneGroup>(L);
		if(group != NULL) {
			PointLight* node = new PointLight(group);
			luaM_pushobject(L, "PointLight", node);
		} else {
			lua_pushnil(L);
		}
		return 1;
	}

	int PointLight_SetLightColor(lua_State* L)
	{
		Color lightColor = luaM_popcolor(L);
		PointLight* node = luaM_popobject<PointLight>(L);
		if(node != NULL) {
			node->SetLightColor(lightColor);
		}

		return 0;
	}

	int PointLight_SetRadius(lua_State* L)
	{
		float value = lua_tonumber(L, -1); lua_pop(L, 1);
		PointLight* node = luaM_popobject<PointLight>(L);
		if(node != NULL) {
			node->SetRadius(value);
		}

		return 0;
	}

	int PointLight_SetConstantAttenuation(lua_State* L)
	{
		float value = lua_tonumber(L, -1); lua_pop(L, 1);
		PointLight* node = luaM_popobject<PointLight>(L);
		if(node != NULL) {
			node->SetConstantAttenuation(value);
		}

		return 0;
	}

	int PointLight_SetLinearAttenuation(lua_State* L)
	{
		float value = lua_tonumber(L, -1); lua_pop(L, 1);
		PointLight* node = luaM_popobject<PointLight>(L);
		if(node != NULL) {
			node->SetLinearAttenuation(value);
		}

		return 0;
	}

	int PointLight_SetQuadricAttenuation(lua_State* L)
	{
		float value = lua_tonumber(L, -1); lua_pop(L, 1);
		PointLight* node = luaM_popobject<PointLight>(L);
		if(node != NULL) {
			node->SetQuadricAttenuation(value);
		}

		return 0;
	}

}
