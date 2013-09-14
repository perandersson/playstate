#include "../memory/memory.h"
#include "canvas_group.h"
#include "../processor/processors/linked_list_update_processor.h"
#include "../script/script_system.h"
using namespace playstate;

CanvasGroup::CanvasGroup(std::auto_ptr<IUpdateProcessor> updateProcessor)
	: mUpdateProcessor(updateProcessor), mWidgets(offsetof(GuiWidget, GuiWidgetLink))
{
	assert_not_null(mUpdateProcessor.get());
}

CanvasGroup::~CanvasGroup()
{
	mWidgets.DeleteAll();
}

void CanvasGroup::AttachUpdatable(IUpdatable* updatable)
{
	assert_not_null(updatable);
	mUpdateProcessor->AttachUpdatable(updatable);
}

void CanvasGroup::DetachUpdatable(IUpdatable* updatable)
{
	assert_not_null(updatable);
	mUpdateProcessor->DetachUpdatable(updatable);
}

void CanvasGroup::AttachTickable(ITickable* tickable)
{
	assert_not_null(tickable);
	mUpdateProcessor->AttachTickable(tickable);
}

void CanvasGroup::DetachTickable(ITickable* tickable)
{
	assert_not_null(tickable);
	mUpdateProcessor->DetachTickable(tickable);
}

void CanvasGroup::Update()
{
	mUpdateProcessor->Update();
}

void CanvasGroup::AddWidget(GuiWidget* widget)
{
	assert_not_null(widget);
	mWidgets.AddLast(widget);
	widget->WidgetAttachedToCanvasGroup(this);
}

void CanvasGroup::RemoveWidget(GuiWidget* widget)
{
	assert_not_null(widget);
	widget->DetachingWidgetFromCanvasGroup(this);
	mWidgets.Remove(widget);
}

const Vector2& CanvasGroup::GetPosition() const
{
	return mPosition;
}

const Vector2& CanvasGroup::GetSize() const
{
	return mSize;
}

void CanvasGroup::SetStyle(const GuiStyle& style)
{
	mStyle = style;
	GuiWidget* widget = mWidgets.First();
	while(widget != NULL) {
		widget->UpdateStyle(mStyle);
		widget = widget->GuiWidgetLink.Tail;
	}
}

bool CanvasGroup::PreRender(GuiGeometryBuilder& builder)
{
	GuiWidget* widget = mWidgets.First();
	while(widget != NULL) {
		widget->PreRender(builder);
		widget = widget->GuiWidgetLink.Tail;
	}
	return true;
}

int playstate::CanvasGroup_Factory(lua_State* L)
{
	std::auto_ptr<IUpdateProcessor> up(new LinkedListUpdateProcessor());
	CanvasGroup* group = new CanvasGroup(up);
	luaM_pushobject(L, "CanvasGroup", group);
	return 1;
}

int playstate::CanvasGroup_Load(lua_State* L)
{
	const playstate::string fileName = lua_tostring(L, -1); lua_pop(L, 1);
	try {
		ScriptSystem& scriptSystem = ScriptSystem::Get();
		std::auto_ptr<Script> script = scriptSystem.CompileFile(fileName);
		CanvasGroup* group = script->ReadInstance<CanvasGroup>();
		if(group == NULL) {
			ILogger::Get().Error("The file: '%s' did not return a canvas group.", fileName.c_str());
			lua_pushnil(L);
		}
		else
			luaM_pushobject(L, "CanvasGroup", group);
		return 1;
	} catch(ScriptException e) {
		ILogger::Get().Error("Could not load canvas group: '%s'. Reason: '%s'", fileName.c_str(), e.GetMessage().c_str());
	}
		
	lua_pushnil(L);
	return 1;
}

int playstate::CanvasGroup_SetStyle(lua_State* L)
{
	if(!lua_istable(L, -1)) {
		luaM_printerror(L, "Expected: self<CanvasGroup>:SetStyle(table)");
		return 0;
	}

	int configRef = luaL_ref(L, LUA_REGISTRYINDEX);
	GuiStyle style(L, configRef);
	CanvasGroup* self = luaM_popobject<CanvasGroup>(L);
	if(self != NULL) {
		self->SetStyle(style);
	} else {
		luaM_printerror(L, "Expected: self<CanvasGroup>:SetStyle(table)");
		return 0;
	}

	return 0;
}

int playstate::CanvasGroup_AddWidget(lua_State* L)
{
	GuiWidget* widget = luaM_popobject<GuiWidget>(L);
	CanvasGroup* self = luaM_popobject<CanvasGroup>(L);
	if(self != NULL && widget != NULL) {
		self->AddWidget(widget);
	}
	return 0;
}

int playstate::CanvasGroup_RemoveWidget(lua_State* L)
{
	GuiWidget* widget = luaM_popobject<GuiWidget>(L);
	CanvasGroup* self = luaM_popobject<CanvasGroup>(L);
	if(self != NULL && widget != NULL) {
		self->RemoveWidget(widget);
	}
	return 0;
}
