#include "../../memory/memory.h"
#include "point_light.h"
#include "../../scene/scene_node.h"
#include "../../scene/scene_group.h"
using namespace playstate;

PointLight::PointLight(const Color& color, float32 radius, float32 constantAttenuation, float32 linearAttenuation, float32 quadricAttenuation)
	: Component(), LightSource(), 
	mColor(color), mRadius(radius), mConstantAttenuation(constantAttenuation), mLinearAttenuation(linearAttenuation), mQuadricAttenuation(quadricAttenuation)
{
}

PointLight::~PointLight()
{
}

void PointLight::OnComponentAdded()
{
	SetBoundingBox(AABB(GetNode()->GetPosition(), 20.0f, 20.0f, 20.0f));
	LightSource::Attach(GetNode()->GetGroup());
}

void PointLight::OnComponentRemoved()
{
	LightSource::Detach();
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

namespace playstate
{
	class ScriptedPointLight : public PointLight, public Scriptable
	{
	public:
		ScriptedPointLight(const Color& color, float32 radius, float32 constantAttenuation,
			float32 linearAttenuation, float32 quadricAttenuation)
			: PointLight(color, radius, constantAttenuation, linearAttenuation, quadricAttenuation)
		{
		}

		virtual ~ScriptedPointLight()
		{
		}
	};

	int PointLight_Factory(lua_State* L)
	{
		float32 quadricAttenuation = lua_tonumber(L, -1); lua_pop(L, 1);
		float32 linearAttenuation = lua_tonumber(L, -1); lua_pop(L, 1);
		float32 constantAttenuation = lua_tonumber(L, -1); lua_pop(L, 1);
		float32 radius = lua_tonumber(L, -1); lua_pop(L, 1);
		Color color = luaM_popcolor(L);

		ScriptedPointLight* obj = new ScriptedPointLight(color, radius, constantAttenuation, linearAttenuation, quadricAttenuation);
		luaM_pushobject(L, "PointLight", obj);
		
		return 1;
	}
}
