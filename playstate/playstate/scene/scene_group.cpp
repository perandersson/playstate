#include "../memory/memory.h"
#include "scene_group.h"
#include "../processor/processors/linked_list_update_processor.h"
#include "../processor/processors/quadtree_light_source_processor.h"
#include "../processor/processors/quadtree_render_processor.h"
#include "../script/script_system.h"
using namespace playstate;

SceneGroup::SceneGroup(std::auto_ptr<IUpdateProcessor> updateProcessor, std::auto_ptr<IRenderProcessor> renderProcessor, 
			std::auto_ptr<ILightSourceProcessor> lightSourceProcessor)
	: SceneNode(), mUpdateProcessor(updateProcessor), mRenderProcessor(renderProcessor), mLightSourceProcessor(lightSourceProcessor)
{
	assert(mUpdateProcessor.get() != NULL && "IUpdateProcessorFactory did not create a valid update processor");
	assert(mRenderProcessor.get() != NULL && "IRenderProcessorFactory did not create a valid render processor");
	assert(mLightSourceProcessor.get() != NULL && "ILightSourceProcessorFactory did not create a valid light source processor");
}

SceneGroup::~SceneGroup()
{
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

void SceneGroup::OnChildAdded(SceneNode* node)
{
	node->NodeAttachedToSceneGroup(this);
	SceneNode::OnChildAdded(node);
}

void SceneGroup::OnChildRemoved(SceneNode* node)
{
	node->DetachingNodeFromSceneGroup(this);
	SceneNode::OnChildRemoved(node);
}

int playstate::SceneGroup_Factory(lua_State* L)
{
	std::auto_ptr<IUpdateProcessor> up(new LinkedListUpdateProcessor());
	std::auto_ptr<ILightSourceProcessor> lp(new QuadTreeLightSourceProcessor());
	std::auto_ptr<IRenderProcessor> rp(new QuadTreeRenderProcessor());

	SceneGroup* sceneGroup = new SceneGroup(up, rp, lp);
	luaM_pushobject(L, "SceneGroup", sceneGroup);
	return 1;
}

int playstate::SceneGroup_Init(lua_State* L)
{
	if(lua_istable(L, -1) == 0) {
		lua_pop(L, 1);
		return 0;
	}

	std::auto_ptr<IUpdateProcessor> up(new LinkedListUpdateProcessor());
	std::auto_ptr<ILightSourceProcessor> lp(new QuadTreeLightSourceProcessor());
	std::auto_ptr<IRenderProcessor> rp(new QuadTreeRenderProcessor());

	SceneGroup* sceneGroup = new SceneGroup(up, rp, lp);
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

int playstate::SceneGroup_AddChild(lua_State* L)
{
	SceneNode* node = luaM_popobject<SceneNode>(L);
	SceneGroup* self = luaM_popobject<SceneGroup>(L);
	if(self != NULL && node != NULL) {
		self->AddChild(node);
	}
	return 0;
}

int playstate::SceneGroup_RemoveChild(lua_State* L)
{
	SceneNode* node = luaM_popobject<SceneNode>(L);
	SceneGroup* self = luaM_popobject<SceneGroup>(L);
	if(self != NULL && node != NULL) {
		self->RemoveChild(node);
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

