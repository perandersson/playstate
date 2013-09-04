#include "../memory/memory.h"
#include "scene_group.h"
#include "../script/script_system.h"
using namespace playstate;

SceneGroup::SceneGroup()
	: mUpdateProcessor(IUpdateProcessorFactory::Get().Create()),
	mRenderProcessor(IRenderProcessorFactory::Get().Create()),
	mLightSourceProcessor(ILightSourceProcessorFactory::Get().Create()), 
	mSceneNodes(offsetof(SceneNode, NodeLink))
{
	assert(mUpdateProcessor.get() != NULL && "IUpdateProcessorFactory did not create a valid update processor");
	assert(mRenderProcessor.get() != NULL && "IRenderProcessorFactory did not create a valid render processor");
	assert(mLightSourceProcessor.get() != NULL && "ILightSourceProcessorFactory did not create a valid light source processor");
}

SceneGroup::SceneGroup(std::auto_ptr<IUpdateProcessor> updateProcessor, std::auto_ptr<IRenderProcessor> renderProcessor, 
			std::auto_ptr<ILightSourceProcessor> lightSourceProcessor)
	: mUpdateProcessor(updateProcessor), mRenderProcessor(renderProcessor), mLightSourceProcessor(lightSourceProcessor), 
	mSceneNodes(offsetof(SceneNode, NodeLink))
{
	assert(mUpdateProcessor.get() != NULL && "IUpdateProcessorFactory did not create a valid update processor");
	assert(mRenderProcessor.get() != NULL && "IRenderProcessorFactory did not create a valid render processor");
	assert(mLightSourceProcessor.get() != NULL && "ILightSourceProcessorFactory did not create a valid light source processor");
}

SceneGroup::~SceneGroup()
{
	mSceneNodes.DeleteAll();
}

void SceneGroup::AddNode(SceneNode* node)
{
	assert_not_null(node);
	mSceneNodes.AddLast(node);
	node->NodeAttachedToSceneGroup(this);
}

void SceneGroup::RemoveNode(SceneNode* node)
{
	assert_not_null(node);
	node->DetachingNodeFromSceneGroup(this);
	mSceneNodes.Remove(node);
}

void SceneGroup::FireEvent(uint32 typeID, uint32 messageID)
{
	SceneNode* node = mSceneNodes.First();
	while(node != NULL) {
		SceneNode* next = node->NodeLink.Tail;
		node->FireEvent(typeID, messageID);
		node = next;
	}
}

void SceneGroup::FireEvent(uint32 typeID, uint32 messageID, type_mask typeMask)
{
	SceneNode* node = mSceneNodes.First();
	while(node != NULL) {
		SceneNode* next = node->NodeLink.Tail;
		node->FireEvent(typeID, messageID, typeMask);
		node = next;
	}
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

void SceneGroup::AttachTickable(ITickable* tickable)
{
	assert_not_null(tickable);
	mUpdateProcessor->AttachTickable(tickable);
}

void SceneGroup::DetachTickable(ITickable* tickable)
{
	assert_not_null(tickable);
	mUpdateProcessor->AttachTickable(tickable);
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

int playstate::SceneGroup_Factory(lua_State* L)
{
	SceneGroup* sceneGroup = new SceneGroup();
	luaM_pushobject(L, "SceneGroup", sceneGroup);
	return 1;
}

int playstate::SceneGroup_Init(lua_State* L)
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

int playstate::SceneGroup_Load(lua_State* L)
{
	const playstate::string fileName = lua_tostring(L, -1); lua_pop(L, 1);
	try {
		ScriptSystem& scriptSystem = ScriptSystem::Get();
		std::auto_ptr<Script> script = scriptSystem.CompileFile(fileName);
		SceneGroup* group = script->ReadInstance<SceneGroup>();
		if(group == NULL) {
			ILogger::Get().Error("The file: '%s' did not return a scene group.", fileName.c_str());
			lua_pushnil(L);
		}
		else
			luaM_pushobject(L, "SceneGroup", group);
		return 1;
	} catch(ScriptException e) {
		ILogger::Get().Error("Could not load scene group: '%s'. Reason: '%s'", fileName.c_str(), e.GetMessage().c_str());
	}
		
	lua_pushnil(L);
	return 1;
}

int playstate::SceneGroup_AddNode(lua_State* L)
{
	SceneNode* node = luaM_popobject<SceneNode>(L);
	SceneGroup* self = luaM_popobject<SceneGroup>(L);
	if(self != NULL && node != NULL) {
		self->AddNode(node);
	}
	return 0;
}

int playstate::SceneGroup_RemoveNode(lua_State* L)
{
	SceneNode* node = luaM_popobject<SceneNode>(L);
	SceneGroup* self = luaM_popobject<SceneGroup>(L);
	if(self != NULL && node != NULL) {
		self->RemoveNode(node);
	}
	return 0;
}

int playstate::SceneGroup_FireEvent(lua_State* L)
{
	type_mask typeMask = 0;
	if(lua_gettop(L) < 3) {
		luaM_printerror(L, "Expected: self<SceneGroup>:FireEvent(typeID, messageID[, typeMask])");
		lua_pushnil(L);
		return 1;
	}

	if(lua_gettop(L) > 3)
		typeMask = lua_tointeger(L, -1); lua_pop(L, 1);
		
	uint32 messageID = lua_tointeger(L, -1); lua_pop(L, 1);
	uint32 typeID = lua_tointeger(L, -1); lua_pop(L, 1);
	SceneGroup* group = luaM_popobject<SceneGroup>(L);
	if(group != NULL) {
		if(typeMask == 0)
			group->FireEvent(typeID, messageID);
		else
			group->FireEvent(typeID, messageID, typeMask);
	} else {
		luaM_printerror(L, "Expected: self<SceneGroup>:FireEvent(typeID, messageID[, typeMask])");
	}

	return 0;
}

