#pragma once

#include "../math/vector2.h"
#include "../math/point.h"

namespace playstate
{
	class IInputState
	{
	public:
		virtual ~IInputState() {}
	};

	class KeyboardKeys
	{
	public:
		enum Enum {
			KS_SPACE = 0x20,
			KS_A = (int)('A'),
			KS_B = (int)('B'),
			KS_C = (int)('C'),
			KS_D = (int)('D'),

			KS_S = (int)('S'),
			KS_T = (int)('T'),

			KS_W = (int)('W')
		};
	};

	class MouseButtons
	{
	public:
		enum Enum {
			MK_LEFT,
			MK_MIDDLE,
			MK_RIGHT,
		};
	};
	
	class IKeyboardState : public IInputState
	{
	public:
		virtual bool IsDown(KeyboardKeys::Enum key) = 0;
		virtual bool IsUp(KeyboardKeys::Enum key) = 0;
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
		virtual bool IsDown(MouseButtons::Enum button) = 0;
		virtual bool IsUp(MouseButtons::Enum button) = 0;
		virtual Point GetPosition() = 0;
	};
}
