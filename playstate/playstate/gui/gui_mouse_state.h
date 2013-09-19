#pragma once

namespace playstate
{
	class GuiMouseState
	{
	public:
		enum Enum {
			OUTSIDE = 0,
			HOVER,
			DOWN,

			NUM_STATES
		};
	};

}
