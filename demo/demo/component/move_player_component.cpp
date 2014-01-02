#include <playstate/memory/memory.h>
#include <playstate/scene/scene_group.h>
#include <playstate/window/window.h>
#include <cmath>
#include "move_player_component.h"

MovePlayerComponent::MovePlayerComponent(float32 speed)
	: Component(), 
	mInputSystem(IInputSystem::Get()), mSoundEngine(ISoundEngine::Get()), mWindow(IWindow::Get()), 
	mSpeed(speed)
{
}

MovePlayerComponent::~MovePlayerComponent()
{
}

void MovePlayerComponent::OnComponentAdded()
{
	EnableFeature(Component::Features::UPDATABLE);
}

void MovePlayerComponent::Update()
{
	RotateThis();
	TranslateThis();
}

void MovePlayerComponent::RotateThis()
{
	const Point mousePos = mInputSystem.GetMouseState().GetPosition();
	if(mPrevMousePos == mousePos) {
		return;
	}

	const Vector2 arcball = GetArcballVector(mousePos);
	const float32 angle = atan2f(arcball.X, arcball.Y);
	GetNode()->SetRotation(Vector3(0, angle, 0));
	mPrevMousePos = mousePos;
}

void MovePlayerComponent::TranslateThis()
{
	Vector3 position = GetNode()->GetPosition();
	bool move = false;

	const IKeyboardState& keyboard = mInputSystem.GetKeyboardState();
	if(keyboard.IsDown(KeyboardKeys::KS_W)) {
		position.Z -= mSpeed * GameDeltaTime;
		move = true;
	}
	
	if(keyboard.IsDown(KeyboardKeys::KS_S)) {
		position.Z += mSpeed * GameDeltaTime;
		move = true;
	}
	
	if(keyboard.IsDown(KeyboardKeys::KS_A)) {
		position.X -= mSpeed * GameDeltaTime;
		move = true;
	}
	
	if(keyboard.IsDown(KeyboardKeys::KS_D)) {
		position.X += mSpeed * GameDeltaTime;
		move = true;
	}

	if(move) {
		GetNode()->SetPosition(position);
		mSoundEngine.GetListener()->SetPosition(position);
	}
}

Vector2 MovePlayerComponent::GetArcballVector(const Point& mousePos)
{
	if(mousePos == Point::Zero) {
		return Vector2::Zero;
	}

	const Size& windowSize = mWindow.GetSize();
	Vector2 arcball(
		(mousePos.X / (float32)windowSize.Width * 2) - 1.0f,
		(mousePos.Y / (float32)windowSize.Height * 2) - 1.0f
	);
	arcball.Normalize();
	return arcball;
}

int MovePlayerComponent_Factory(lua_State* L)
{
	float32 speed = luaM_popfloat(L);
	MovePlayerComponent* component = new MovePlayerComponent(speed);
	luaM_pushobject(L, "MovePlayerComponent", component);
	return 1;
}
