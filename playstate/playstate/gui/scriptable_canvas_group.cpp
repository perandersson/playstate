#include "../memory/memory.h"
#include "scriptable_canvas_group.h"
#include "canvas.h"
using namespace playstate;

ScriptableCanvasGroup::ScriptableCanvasGroup()
	: CanvasGroup(), Updatable(), Tickable(),
	mOnProcessCanvas(NULL), mUpdate(NULL), mTick(NULL)
{
}

ScriptableCanvasGroup::~ScriptableCanvasGroup()
{
	delete mOnProcessCanvas;
	delete mUpdate;
	delete mTick;
}

void ScriptableCanvasGroup::OnProcessCanvas(GuiGeometryBuilder* builder)
{
	if(mOnProcessCanvas != NULL)
		mOnProcessCanvas->Invoke();
}

void ScriptableCanvasGroup::OnAddedToCanvas()
{
	if(mUpdate != NULL)
		Updatable::Attach(GetCanvas());

	if(mTick != NULL)
		Tickable::Attach(GetCanvas());
}

void ScriptableCanvasGroup::OnRemovingFromCanvas()
{
	Updatable::Detach();
	Tickable::Detach();
}

void ScriptableCanvasGroup::Update()
{
	if(mUpdate != NULL)
		mUpdate->Invoke();
}

void ScriptableCanvasGroup::Tick()
{
	if(mTick != NULL)
		mTick->Invoke();
}

void ScriptableCanvasGroup::OnRegistered()
{
	mOnProcessCanvas = GetMethod("OnProcessCanvas");
	mUpdate = GetMethod("Update");
	mTick = GetMethod("Tick");
}

int playstate::CanvasGroup_Init(lua_State* L)
{
	ScriptableCanvasGroup* group = new ScriptableCanvasGroup();
	luaM_setinstance(L, group);
	const int ref = luaL_ref(L, LUA_REGISTRYINDEX);
	group->RegisterObject(L, ref);
	return 0;
}

int playstate::CanvasGroup_SetName(lua_State* L)
{
	int numParams = lua_gettop(L);
	if(numParams < 2) {
		luaM_printerror(L, "Expected: self<CanvasGroup>:SetName(name : string)");
		return 0;
	}

	const playstate::string name = lua_tostring(L, -1); lua_pop(L, 1);
	ScriptableCanvasGroup* self = luaM_popobject<ScriptableCanvasGroup>(L);
	if(self != NULL)
		self->SetName(name);
	else
		luaM_printerror(L, "Expected: self<CanvasGroup>:SetName(name : table)");

	return 0;
}

int playstate::CanvasGroup_GetName(lua_State* L)
{
	int numParams = lua_gettop(L);
	if(numParams < 1) {
		luaM_printerror(L, "Expected: self<CanvasGroup>:GetName()");
		lua_pushstring(L, "");
		return 1;
	}
	
	ScriptableCanvasGroup* self = luaM_popobject<ScriptableCanvasGroup>(L);
	if(self != NULL) {
		const playstate::string& name = self->GetName();
		lua_pushstring(L, name.c_str());
	} else {
		luaM_printerror(L, "Expected: self<CanvasGroup>:GetName()");
		lua_pushstring(L, "");
	}

	return 1;
}

int playstate::CanvasGroup_SetStyle(lua_State* L)
{
	if(!lua_istable(L, -1)) {
		luaM_printerror(L, "Expected: self<CanvasGroup>:SetStyle(style : table)");
		return 0;
	}

	int configRef = luaL_ref(L, LUA_REGISTRYINDEX);
	GuiStyle style(L, configRef);
	ScriptableCanvasGroup* self = luaM_popobject<ScriptableCanvasGroup>(L);
	if(self != NULL)
		self->SetStyle(style);
	else
		luaM_printerror(L, "Expected: self<CanvasGroup>:SetStyle(style : table)");

	return 0;
}

int playstate::CanvasGroup_BeginFrame(lua_State* L)
{
	int numParams = lua_gettop(L);
	if(numParams < 2) {
		luaM_printerror(L, "Expected: self<CanvasGroup>:BeginFrame(rect : Rect, [title : string])");
		return 0;
	}

	playstate::string title;
	if(numParams >= 3 && lua_isstring(L, -1)) {
		title = lua_tostring(L, -1); lua_pop(L, 1);
	}

	const Rect rect = luaM_poprect(L);
	ScriptableCanvasGroup* group = luaM_popobject<ScriptableCanvasGroup>(L);
	if(group != NULL)
		group->BeginFrame(rect, title);
	else
		luaM_printerror(L, "Expected: self<CanvasGroup>:BeginFrame(rect : Rect, [title : string])");

	return 0;
}

int playstate::CanvasGroup_EndFrame(lua_State* L)
{
	if(lua_gettop(L) < 1) {
		luaM_printerror(L, "Expected: self<CanvasGroup>:EndFrame()");
		return 0;
	}

	ScriptableCanvasGroup* group = luaM_popobject<ScriptableCanvasGroup>(L);
	if(group != NULL) {
		group->EndFrame();
	} else {
		luaM_printerror(L, "Expected: self<CanvasGroup>:EndFrame()");
	}

	return 0;
}

int playstate::CanvasGroup_Button(lua_State* L)
{
	int numParams = lua_gettop(L);
	if(numParams < 2) {
		luaM_printerror(L, "Expected: self<CanvasGroup>:Button(rect : Rect, [text : string])");
		lua_pushboolean(L, 0);
		return 1;
	}

	playstate::string text;
	if(numParams >= 3 && lua_isstring(L, -1)) {
		text = lua_tostring(L, -1); lua_pop(L, 1);
	}
	
	const Rect rect = luaM_poprect(L);
	ScriptableCanvasGroup* group = luaM_popobject<ScriptableCanvasGroup>(L);
	if(group != NULL) {
		bool result = group->Button(rect, text);
		lua_pushboolean(L, result ? 1 : 0);
	} else {
		luaM_printerror(L, "Expected: self<CanvasGroup>:Button(rect : Rect, [text : string])");
		lua_pushboolean(L, 0);
	}

	return 1;
}

int playstate::CanvasGroup_Label(lua_State* L)
{
	int numParams = lua_gettop(L);
	if(numParams < 3) {
		luaM_printerror(L, "Expected: self<CanvasGroup>:Label(rect : Rect, text : string)");
		return 0;
	}

	playstate::string text = lua_tostring(L, -1); lua_pop(L, 1);
	const Rect rect = luaM_poprect(L);

	ScriptableCanvasGroup* group = luaM_popobject<ScriptableCanvasGroup>(L);
	if(group != NULL) {
		group->Label(rect, text);
	} else {
		luaM_printerror(L, "Expected: self<CanvasGroup>:Label(rect : Rect, text : string)");
		lua_pushboolean(L, 0);
	}

	return 0;
}

int playstate::CanvasGroup_Checkbox(lua_State* L)
{
	int numParams = lua_gettop(L);
	if(numParams < 3) {
		luaM_printerror(L, "Expected: self<CanvasGroup>:Checkbox(rect : Rect, toggled : bool, [text : string])");
		lua_pushboolean(L, 0);
		return 1;
	}
	
	playstate::string text;
	if(numParams >= 4 && lua_isstring(L, -1)) {
		text = lua_tostring(L, -1); lua_pop(L, 1);
	}

	const bool toggled = lua_toboolean(L, -1) == 1; lua_pop(L, 1);
	const Rect rect = luaM_poprect(L);
	
	ScriptableCanvasGroup* group = luaM_popobject<ScriptableCanvasGroup>(L);
	if(group != NULL) {
		bool result = group->Checkbox(rect, toggled, text);
		lua_pushboolean(L, result ? 1 : 0);
	} else {
		luaM_printerror(L, "Expected: self<CanvasGroup>:Checkbox(rect : Rect, toggled : bool, [text : string])");
		lua_pushboolean(L, 0);
	}

	return 1;
}

int playstate::CanvasGroup_Slider(lua_State* L)
{
	int numParams = lua_gettop(L);
	if(numParams < 6) {
		luaM_printerror(L, "Expected: self<CanvasGroup>:Slider(rect : Rect, value : number, leftValue : number, rightValue : number, stepValue : number)");
		lua_pushnumber(L, 0.0f);
		return 1;
	}

	const float32 stepValue = luaM_tofloat(L, -1); 
	const float32 rightValue = luaM_tofloat(L, -2);
	const float32 leftValue = luaM_tofloat(L, -3);
	const float32 value = luaM_tofloat(L, -4);
	lua_pop(L, 4);
	
	const Rect rect = luaM_poprect(L);
	ScriptableCanvasGroup* group = luaM_popobject<ScriptableCanvasGroup>(L);
	if(group != NULL) {
		const float32 newValue = group->Slider(rect, value, leftValue, rightValue, stepValue);
		lua_pushnumber(L, newValue);
	} else {
		luaM_printerror(L, "Expected: self<CanvasGroup>:Slider(rect : Rect, value : number, leftValue : number, rightValue : number, stepValue : number)");
		lua_pushnumber(L, 0.0f);
	}

	return 1;
}

int playstate::CanvasGroup_ComboBox(lua_State* L)
{
	int numParams = lua_gettop(L);
	if(numParams < 4) {
		luaM_printerror(L, "Expected: self<CanvasGroup>:ComboBox(rect : Rect, selectedIndex : integer, values : table)");
		lua_pushinteger(L, 0);
		return 1;
	}

	// Load the tabel later
	lua_pop(L, 1);
	const uint32 selectedIndex = lua_tointeger(L, -1); lua_pop(L, 1);
	const Rect rect = luaM_poprect(L);

	ScriptableCanvasGroup* group = luaM_popobject<ScriptableCanvasGroup>(L);
	if(group != NULL) {
		const uint32 newValue = group->ComboBox(rect, selectedIndex);
		lua_pushinteger(L, newValue);
	} else {
		luaM_printerror(L, "Expected: self<CanvasGroup>:ComboBox(rect : Rect, selectedIndex : integer, values : table)");
		lua_pushinteger(L, 0);
	}
	return 1;
}
