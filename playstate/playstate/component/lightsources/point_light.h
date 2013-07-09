#pragma once

#include "../component.h"
#include "../../processor/light_source.h"
#include "../../math/color.h"

namespace playstate
{
	class PointLight : public Component, public LightSource
	{
	public:
		//
		// Constructor
		// @param color
		// @param radius
		// @param constantAttenuation
		// @param linearAttenuation
		// @param quadricAttenuation
		PointLight(const Color& color, float radius, float constantAttenuation,
			float linearAttenuation, float quadricAttenuation);

		//
		// Destructor
		virtual ~PointLight();

	// Component
	public:
		virtual void OnComponentAdded();
		virtual void OnComponentRemoved();

	private:
		Color mColor;
		float mRadius;
		float mConstantAttenuation;
		float mLinearAttenuation;
		float mQuadricAttenuation;
	};

	//
	// Script integration
	//

	extern int PointLight_Factory(lua_State* L);
	static luaL_Reg PointLight_Methods[] = {
		{ LUA_CONSTRUCTOR, PointLight_Factory},
		{ NULL, NULL }
	};
}
