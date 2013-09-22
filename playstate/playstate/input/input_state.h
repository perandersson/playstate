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

	class GamePadButtons
	{
	public:
		enum Enum {
			X,
			Y,
			A,
			B,
			START,
			SELECT,
			LTRIGGER,
			RTRIGGER,
			LBUMPER,
			RBUMPER
		};
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
			LEFT = 0,
			MIDDLE,
			RIGHT,

			NO_BUTTON
		};
	};
	
	class IKeyboardState : public IInputState
	{
	public:
		virtual bool IsDown(KeyboardKeys::Enum key) const = 0;
		virtual bool IsUp(KeyboardKeys::Enum key) const = 0;
	};

	class IGamePadState : public IInputState
	{
	public:
		virtual bool IsDown(GamePadButtons::Enum button) const = 0;
		virtual bool IsUp(GamePadButtons::Enum button) const = 0;
		virtual Vector2 GetLeftThumbStick() const = 0;
		virtual Vector2 GetRightThumbStick() const = 0;
	};

	class IMouseState : public IInputState
	{
	public:
		virtual bool IsDown(MouseButtons::Enum button) const = 0;
		virtual bool IsUp(MouseButtons::Enum button) const = 0;
		virtual Point GetPosition() const = 0;
	};
}
