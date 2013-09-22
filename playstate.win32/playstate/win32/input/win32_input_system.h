#pragma once

#include <playstate/input/input_system.h>
#include <vector>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace playstate
{
	namespace win32
	{
		static const DWORD Win32MouseButtons[MouseButtons::NO_BUTTON] = {
			VK_LBUTTON,
			VK_MBUTTON,
			VK_RBUTTON
		};

		class Win32Window;
		class Win32InputSystem : public IInputSystem, public IGamePadState, public IKeyboardState, public IMouseState
		{
		public:
			Win32InputSystem(Win32Window& window);
			virtual ~Win32InputSystem();
		
		// IInputSystem
		public:
			virtual void Poll();
			virtual const IGamePadState& GetGamePadState() const;
			virtual const IKeyboardState& GetKeyboardState() const;
			virtual const IMouseState& GetMouseState() const;

		// IGamePadState
		public:
			virtual bool IsDown(GamePadButtons::Enum button) const;
			virtual bool IsUp(GamePadButtons::Enum button) const;
			virtual Vector2 GetLeftThumbStick() const;
			virtual Vector2 GetRightThumbStick() const;

		// IKeyboardState
		public:
			virtual bool IsDown(KeyboardKeys::Enum key) const;
			virtual bool IsUp(KeyboardKeys::Enum key) const;

		// IMouseState
		public:
			virtual bool IsDown(MouseButtons::Enum button) const;
			virtual bool IsUp(MouseButtons::Enum button) const;
			virtual Point GetPosition() const;

		private:
			Win32Window& mWindow;
			bool mEnabled;
			Point mPosition;
		};
	}
}
