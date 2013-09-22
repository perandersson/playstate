#include <playstate/memory/memory.h>
#include "win32_input_system.h"
#include "../window/win32_window.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace playstate;
using namespace playstate::win32;

Win32InputSystem::Win32InputSystem(Win32Window& window) : mWindow(window), mEnabled(false)
{
}

Win32InputSystem::~Win32InputSystem()
{
}

void Win32InputSystem::Poll()
{	
	if(!mWindow.HasFocus()) {
		mEnabled = false;
		return;
	}
	mEnabled = true;

	POINT p;
	if(GetCursorPos(&p)) {
		if(ScreenToClient(mWindow.GetWindowHandle(), &p)) {
			Point cursorPoint((int32)p.x, (int32)p.y);
			if(cursorPoint >= Point::Zero && cursorPoint <= mWindow.GetSize()) {
				mPosition = Point(p.x, p.y);
			} else {
				mEnabled = false;
			}
		}
	}
}

const IGamePadState& Win32InputSystem::GetGamePadState() const
{
	return *this;
}

const IKeyboardState& Win32InputSystem::GetKeyboardState() const
{
	return *this;
}

const IMouseState& Win32InputSystem::GetMouseState() const
{
	return *this;
}

bool Win32InputSystem::IsDown(GamePadButtons::Enum button) const
{
	return false;
}

bool Win32InputSystem::IsUp(GamePadButtons::Enum button) const
{
	return true;
}

Vector2 Win32InputSystem::GetLeftThumbStick() const
{
	return Vector2();
}

Vector2 Win32InputSystem::GetRightThumbStick() const
{
	return Vector2();
}

bool Win32InputSystem::IsDown(KeyboardKeys::Enum key) const
{
	if(!mEnabled)
		return false;

	bool res = (GetKeyState((int)key) & 0x80) != 0;
	return res;
}

bool Win32InputSystem::IsUp(KeyboardKeys::Enum key) const
{
	if(!mEnabled)
		return true;

	return (GetKeyState((int)key) & 0x80) == 0;
}

bool Win32InputSystem::IsDown(MouseButtons::Enum button) const
{
	if(button == MouseButtons::NO_BUTTON)
		return false;

	return (GetKeyState(Win32MouseButtons[button]) & 0x80) != 0;
}

bool Win32InputSystem::IsUp(MouseButtons::Enum button) const
{
	if(button == MouseButtons::NO_BUTTON)
		return false;

	return (GetKeyState(Win32MouseButtons[button]) & 0x80) == 0;
}

Point Win32InputSystem::GetPosition() const
{
	return mPosition;
}
