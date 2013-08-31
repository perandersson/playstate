#include "../memory/memory.h"
#include "luam.h"
#include "scriptable.h"
#include "../math/vector3.h"
#include "../math/vector2.h"
#include "../math/point.h"
#include "../math/color.h"
#include "../logging/logger.h"
#include "script_system.h"

#ifndef luaM_tofloat
#define luaM_tofloat(L, I) (float32)lua_tonumber(L, I)
#endif

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

		const playstate::string metaName = playstate::string(className) + "_mt";

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

	void luaM_pushresource(lua_State* L, ResourceData* resourceData)
	{
		assert_not_null(L);
		assert_not_null(resourceData);
		ResourceData** container = (ResourceData**)lua_newuserdata(L, sizeof(ResourceData**));
		*container = resourceData;
	}

	ResourceData* luaM_popresource(lua_State* L)
	{
		assert_not_null(L);
		ResourceData* result = NULL;
		if(lua_isuserdata(L, -1)) {
			ResourceData** container = (ResourceData**)lua_touserdata(L, -1);
			result = *container;
		}
		
		lua_pop(L, 1);
		return result;
	}

	Vector3 luaM_popvector3(lua_State* L)
	{
		assert_not_null(L);
		Vector3 vec;
		if(lua_istable(L, -1)) {
			float* ptr = vec.Points;
			lua_pushnil(L);

			lua_next(L, -2);
			*ptr++ = luaM_tofloat(L, -1);
			lua_pop(L, 1);

			lua_next(L, -2);
			*ptr++ = luaM_tofloat(L, -1);
			lua_pop(L, 1);

			bool rest = lua_next(L, -2) != 0;
			*ptr++ = luaM_tofloat(L, -1);
			lua_pop(L, rest ? 2 : 1);

			lua_pop(L, 1);
			return vec;
		} else if(lua_isnumber(L, -1) && lua_isnumber(L, -2) && lua_isnumber(L, -2)) {
			float* ptr = &vec.Z;
			*ptr-- = luaM_tofloat(L, -1); 
			*ptr-- = luaM_tofloat(L, -2); 
			*ptr = luaM_tofloat(L, -3); 
			lua_pop(L, 3);
		}

		return vec;
	}

	Vector2 luaM_popvector2(lua_State* L)
	{
		assert_not_null(L);
		Vector2 vec;
		if(lua_istable(L, -1)) {
			float* ptr = vec.Points;
			lua_pushnil(L);

			lua_next(L, -2);
			*ptr++ = luaM_tofloat(L, -1);
			lua_pop(L, 1);

			bool rest = lua_next(L, -2) != 0;
			*ptr++ = luaM_tofloat(L, -1);
			lua_pop(L, rest ? 2 : 1);

			lua_pop(L, 1);
			return vec;
		} else if(lua_isnumber(L, -1) && lua_isnumber(L, -2)) {
			float* ptr = &vec.Y;
			*ptr-- = luaM_tofloat(L, -1); 
			*ptr = luaM_tofloat(L, -2); 
			lua_pop(L, 2);
		}

		return vec;
	}

	uint32 luaM_pushvector3(lua_State* L, const Vector3& vec)
	{
		assert_not_null(L);

		// X, Y, Z
		lua_pushnumber(L, vec.X);
		lua_pushnumber(L, vec.Y);
		lua_pushnumber(L, vec.Z);

		return 3;
	}

	uint32 luaM_pushvector2(lua_State* L, const Vector2& vec)
	{
		assert_not_null(L);

		// X, Y
		lua_pushnumber(L, vec.X);
		lua_pushnumber(L, vec.Y);

		return 2;
	}
	
	uint32 luaM_pushpoint(lua_State* L, const Point& point)
	{
		assert_not_null(L);

		// X, Y
		lua_pushinteger(L, point.X);
		lua_pushinteger(L, point.Y);

		return 2;
	}
	
	uint32 luaM_pushcolor(lua_State* L, const Color& color)
	{
		assert_not_null(L);

		// Red, Green, Blue, Alpha
		lua_pushnumber(L, color.Red);
		lua_pushnumber(L, color.Green);
		lua_pushnumber(L, color.Blue);
		lua_pushnumber(L, color.Alpha);

		return 4;
	}

	Color luaM_popcolor(lua_State* L)
	{
		assert_not_null(L);
		Color color;
		int numElements = 0;
		if(lua_istable(L, -1)) {
			float* colors = color.Colors;
			lua_pushnil(L);
			while(lua_next(L, -2)) {
				if(numElements > 4) {
					lua_pop(L, 2);
					break;
				}
				*colors++ = luaM_tofloat(L, -1);
				numElements++;
				lua_pop(L, 1);
			}

			lua_pop(L, 1);
		} else if(lua_isnumber(L, -1)) {
			float* colors = color.Colors;
			*colors++ = luaM_tofloat(L, -1); lua_pop(L, 1);
			if(lua_isnumber(L, -1)) {
				*colors++ = luaM_tofloat(L, -1); lua_pop(L, 1);
			}
			if(lua_isnumber(L, -1)) {
				*colors++ = luaM_tofloat(L, -1); lua_pop(L, 1);
			}
			if(lua_isnumber(L, -1)) {
				*colors++ = luaM_tofloat(L, -1); lua_pop(L, 1);
			}
		} else if(lua_isstring(L, -1)) {
			playstate::string hex = lua_tostring(L, -1); lua_pop(L, 1);
			color = Color::HexToRGB(hex.c_str());
		}

		if(numElements == 1) {
			color.Green = color.Blue = color.Alpha = color.Red;
		} else if(numElements == 3) {
			color.Alpha = 1.0f;
		}

		return color;
	}
	
	Point luaM_poppoint(lua_State* L)
	{
		Point point;

		if(lua_istable(L, -1)) {
			lua_pushnil(L);
			if(lua_next(L, -2)) {
				point.X = lua_tointeger(L, -1);
				lua_pop(L, 1);
			}
	
			if(lua_next(L, -2)) {
				point.Y = lua_tointeger(L, -1);
				lua_pop(L, 1);
			}
			lua_pop(L, 1);
		} else if(lua_isnumber(L, -1) && lua_isnumber(L, -2)) {
			point.X = lua_tointeger(L, -2);
			point.Y = lua_tointeger(L, -1);
			lua_pop(L, 2);
		}

		return point;
	}

	void luaM_printerror(lua_State* L, const char* msg)
	{
		lua_Debug ar;
		lua_getstack(L, 1, &ar);
		lua_getinfo(L, "lS", &ar);

		const playstate::string identity = ScriptSystem::Get().GetIdentity();
		ILogger::Get().Error("%s@%d: %s", identity.c_str(), ar.currentline, msg);
	}
}
