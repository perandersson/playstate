#pragma once

#include <playstate/input/input_state.h>

namespace playstate
{
	namespace win32
	{
		class Win32GamePadState : public IGamePadState
		{
		public:
			Win32GamePadState();
			virtual ~Win32GamePadState();

			virtual bool IsDown(int key);
			virtual bool IsUp(int key);
			virtual Vector2 GetLeftThumbStick();
			virtual Vector2 GetRightThumbStick();
		};

		class Win32KeyboardState : public IKeyboardState
		{
		public:
			Win32KeyboardState();
			virtual ~Win32KeyboardState();

			virtual bool IsDown(KeyboardKeys key);
			virtual bool IsUp(KeyboardKeys key);
		};

		class Win32MouseState : public IMouseState
		{
		public:
			Win32MouseState();
			virtual ~Win32MouseState();

			virtual bool IsDown(MouseKeys key);
			virtual bool IsUp(MouseKeys key);
			virtual Point GetPosition();

			void SetPosition(const Point& pt);

		private:
			Point mPoint;
		};
	}
}