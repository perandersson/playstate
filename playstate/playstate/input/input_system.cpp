#include "../memory/memory.h"
#include "input_system.h"

template<> playstate::IInputSystem* playstate::Singleton<playstate::IInputSystem>::gSingleton = NULL;

namespace playstate
{
	int IInputSystem_Keys_IsKeyDown(lua_State* L)
	{
		int key = (int)lua_tonumber(L, -1); lua_pop(L, 1);
		const IKeyboardState& state = IInputSystem::Get().GetKeyboardState();
		lua_pushboolean(L, state.IsDown((KeyboardKeys::Enum)key) ? 1 : 0);
		return 1;
	}
	
	int IInputSystem_Keys_IsKeyUp(lua_State* L)
	{
		int key = (int)lua_tonumber(L, -1); lua_pop(L, 1);
		const IKeyboardState& state = IInputSystem::Get().GetKeyboardState();
		lua_pushboolean(L, state.IsUp((KeyboardKeys::Enum)key) ? 1 : 0);
		return 1;
	}

	int IInputSystem_Mouse_GetPosition(lua_State* L)
	{
		const IMouseState& state = IInputSystem::Get().GetMouseState();
		luaM_pushpoint(L, state.GetPosition());
		return 2;
	}
}
