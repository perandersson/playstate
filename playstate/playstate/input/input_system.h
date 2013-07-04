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

	extern int IInputSystem_Keys_IsKeyDown(lua_State* L);
	extern int IInputSystem_Keys_IsKeyUp(lua_State* L);
	//extern int IInputSystem_Keys_GetMouseState(lua_State* L);
	static luaL_Reg IInputSystem_Keys_Methods[] = {
		{ "IsKeyDown", IInputSystem_Keys_IsKeyDown },
		{ "IsKeyUp", IInputSystem_Keys_IsKeyUp },
//		{ "GetMouseState", IInputSystem_GetMouseState },
		{ NULL, NULL }
	};

	extern int IInputSystem_Mouse_GetX(lua_State* L);
	extern int IInputSystem_Mouse_GetY(lua_State* L);
	//extern int IInputSystem_Keys_GetMouseState(lua_State* L);
	static luaL_Reg IInputSystem_Mouse_Methods[] = {
		{ "GetX", IInputSystem_Mouse_GetX },
		{ "GetY", IInputSystem_Mouse_GetY },
//		{ "GetMouseState", IInputSystem_GetMouseState },
		{ NULL, NULL }
	};
}
