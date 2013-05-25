#include "../../memory/memory.h"
#include "win32_input_state.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace playstate;

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

bool Win32KeyboardState::IsDown(KeyboardKeys key)
{
	bool res = (GetKeyState((int)key) & 0x80) != 0;
	return res;
}

bool Win32KeyboardState::IsUp(KeyboardKeys key)
{
	return (GetKeyState((int)key) & 0x80) == 0;
}


Win32MouseState::Win32MouseState() : mX(0), mY(0)
{
}

Win32MouseState::~Win32MouseState()
{
}

bool Win32MouseState::IsDown(MouseKeys key)
{
	switch(key)
	{
	case MK_LEFT:
		return (GetKeyState(VK_LBUTTON) & 0x80) != 0;
	case MK_MIDDLE:
		return (GetKeyState(VK_MBUTTON) & 0x80) != 0;
	case MK_RIGHT:
		return (GetKeyState(VK_RBUTTON) & 0x80) != 0;
	default:
		return false;
	}
}

bool Win32MouseState::IsUp(MouseKeys key)
{
	switch(key)
	{
	case MK_LEFT:
		return (GetKeyState(VK_LBUTTON) & 0x80) == 0;
	case MK_MIDDLE:
		return (GetKeyState(VK_MBUTTON) & 0x80) == 0;
	case MK_RIGHT:
		return (GetKeyState(VK_RBUTTON) & 0x80) == 0;
	default:
		return true;
	}
}

int Win32MouseState::GetX()
{
	return mX;
}

int Win32MouseState::GetY()
{
	return mY;
}

void Win32MouseState::SetX(int x)
{
	mX = x;
}

void Win32MouseState::SetY(int y)
{
	mY = y;
}

