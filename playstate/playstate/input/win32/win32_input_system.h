#pragma once

#include "../input_system.h"
#include "win32_input_state.h"

#include <vector>

namespace playstate
{
	class Win32Window;
	class Win32InputSystem : public IInputSystem
	{
	public:
		Win32InputSystem(Win32Window& window);
		virtual ~Win32InputSystem();
		
	// IInputSystem
	public:
		virtual void Poll();
		virtual IGamePadState& GetGamePadState();
		virtual IKeyboardState& GetKeyboardState();
		virtual IMouseState& GetMouseState();

	private:
		Win32Window& mWindow;

		Win32GamePadState mGamePadState;
		Win32KeyboardState mKeyboardState;
		Win32MouseState mMouseState;
	};
}
