#include <playstate/memory/memory.h>
#include "win32_input_state.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace playstate;
using namespace win32;

Win32GamePadState::Win32GamePadState()
{
}

Win32GamePadState::~Win32GamePadState()
{
}

bool Win32GamePadState::IsDown(int button)
{
	return false;
}

bool Win32GamePadState::IsUp(int button)
{
	return true;
}

Vector2 Win32GamePadState::GetLeftThumbStick()
{
	return Vector2();
}

Vector2 Win32GamePadState::GetRightThumbStick()
{
	return Vector2();
}

Win32KeyboardState::Win32KeyboardState()
{
}

Win32KeyboardState::~Win32KeyboardState()
{
}

bool Win32KeyboardState::IsDown(KeyboardKeys::Enum key)
{
	bool res = (GetKeyState((int)key) & 0x80) != 0;
	return res;
}

bool Win32KeyboardState::IsUp(KeyboardKeys::Enum key)
{
	return (GetKeyState((int)key) & 0x80) == 0;
}


Win32MouseState::Win32MouseState()
{
}

Win32MouseState::~Win32MouseState()
{
}

bool Win32MouseState::IsDown(MouseButtons::Enum button)
{
	if(button == MouseButtons::NO_BUTTON)
		return false;

	return (GetKeyState(Win32MouseButtons[button]) & 0x80) != 0;
}

bool Win32MouseState::IsUp(MouseButtons::Enum button)
{
	if(button == MouseButtons::NO_BUTTON)
		return false;

	return (GetKeyState(Win32MouseButtons[button]) & 0x80) == 0;
}

Point Win32MouseState::GetPosition()
{
	return mPoint;
}

void Win32MouseState::SetPosition(const Point& pt)
{
	mPoint = pt;
}


