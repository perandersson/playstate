#include "../memory/memory.h"
#include "scene_group.h"
#include "../script/script_system.h"
using namespace playstate;

SceneGroup::SceneGroup()
	: mUpdateProcessor(IUpdateProcessorFactory::Get().Create()),
	mRenderProcessor(IRenderProcessorFactory::Get().Create()),
	mLightSourceProcessor(ILightSourceProcessorFactory::Get().Create())
{
	assert(mUpdateProcessor != NULL && "IUpdateProcessorFactory did not create a valid update processor");
	assert(mRenderProcessor != NULL && "IRenderProcessorFactory did not create a valid render processor");
	assert(mLightSourceProcessor != NULL && "ILightSourceProcessorFactory did not create a valid light source processor");
}

SceneGroup::SceneGroup(IUpdateProcessorFactory& updateProcessFactory, IRenderProcessorFactory& renderProcessFactory,
	ILightSourceProcessorFactory& lightSourceProcessorFactory)
	: mUpdateProcessor(updateProcessFactory.Create()),
	mRenderProcessor(renderProcessFactory.Create()),
	mLightSourceProcessor(lightSourceProcessorFactory.Create())
{
	assert(mUpdateProcessor != NULL && "IUpdateProcessorFactory did not create a valid update processor");
	assert(mRenderProcessor != NULL && "IRenderProcessorFactory did not create a valid render processor");
	assert(mLightSourceProcessor != NULL && "ILightSourceProcessorFactory did not create a valid light source processor");
}

SceneGroup::~SceneGroup()
{
	mSceneNodes.DeleteAll();

	if(mUpdateProcessor != NULL) {
		delete mUpdateProcessor;
		mUpdateProcessor = NULL;
	}

	if(mRenderProcessor != NULL) {
		delete mRenderProcessor;
		mRenderProcessor = NULL;
	}

	if(mLightSourceProcessor != NULL) {
		delete mLightSourceProcessor;
		mLightSourceProcessor = NULL;
	}
}

void SceneGroup::AddSceneNode(SceneNode* node)
{
	assert_not_null(node);
	mSceneNodes.AddLast(node);
}

void SceneGroup::RemoveSceneNode(SceneNode* node)
{
	assert_not_null(node);
	delete node;
}

void SceneGroup::Update()
{
	mUpdateProcessor->Update();
}

void SceneGroup::AttachUpdatable(IUpdatable* updatable)
{
	assert_not_null(updatable);
	mUpdateProcessor->AttachUpdatable(updatable);
}

void SceneGroup::DetachUpdatable(IUpdatable* updatable)
{
	assert_not_null(updatable);
	mUpdateProcessor->DetachUpdatable(updatable);
}

void SceneGroup::AttachRenderable(Renderable* renderable)
{
	assert_not_null(renderable);
	mRenderProcessor->AttachRenderable(renderable);
}

void SceneGroup::DetachRenderable(Renderable* renderable)
{
	assert_not_null(renderable);
	mRenderProcessor->DetachRenderable(renderable);
}

void SceneGroup::AttachLightSource(LightSource* lightSource)
{
	assert_not_null(lightSource);
	mLightSourceProcessor->AttachLightSource(lightSource);
}

void SceneGroup::DetachLightSource(LightSource* lightSource)
{
	assert_not_null(lightSource);
	mLightSourceProcessor->DetachLightSource(lightSource);
}

bool SceneGroup::Find(const FindQuery& query, RenderBlockResultSet* target) const
{
	return mRenderProcessor->Find(query, target);
}

bool SceneGroup::Find(const FindQuery& query, LightSourceResultSet* target) const
{
	return mLightSourceProcessor->Find(query, target);
}

namespace playstate
{
	int SceneGroup_Factory(lua_State* L)
	{
		SceneGroup* sceneGroup = new SceneGroup();
		luaM_pushobject(L, "SceneGroup", sceneGroup);
		return 1;
	}

	int SceneGroup_Init(lua_State* L)
	{
		if(lua_istable(L, -1) == 0) {
			lua_pop(L, 1);
			return 0;
		}

		SceneGroup* sceneGroup = new SceneGroup();
		luaM_setinstance(L, sceneGroup);
		
		const int ref = luaL_ref(L, LUA_REGISTRYINDEX);
		sceneGroup->RegisterObject(L, ref);
		return 0;
	}

	int SceneGroup_Load(lua_State* L)
	{
		const std::string fileName = lua_tostring(L, -1); lua_pop(L, 1);
		try {
			ScriptSystem& scriptSystem = ScriptSystem::Get();
			std::auto_ptr<Script> script = scriptSystem.CompileFile(fileName);
			SceneGroup* group = script->ReadInstance<SceneGroup>();
			luaM_pushobject(L, "SceneGroup", group);
			return 1;
		} catch(ScriptException e) {
			ILogger::Get().Error("Could not load scene group: '%s'. Reason: '%s'", fileName.c_str(), e.GetMessage().c_str());
		}
		
		lua_pushnil(L);
		return 1;
	}
}
