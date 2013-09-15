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

void ScriptableCanvasGroup::OnProcessCanvas()
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
		return 0;
	}

	playstate::string text;
	if(numParams >= 4 && lua_isstring(L, -1))
		text = lua_tostring(L, -1); lua_pop(L, 1);

	const Vector2 position = luaM_popvector2(L);
	const Size size = luaM_poppoint(L);

	ScriptableCanvasGroup* group = luaM_popobject<ScriptableCanvasGroup>(L);
	if(group != NULL) {
		group->Button(size, position, text);
	} else {
		luaM_printerror(L, "Expected: self<CanvasGroup>:Button(Size, Position, [Text])");
	}

	return 0;
}
