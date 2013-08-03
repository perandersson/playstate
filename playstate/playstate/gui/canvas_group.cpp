#include "../memory/memory.h"
#include "canvas_group.h"
#include "../script/script_system.h"
using namespace playstate;

CanvasGroup::CanvasGroup()
	: mUpdateProcessor(IUpdateProcessorFactory::Get().Create())
{
	assert(mUpdateProcessor != NULL && "IUpdateProcessorFactory did not create a valid update processor");
}

CanvasGroup::CanvasGroup(IUpdateProcessorFactory& updateProcessFactory)
	: mUpdateProcessor(updateProcessFactory.Create())
{
	assert(mUpdateProcessor != NULL && "IUpdateProcessorFactory did not create a valid update processor");
}

CanvasGroup::~CanvasGroup()
{
	mControls.DeleteAll();
	
	if(mUpdateProcessor != NULL) {
		delete mUpdateProcessor;
		mUpdateProcessor = NULL;
	}
}

void CanvasGroup::AddControl(GuiControl* control)
{
	assert_not_null(control);
	mControls.AddLast(control);
}

void CanvasGroup::RemoveControl(GuiControl* control)
{
	assert_not_null(control);
	delete control;
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

void CanvasGroup::Update()
{
	mUpdateProcessor->Update();
}

namespace playstate
{
	int CanvasGroup_Factory(lua_State* L)
	{
		CanvasGroup* group = new CanvasGroup();
		luaM_pushobject(L, "CanvasGroup", group);
		return 1;
	}

	int CanvasGroup_Init(lua_State* L)
	{
		if(lua_istable(L, -1) == 0) {
			lua_pop(L, 1);
			return 0;
		}

		CanvasGroup* group = new CanvasGroup();
		luaM_setinstance(L, group);
		
		const int ref = luaL_ref(L, LUA_REGISTRYINDEX);
		group->RegisterObject(L, ref);
		return 0;
	}

	int CanvasGroup_Load(lua_State* L)
	{
		const std::string fileName = lua_tostring(L, -1); lua_pop(L, 1);
		try {
			ScriptSystem& scriptSystem = ScriptSystem::Get();
			std::auto_ptr<Script> script = scriptSystem.CompileFile(fileName);
			CanvasGroup* group = script->ReadInstance<CanvasGroup>();
			luaM_pushobject(L, "CanvasGroup", group);
			return 1;
		} catch(ScriptException e) {
			ILogger::Get().Error("Could not load canvas group: '%s'. Reason: '%s'", fileName.c_str(), e.GetMessage().c_str());
		}
		
		lua_pushnil(L);
		return 1;
	}
}
