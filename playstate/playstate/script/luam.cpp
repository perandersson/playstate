#include "../memory/memory.h"
#include "luam.h"
#include "scriptable.h"
#include "../math/vector3.h"
#include "../math/vector2.h"
#include "../math/point.h"
#include "../math/color.h"

namespace playstate
{
	const uint32 LuaInstanceID = 1;

	void luaM_setinstance(lua_State* L, Scriptable* data)
	{
		assert_not_null(L);

		if(lua_istable(L, -1) == 0) {
			return;
		}

		lua_pushnumber(L, LuaInstanceID);
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
		lua_pushnumber(L, LuaInstanceID);
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

		// X, Y, Z
		lua_pushnumber(L, vec.X);
		lua_pushnumber(L, vec.Y);
		lua_pushnumber(L, vec.Z);
	}

	void luaM_pushvector2(lua_State* L, const Vector2& vec)
	{
		assert_not_null(L);

		// X, Y
		lua_pushnumber(L, vec.X);
		lua_pushnumber(L, vec.Y);
	}
	
	void luaM_pushpoint(lua_State* L, const Point& point)
	{
		assert_not_null(L);

		// X, Y
		lua_pushinteger(L, point.X);
		lua_pushinteger(L, point.Y);
	}
	
	void luaM_pushcolor(lua_State* L, const Color& color)
	{
		assert_not_null(L);

		// Red, Green, Blue, Alpha
		lua_pushnumber(L, color.Red);
		lua_pushnumber(L, color.Green);
		lua_pushnumber(L, color.Blue);
		lua_pushnumber(L, color.Alpha);
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

			if(numElements == 1) {
				color.Green = color.Blue = color.Alpha = color.Red;
			} else if(numElements == 3) {
				color.Alpha = 1.0f;
			}
			
			lua_pop(L, 1);
			return color;
		}	

		return Color::Nothing;
	}
}
