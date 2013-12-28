#include "../memory/memory.h"
#include "point_light.h"
#include "../scene/scene_group.h"
using namespace playstate;

PointLight::PointLight(const Color& color, float32 radius, float32 constantAttenuation, float32 linearAttenuation, float32 quadricAttenuation)
	: LightSource(), 
	mColor(color), mRadius(radius), mConstantAttenuation(constantAttenuation), mLinearAttenuation(linearAttenuation), mQuadricAttenuation(quadricAttenuation)
{
}

PointLight::~PointLight()
{
}

const Color& PointLight::GetColor()
{
	return mColor;
}

float32 PointLight::GetRadius() const
{
	return mRadius;
}

float32 PointLight::GetConstantAttenuation() const
{
	return mConstantAttenuation;
}

float32 PointLight::GetLinearAttenuation() const
{
	return mLinearAttenuation;
}

float32 PointLight::GetQuadricAttenuation() const
{
	return mQuadricAttenuation;
}

void PointLight::OnAttachedToSceneGroup()
{
	SetBoundingBox(AABB(GetPosition(), 20.0f, 20.0f, 20.0f));
	LightSource::Attach(GetGroup());
}

void PointLight::OnDetachingFromSceneGroup()
{
	LightSource::Detach();
}

int playstate::PointLight_Factory(lua_State* L)
{
	float32 quadricAttenuation = luaM_tofloat(L, -1); lua_pop(L, 1);
	float32 linearAttenuation = luaM_tofloat(L, -1); lua_pop(L, 1);
	float32 constantAttenuation = luaM_tofloat(L, -1); lua_pop(L, 1);
	float32 radius = luaM_tofloat(L, -1); lua_pop(L, 1);
	Color color = luaM_popcolor(L);

	PointLight* obj = new PointLight(color, radius, constantAttenuation, linearAttenuation, quadricAttenuation);
	luaM_pushobject(L, "PointLight", obj);
		
	return 1;
}