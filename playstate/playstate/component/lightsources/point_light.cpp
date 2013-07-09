#include "../../memory/memory.h"
#include "point_light.h"
#include "../../scene/scene_node.h"
#include "../../scene/scene_group.h"
using namespace playstate;

PointLight::PointLight(const Color& color, float radius, float constantAttenuation,
	float linearAttenuation, float quadricAttenuation)
{
}

PointLight::~PointLight()
{
}

void PointLight::OnComponentAdded()
{
	LightSource::Attach(Node->Group);
}

void PointLight::OnComponentRemoved()
{
	LightSource::Detach();
}

namespace playstate
{
	class ScriptedPointLight : public PointLight, public Scriptable
	{
	public:
		ScriptedPointLight(const Color& color, float radius, float constantAttenuation,
			float linearAttenuation, float quadricAttenuation)
			: PointLight(color, radius, constantAttenuation, linearAttenuation, quadricAttenuation)
		{
		}

		virtual ~ScriptedPointLight()
		{
		}
	};

	int PointLight_Factory(lua_State* L)
	{
		float quadricAttenuation = lua_tonumber(L, -1); lua_pop(L, 1);
		float linearAttenuation = lua_tonumber(L, -1); lua_pop(L, 1);
		float constantAttenuation = lua_tonumber(L, -1); lua_pop(L, 1);
		float radius = lua_tonumber(L, -1); lua_pop(L, 1);
		Color color = luaM_popcolor(L);

		ScriptedPointLight* obj = new ScriptedPointLight(color, radius, constantAttenuation, linearAttenuation, quadricAttenuation);
		luaM_pushobject(L, "PointLight", obj);
		
		return 1;
	}
}
