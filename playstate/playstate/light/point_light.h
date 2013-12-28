#pragma once

#include "../processor/light_source.h"
#include "../math/color.h"

namespace playstate
{
	class PointLight : public LightSource
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
		virtual ~PointLight();

		const Color& GetColor();
		float32 GetRadius() const;
		float32 GetConstantAttenuation() const;
		float32 GetLinearAttenuation() const;
		float32 GetQuadricAttenuation() const;

		
	// SceneNode
	public:
		virtual void OnAttachedToSceneGroup();
		virtual void OnDetachingFromSceneGroup();

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
