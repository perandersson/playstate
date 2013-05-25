#pragma once

#include "../math/vector2.h"

namespace playstate
{
	class IInputState
	{
	public:
		virtual ~IInputState() {}
	};

	enum KeyboardKeys 
	{
		KS_SPACE = 0x20,
		KS_W = (int)('W'),
		KS_S = (int)('S'),
		KS_A = (int)('A'),
		KS_D = (int)('D')
	};

	enum MouseKeys
	{
		MK_LEFT,
		MK_MIDDLE,
		MK_RIGHT,
	};

	class IKeyboardState : public IInputState
	{
	public:
		virtual bool IsDown(KeyboardKeys key) = 0;
		virtual bool IsUp(KeyboardKeys key) = 0;
	};

	class IGamePadState : public IInputState
	{
	public:
		virtual bool IsDown(int key) = 0;
		virtual bool IsUp(int key) = 0;
		virtual Vector2 GetLeftThumbStick() = 0;
		virtual Vector2 GetRightThumbStick() = 0;
	};

	class IMouseState : public IInputState
	{
	public:
		virtual bool IsDown(MouseKeys key) = 0;
		virtual bool IsUp(MouseKeys key) = 0;

		virtual int GetX() = 0;
		virtual int GetY() = 0;
	};
}
