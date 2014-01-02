#pragma once
#include <playstate/component/component.h>
#include <playstate/math/point.h>
#include <playstate/math/vector2.h>
#include <playstate/sound/sound_engine.h>
#include <playstate/input/input_system.h>
using namespace playstate;

class MovePlayerComponent : public Component
{
public:
	MovePlayerComponent(float32 speed);
	virtual ~MovePlayerComponent();

// Component
protected:
	virtual void OnComponentAdded();

// Updatable
public:
	virtual void Update();

private:
	void RotateThis();
	void TranslateThis();
	Vector2 GetArcballVector(const Point& mousePos);

private:
	IInputSystem& mInputSystem;
	ISoundEngine& mSoundEngine;
	IWindow& mWindow;
	float32 mSpeed;
	Point mPrevMousePos;
};

//
// Script integration
//

extern int MovePlayerComponent_Factory(lua_State* L);
static luaL_Reg MovePlayerComponent_Methods[] = {
	{ LUA_CONSTRUCTOR, MovePlayerComponent_Factory },
	{ NULL, NULL }
};