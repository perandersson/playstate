#include "../memory/memory.h"
#include "input_system.h"

namespace playstate
{
	int IInputSystem_Keys_IsKeyDown(lua_State* L)
	{
		int key = (int)lua_tonumber(L, -1); lua_pop(L, 1);
		IKeyboardState& state = IInputSystem::Get().GetKeyboardState();
		lua_pushboolean(L, state.IsDown((KeyboardKeys)key) ? 1 : 0);
		return 1;
	}
	
	int IInputSystem_Keys_IsKeyUp(lua_State* L)
	{
		int key = (int)lua_tonumber(L, -1); lua_pop(L, 1);
		IKeyboardState& state = IInputSystem::Get().GetKeyboardState();
		lua_pushboolean(L, state.IsUp((KeyboardKeys)key) ? 1 : 0);
		return 1;
	}

	int IInputSystem_Mouse_GetPosition(lua_State* L)
	{
		IMouseState& state = IInputSystem::Get().GetMouseState();
		luaM_pushpoint(L, state.GetPosition());
		return 1;
	}
}
