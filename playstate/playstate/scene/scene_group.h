#pragma once
#include "../script/scriptable.h"
#include "scene_node.h"
#include "../linked_list.h"
#include "../processor/update_processor.h"
#include "../processor/render_processor.h"
#include "../processor/light_source_processor.h"

namespace playstate
{
	//
	// A SceneGroup is the concept where we group scene nodes together in a logical structure. A scene group can
	// be a level, where all the contained nodes are items for that specific level.
	class SceneGroup : public SceneNode, public IUpdateProcessor, public IRenderProcessor, public ILightSourceProcessor
	{
	public:
		LinkedListLink<SceneGroup> GroupLink;

	public:
		SceneGroup(std::auto_ptr<IUpdateProcessor> updateProcessor, std::auto_ptr<IRenderProcessor> renderProcessor, 
			std::auto_ptr<ILightSourceProcessor> lightSourceProcessor);
		virtual ~SceneGroup();

	// IUpdateProcessor
	public:
		virtual void AttachUpdatable(IUpdatable* updatable);
		virtual void DetachUpdatable(IUpdatable* updatable);
		virtual void AttachTickable(ITickable* tickable);
		virtual void DetachTickable(ITickable* tickable);
		virtual void Update();

	// IRenderProcessor
	public:
		virtual void AttachRenderable(Renderable* renderable);
		virtual void DetachRenderable(Renderable* renderable);
		virtual bool Find(const FindQuery& query, RenderBlockResultSet* target) const;

	// ILightSourceProcessor
	public:
		virtual void AttachLightSource(LightSource* lightSource);
		virtual void DetachLightSource(LightSource* lightSource);
		virtual bool Find(const FindQuery& query, LightSourceResultSet* target) const;

	//
	protected:
		virtual void OnChildAdded(SceneNode* node);
		virtual void OnChildRemoved(SceneNode* node);

	private:
		std::auto_ptr<IUpdateProcessor> mUpdateProcessor;
		std::auto_ptr<IRenderProcessor> mRenderProcessor;
		std::auto_ptr<ILightSourceProcessor> mLightSourceProcessor;
	};
	
	//
	// Script integration
	//

	extern int SceneGroup_Factory(lua_State* L);
	extern int SceneGroup_Init(lua_State* L);
	extern int SceneGroup_Load(lua_State* L);
	extern int SceneGroup_AddChild(lua_State* L);
	extern int SceneGroup_RemoveChild(lua_State* L);
	extern int SceneGroup_FireEvent(lua_State* L);
	static luaL_Reg SceneGroup_Methods[] = {
		{ LUA_CONSTRUCTOR, SceneGroup_Factory },
		{ LUA_INHERIT_CONSTRUCTOR, SceneGroup_Init },
		{ "Load", SceneGroup_Load },
		{ "AddChild", SceneGroup_AddChild },
		{ "RemoveChild", SceneGroup_RemoveChild },
		{ "FireEvent", SceneGroup_FireEvent },
		{ NULL, NULL }
	};
}