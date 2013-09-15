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

int playstate::CanvasGroup_SetStyle(lua_State* L)
{
	if(!lua_istable(L, -1)) {
		luaM_printerror(L, "Expected: self<CanvasGroup>:SetStyle(table)");
		return 0;
	}

	int configRef = luaL_ref(L, LUA_REGISTRYINDEX);
	GuiStyle style(L, configRef);
	ScriptableCanvasGroup* self = luaM_popobject<ScriptableCanvasGroup>(L);
	if(self != NULL) {
		self->SetStyle(style);
	} else {
		luaM_printerror(L, "Expected: self<CanvasGroup>:SetStyle(table)");
		return 0;
	}

	return 0;
}

int playstate::CanvasGroup_BeginFrame(lua_State* L)
{
	int numParams = lua_gettop(L);
	if(numParams < 3) {
		luaM_printerror(L, "Expected: self<CanvasGroup>:BeginFrame(Size, Position, [Title])");
		return 0;
	}

	playstate::string title;
	if(numParams >= 4 && lua_isstring(L, -1))
		title = lua_tostring(L, -1); lua_pop(L, 1);

	const Vector2 position = luaM_popvector2(L);
	const Size size = luaM_poppoint(L);

	ScriptableCanvasGroup* group = luaM_popobject<ScriptableCanvasGroup>(L);
	if(group != NULL) {
		group->BeginFrame(size, position, title);
	} else {
		luaM_printerror(L, "Expected: self<CanvasGroup>:BeginFrame(Size, Position, [Title])");
	}

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
	if(numParams < 3) {
		luaM_printerror(L, "Expected: self<CanvasGroup>:Button(Size, Position, [Text])");
		lua_pushboolean(L, 0);
		return 1;
	}

	playstate::string text;
	if(numParams >= 4 && lua_isstring(L, -1))
		text = lua_tostring(L, -1); lua_pop(L, 1);

	const Vector2 position = luaM_popvector2(L);
	const Size size = luaM_poppoint(L);

	ScriptableCanvasGroup* group = luaM_popobject<ScriptableCanvasGroup>(L);
	if(group != NULL) {
		bool result = group->Button(size, position, text);
		lua_pushboolean(L, result ? 1 : 0);
	} else {
		luaM_printerror(L, "Expected: self<CanvasGroup>:Button(Size, Position, [Text])");
		lua_pushboolean(L, 0);
	}

	return 1;
}

int playstate::CanvasGroup_Toggle(lua_State* L)
{
	int numParams = lua_gettop(L);
	if(numParams < 4) {
		luaM_printerror(L, "Expected: self<CanvasGroup>:Toggle(Size, Position, Toggled, [Text])");
		lua_pushboolean(L, 0);
		return 1;
	}
	
	playstate::string text;
	if(numParams >= 5 && lua_isstring(L, -1))
		text = lua_tostring(L, -1); lua_pop(L, 1);

	const bool toggled = lua_toboolean(L, -1) == 1; lua_pop(L, 1);
	const Vector2 position = luaM_popvector2(L);
	const Size size = luaM_poppoint(L);
	
	ScriptableCanvasGroup* group = luaM_popobject<ScriptableCanvasGroup>(L);
	if(group != NULL) {
		bool result = group->Toggle(size, position, toggled, text);
		lua_pushboolean(L, result ? 1 : 0);
	} else {
		luaM_printerror(L, "Expected: self<CanvasGroup>:Toggle(Size, Position, Toggled, [Text])");
		lua_pushboolean(L, 0);
	}

	return 1;
}

int playstate::CanvasGroup_Slider(lua_State* L)
{
	int numParams = lua_gettop(L);
	if(numParams < 7) {
		luaM_printerror(L, "Expected: self<CanvasGroup>:Slider(Size, Position, Value, LeftValue, RightValue, StepValue)");
		lua_pushnumber(L, 0.0f);
		return 1;
	}

	const float32 stepValue = lua_tonumber(L, -1); 
	const float32 rightValue = lua_tonumber(L, -2);
	const float32 leftValue = lua_tonumber(L, -3);
	const float32 value = lua_tonumber(L, -4);
	lua_pop(L, 4);

	const Vector2 position = luaM_popvector2(L);
	const Size size = luaM_poppoint(L);
	ScriptableCanvasGroup* group = luaM_popobject<ScriptableCanvasGroup>(L);
	if(group != NULL) {
		const float32 newValue = group->Slider(size, position, value, leftValue, rightValue, stepValue);
		lua_pushnumber(L, newValue);
	} else {
		luaM_printerror(L, "Expected: self<CanvasGroup>:Slider(Size, Position, Value, LeftValue, RightValue, StepValue)");
		lua_pushnumber(L, 0.0f);
	}

	return 1;
}

int playstate::CanvasGroup_ComboBox(lua_State* L)
{
	int numParams = lua_gettop(L);
	if(numParams < 5) {
		luaM_printerror(L, "Expected: self<CanvasGroup>:ComboBox(Size, Position, SelectedIndex, Values)");
		lua_pushinteger(L, 0);
		return 1;
	}

	// Load the tabel later
	lua_pop(L, 1);
	const uint32 selectedIndex = lua_tointeger(L, -1); lua_pop(L, 1);
	const Vector2 position = luaM_popvector2(L);
	const Size size = luaM_poppoint(L);

	ScriptableCanvasGroup* group = luaM_popobject<ScriptableCanvasGroup>(L);
	if(group != NULL) {
		const uint32 newValue = group->ComboBox(size, position, selectedIndex);
		lua_pushinteger(L, newValue);
	} else {
		luaM_printerror(L, "Expected: self<CanvasGroup>:ComboBox(Size, Position, SelectedIndex, Values)");
		lua_pushinteger(L, 0);
	}
	return 1;
}
