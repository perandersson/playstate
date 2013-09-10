#include "../memory/memory.h"
#include "scene_group.h"
#include "../processor/processors/linked_list_update_processor.h"
#include "../processor/processors/quadtree_light_source_processor.h"
#include "../processor/processors/quadtree_render_processor.h"
#include "../script/script_system.h"
#include "../search/find_query.h"
using namespace playstate;

SceneGroup::SceneGroup(std::auto_ptr<IUpdateProcessor> updateProcessor, std::auto_ptr<IRenderProcessor> renderProcessor, 
			std::auto_ptr<ILightSourceProcessor> lightSourceProcessor)
	: Scriptable(), 
	mUpdateProcessor(updateProcessor), mRenderProcessor(renderProcessor), mLightSourceProcessor(lightSourceProcessor), 
	mComponents(offsetof(Component, ComponentLink)), mChildren(offsetof(SceneNode, NodeLink))
{
	assert(mUpdateProcessor.get() != NULL && "IUpdateProcessorFactory did not create a valid update processor");
	assert(mRenderProcessor.get() != NULL && "IRenderProcessorFactory did not create a valid render processor");
	assert(mLightSourceProcessor.get() != NULL && "ILightSourceProcessorFactory did not create a valid light source processor");
}

SceneGroup::~SceneGroup()
{
	mComponents.DeleteAll();
	mChildren.DeleteAll();
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
	assert(query.Camera != NULL && "Why are you trying to find items using a non-existing camera?");

	// Move the camera into the SceneGroup view space
	Camera localCamera(*query.Camera);
	localCamera.Move(GetPosition() * -1.0f);

	FindQuery localQuery;
	localQuery.Camera = &localCamera;
	localQuery.Filter = query.Filter;
	return mRenderProcessor->Find(localQuery, target);
}

bool SceneGroup::Find(const FindQuery& query, LightSourceResultSet* target) const
{
	return mLightSourceProcessor->Find(query, target);
}

void SceneGroup::AddComponent(Component* component)
{
	assert_not_null(component);
	assert(component->GetNode() == NULL && "You are not allowed to add a component on multiple scene nodes");
	
	mComponents.AddLast(component);
	component->OnAttachedToSceneGroup(this);
}

void SceneGroup::RemoveComponent(Component* component)
{
	assert_not_null(component);
	assert(component->GetGroup() == this && "You are not allowed to remove a component on that isn't attached here");

	delete component;
}

Component* SceneGroup::GetComponent(type_mask typeMask)
{
	Component* component = mComponents.First();
	while(component != NULL) {
		if(BIT_ISSET(component->GetTypeMask(), typeMask))
			return component;

		component = component->ComponentLink.Tail;
	}

	return NULL;
}

void SceneGroup::AddChild(SceneNode* node)
{
	assert_not_null(node);

	mChildren.AddLast(node);
	node->NodeAttachedToSceneGroup(this);
}

void SceneGroup::RemoveChild(SceneNode* node)
{
	assert_not_null(node);
	assert(node->GetGroup() == this && "You are not allowed to remove another scene objects child node");

	node->DetachingNodeFromSceneGroup(this);
	mChildren.Remove(node);
}

void SceneGroup::SetPosition(const Vector3& position)
{
	mPosition = position;

	SceneNode* child = mChildren.First();
	while(child != NULL) {
		child->UpdatePosition(Vector3());
		child = child->NodeLink.Tail;
	}
}

void SceneGroup::FireEvent(uint32 typeID, uint32 messageID)
{
	Component* component = mComponents.First();
	while(component != NULL) {
		Component* next = component->ComponentLink.Tail;
		component->NotifyOnEvent(typeID, messageID);
		component = next;
	}

	SceneNode* child = mChildren.First();
	while(child != NULL) {
		SceneNode* next = child->NodeLink.Tail;
		child->FireEvent(typeID, messageID);
		child = next;
	}
}

void SceneGroup::FireEvent(uint32 typeID, uint32 messageID, type_mask typeMask)
{
	Component* component = mComponents.First();
	while(component != NULL) {
		Component* next = component->ComponentLink.Tail;
		component->NotifyOnEvent(typeID, messageID);
		component = next;
	}

	SceneNode* child = mChildren.First();
	while(child != NULL) {
		SceneNode* next = child->NodeLink.Tail;
		child->FireEvent(typeID, messageID, typeMask);
		child = next;
	}
}

int playstate::SceneGroup_Factory(lua_State* L)
{
	std::auto_ptr<IUpdateProcessor> up;//(new LinkedListUpdateProcessor());
	std::auto_ptr<ILightSourceProcessor> lp;//(new QuadTreeLightSourceProcessor());
	std::auto_ptr<IRenderProcessor> rp;//(new QuadTreeRenderProcessor());
	if(lua_gettop(L) > 1) {
		std::auto_ptr<ScriptCollection> collection(luaM_popcollection(L));
	} else {
		up = std::auto_ptr<IUpdateProcessor>(new LinkedListUpdateProcessor());
		lp = std::auto_ptr<ILightSourceProcessor>(new QuadTreeLightSourceProcessor());
		rp = std::auto_ptr<IRenderProcessor>(new QuadTreeRenderProcessor());
	}

	SceneGroup* sceneGroup = new SceneGroup(up, rp, lp);
	luaM_pushobject(L, "SceneGroup", sceneGroup);
	return 1;
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

int playstate::SceneGroup_AddComponent(lua_State* L)
{
	Component* component = luaM_popobject<Component>(L);
	SceneGroup* group = luaM_popobject<SceneGroup>(L);
	if(component != NULL && group != NULL) {
		group->AddComponent(component);
	} else {
		luaM_printerror(L, "Expected: self<SceneGroup>:AddComponent(Component)");
	}

	return 0;
}

int playstate::SceneGroup_RemoveComponent(lua_State* L)
{
	Component* component = luaM_popobject<Component>(L);
	SceneGroup* group = luaM_popobject<SceneGroup>(L);
	if(component != NULL && group != NULL) {
		group->RemoveComponent(component);
	} else {
		luaM_printerror(L, "Expected: self<SceneGroup>:RemoveComponent(Component)");
	}

	return 0;
}

int playstate::SceneGroup_SetPosition(lua_State* L)
{
	Vector3 vec = luaM_popvector3(L);
	SceneGroup* group = luaM_popobject<SceneGroup>(L);
	if(group != NULL) {
		group->SetPosition(vec);
	} else {
		luaM_printerror(L, "Expected: self<SceneGroup>:SetPosition(Vector3)");
	}

	return 0;
}
