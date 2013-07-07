#include "../memory/memory.h"
#include "luam.h"
#include "scriptable.h"
#include "../math/vector3.h"
#include "../math/vector2.h"
#include "../math/point.h"
#include "../math/color.h"

namespace playstate
{
	void luaM_setinstance(lua_State* L, Scriptable* data)
	{
		assert_not_null(L);

		if(lua_istable(L, -1) == 0) {
			return;
		}

		lua_pushstring(L, "_instance");
		if(data == NULL)
			lua_pushnil(L);
		else
			lua_pushlightuserdata(L, data);
		lua_settable(L, -3);
	}

	Scriptable* luaM_getinstance(lua_State *L)
	{
		assert_not_null(L);

		if(lua_istable(L, -1) == 0) {
			return NULL;
		}
		lua_pushstring(L, "_instance");
		lua_gettable(L, -2);
		if(lua_isuserdata(L, -1) == 0) {
			lua_pop(L, 1);
			return NULL;
		}

		void* userdata = lua_touserdata(L, -1);
		lua_pop(L, 1);
		return (Scriptable*)userdata;
	}

	void luaM_createclass(lua_State* L, const char* className)
	{
		assert_not_null(L);
		assert_not_null(className);

		const std::string metaName = std::string("playstate.") + className;

		lua_newtable(L);

		lua_pushvalue(L, -1);
		lua_setfield(L, -2, "__index");

		luaL_getmetatable(L, metaName.c_str());
		lua_setmetatable(L, -2);
	}

	void luaM_pushobject(lua_State* L, const char* className, Scriptable* object)
	{
		assert_not_null(L);
		assert_not_null(className);
		assert_not_null(object);

		if(object->IsScriptBound()) {
			lua_rawgeti(L, LUA_REGISTRYINDEX, object->GetID());
		} else {
			luaM_createclass(L, className);
			luaM_setinstance(L, object);
			const uint32 ref = luaL_ref(L, LUA_REGISTRYINDEX);
			object->RegisterObject(L, ref);
			lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
		}
	}

	Vector3 luaM_popvector3(lua_State* L)
	{
		assert_not_null(L);
		if(lua_istable(L, -1)) {
			Vector3 vec;
			float* ptr = vec.Points;
			lua_pushnil(L);

			lua_next(L, -2);
			*ptr++ = lua_tonumber(L, -1);
			lua_pop(L, 1);

			lua_next(L, -2);
			*ptr++ = lua_tonumber(L, -1);
			lua_pop(L, 1);

			bool rest = lua_next(L, -2) != 0;
			*ptr++ = lua_tonumber(L, -1);
			lua_pop(L, rest ? 2 : 1);

			lua_pop(L, 1);
			return vec;
		}	

		return Vector3::Zero;
	}

	void luaM_pushvector3(lua_State* L, const Vector3& vec)
	{
		assert_not_null(L);

		// { vec.X, vec.Y, vec.Z }
		// @see http://www.lua.org/pil/11.1.html 
		// ", it is customary in Lua to start arrays with index 1"
		lua_newtable(L);
		
		lua_pushnumber(L, vec.X);
		lua_rawseti(L, -2, 1);
		
		lua_pushnumber(L, vec.Y);
		lua_rawseti(L, -2, 2);
		
		lua_pushnumber(L, vec.Z);
		lua_rawseti(L, -2, 3);
	}

	void luaM_pushvector2(lua_State* L, const Vector2& vec)
	{
		assert_not_null(L);

		// { vec.X, vec.Y }
		lua_newtable(L);
		
		lua_pushnumber(L, vec.X);
		lua_rawseti(L, -2, 1);
		
		lua_pushnumber(L, vec.Y);
		lua_rawseti(L, -2, 2);
	}
	
	void luaM_pushpoint(lua_State* L, const Point& point)
	{
		assert_not_null(L);

		// { point.X, point.Y }
		lua_newtable(L);
		
		lua_pushinteger(L, point.X);
		lua_rawseti(L, -2, 1);
		
		lua_pushinteger(L, point.Y);
		lua_rawseti(L, -2, 2);
	}
	
	void luaM_pushcolor(lua_State* L, const Color& color)
	{
		assert_not_null(L);

		// { color.Red, color.Green, color.Blue, color.Alpha }
		lua_newtable(L);
		
		lua_pushnumber(L, color.Red);
		lua_rawseti(L, -2, 1);
		
		lua_pushnumber(L, color.Green);
		lua_rawseti(L, -2, 2);
		
		lua_pushnumber(L, color.Blue);
		lua_rawseti(L, -2, 3);

		lua_pushnumber(L, color.Alpha);
		lua_rawseti(L, -2, 4);
	}

	Color luaM_popcolor(lua_State* L)
	{
		assert_not_null(L);
		if(lua_istable(L, -1)) {
			Color color;
			float* colors = color.Colors;
			int numElements = 0;
			lua_pushnil(L);
			while(lua_next(L, -2)) {
				if(numElements > 4) {
					lua_pop(L, 2);
					break;
				}
				*colors++ = lua_tonumber(L, -1);
				numElements++;
				lua_pop(L, 1);
			}
			
			lua_pop(L, 1);
			return color;
		}	

		return Color::Nothing;
	}
}
