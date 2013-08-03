#pragma once
#include "../script/scriptable.h"
#include "scene_node.h"
#include "../linked_list.h"
#include "../processor/update_processor_factory.h"
#include "../processor/render_processor_factory.h"
#include "../processor/light_source_processor_factory.h"

namespace playstate
{
	//
	// A SceneGroup is the concept where we group scene nodes together in a logical structure. A scene group can
	// be a level, where all the contained nodes are items for that specific level.
	class SceneGroup : public Scriptable, public IUpdateProcessor, public IRenderProcessor, public ILightSourceProcessor
	{
	public:
		LinkedListLink<SceneGroup> GroupLink;

	public:
		SceneGroup();
		SceneGroup(IUpdateProcessorFactory& updateProcessFactory, IRenderProcessorFactory& renderProcessFactory, 
			ILightSourceProcessorFactory& lightSourceProcessorFactory);
		virtual ~SceneGroup();

		//
		// 
		void AddSceneNode(SceneNode* node);

		//
		//
		void RemoveSceneNode(SceneNode* node);
		
	// IUpdateProcessor
	public:
		virtual void AttachUpdatable(IUpdatable* updatable);
		virtual void DetachUpdatable(IUpdatable* updatable);
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

	private:
		LinkedList<SceneNode, &SceneNode::NodeLink> mSceneNodes;
		IUpdateProcessor* mUpdateProcessor;
		IRenderProcessor* mRenderProcessor;
		ILightSourceProcessor* mLightSourceProcessor;
	};
	
	//
	// Script integration
	//

	extern int SceneGroup_Factory(lua_State* L);
	extern int SceneGroup_Init(lua_State* L);
	extern int SceneGroup_Load(lua_State* L);
	static luaL_Reg SceneGroup_Methods[] = {
		{ LUA_CONSTRUCTOR, SceneGroup_Factory },
		{ LUA_INHERIT_CONSTRUCTOR, SceneGroup_Init },
		{ "Load", SceneGroup_Load },
		{ NULL, NULL }
	};
}