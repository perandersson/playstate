#pragma once

#include "../singleton.h"
#include "input_state.h"
#include "../script/script_system.h"

namespace playstate
{
	class IInputSystem
	{
	public:
		virtual ~IInputSystem() {}

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

	extern int IInputSystem_Keys_IsKeyDown(lua_State* L);
	extern int IInputSystem_Keys_IsKeyUp(lua_State* L);
	//extern int IInputSystem_Keys_GetMouseState(lua_State* L);
	static luaL_Reg IInputSystem_Keys_Methods[] = {
		{ "IsKeyDown", IInputSystem_Keys_IsKeyDown },
		{ "IsKeyUp", IInputSystem_Keys_IsKeyUp },
//		{ "GetMouseState", IInputSystem_GetMouseState },
		{ NULL, NULL }
	};

	extern int IInputSystem_Mouse_GetPosition(lua_State* L);
	//extern int IInputSystem_Keys_GetMouseState(lua_State* L);
	static luaL_Reg IInputSystem_Mouse_Methods[] = {
		{ "GetPosition", IInputSystem_Mouse_GetPosition },
//		{ "GetMouseState", IInputSystem_GetMouseState },
		{ NULL, NULL }
	};
}
