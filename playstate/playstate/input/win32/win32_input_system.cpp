#include "../../memory/memory.h"
#include "win32_input_system.h"
#include "../../window/win32/win32_window.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace playstate;

template<> playstate::IInputSystem* playstate::Singleton<playstate::IInputSystem>::gSingleton = NULL;

Win32InputSystem::Win32InputSystem(Win32Window& window) : mWindow(mWindow)
{
}

Win32InputSystem::~Win32InputSystem()
{
}

void Win32InputSystem::Poll()
{	
	POINT p;
	if(GetCursorPos(&p)) {
		if (ScreenToClient(mWindow.GetWindowHandle(), &p)) {
			mMouseState.SetX(p.x);
			mMouseState.SetY(p.y);
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
