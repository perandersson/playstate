#include <playstate/memory/memory.h>
#include "win32_input_system.h"
#include "../window/win32_window.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace playstate;
using namespace playstate::win32;

Win32InputSystem::Win32InputSystem(Win32Window& window) : mWindow(window)
{
}

Win32InputSystem::~Win32InputSystem()
{
}

void Win32InputSystem::Poll()
{	
	POINT p;
	if(GetCursorPos(&p)) {
		if(ScreenToClient(mWindow.GetWindowHandle(), &p)) {
			Point cursorPoint((int32)p.x, (int32)p.y);
			if(cursorPoint >= Point::Zero && cursorPoint <= mWindow.GetSize()) {
				mMouseState.SetPosition(Point(p.x, p.y));
			}
		}
	}
}


IGamePadState& Win32InputSystem::GetGamePadState()
{
	return mGamePadState;
}

IKeyboardState& Win32InputSystem::GetKeyboardState()
{
	return mKeyboardState;
}

IMouseState& Win32InputSystem::GetMouseState()
{
	return mMouseState;
}
