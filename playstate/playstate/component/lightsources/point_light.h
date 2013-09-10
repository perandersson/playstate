#pragma once

#include "../component.h"
#include "../../processor/light_source.h"
#include "../../math/color.h"
#include "../../script/luam.h"

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
		PointLight(const Color& color, float32 radius, float32 constantAttenuation,
			float32 linearAttenuation, float32 quadricAttenuation);

		//
		// Destructor
		virtual ~PointLight();

		const Color& GetColor();
		float32 GetRadius() const;
		float32 GetConstantAttenuation() const;
		float32 GetLinearAttenuation() const;
		float32 GetQuadricAttenuation() const;

	// Component
	public:
		virtual void OnComponentAdded();
		virtual void OnComponentRemoved();

	private:
		Color mColor;
		float32 mRadius;
		float32 mConstantAttenuation;
		float32 mLinearAttenuation;
		float32 mQuadricAttenuation;
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
