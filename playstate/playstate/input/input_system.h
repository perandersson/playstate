#pragma once

#include "../singleton.h"
#include "input_state.h"
#include "../script/script_system.h"

namespace playstate
{
	class IInputSystem : public Singleton<IInputSystem>
	{
	public:
		//
		// Poll the input system for events
		virtual void Poll() = 0;
		virtual IGamePadState& GetGamePadState() = 0;
		virtual IKeyboardState& GetKeyboardState() = 0;
		virtual IMouseState& GetMouseState() = 0;
	};
	
	//
	// Script integration
	//

	extern int IInputSystem_IsKeyDown(lua_State* L);
	extern int IInputSystem_GetMouseState(lua_State* L);
	static luaL_Reg IInputSystem_Methods[] = {
		{ "IsKeyDown", IInputSystem_IsKeyDown },
		{ "GetMouseState", IInputSystem_GetMouseState },
		{ NULL, NULL }
	};
}
