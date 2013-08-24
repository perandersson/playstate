#pragma once

#include "../linked_list.h"
#include "../processor/update_processor_factory.h"
#include "gui_widget.h"
#include "../math/vector2.h"
#include "../math/point.h"
#include "gui_geometry_builder.h"
#include "../script/scriptable.h"
#include "gui_style.h"

namespace playstate
{
	//
	// A CanvasGroup is a container which manages collections of CanvasNodes and their related action types.
	// 
	class CanvasGroup : public Scriptable, public IUpdateProcessor
	{
	public:
		LinkedListLink<CanvasGroup> CanvasGroupLink;

	public:
		CanvasGroup();
		CanvasGroup(IUpdateProcessorFactory& updateProcessorFactory);
		virtual ~CanvasGroup();

		//
		// Adds the supplied node to this canvas group
		void AddWidget(GuiWidget* widget);

		//
		// Removes this node from the canvas group
		void RemoveWidget(GuiWidget* widget);
		
		//
		const Vector2& GetPosition() const;

		//
		const Vector2& GetSize() const;

		//
		// Updates the style for all items in this canvas group with the supplied style
		//
		// @param style
		void SetStyle(const GuiStyle& style);

		//
		// 
		inline const GuiStyle& GetStyle() const {
			return mStyle;
		}
		
		//
		// Builds the geometry needed to draw the user interface
		//
		// @param builder
		bool PreRender(GuiGeometryBuilder& builder);

	// IUpdateProcessor
	public:
		virtual void AttachUpdatable(IUpdatable* updatable);
		virtual void DetachUpdatable(IUpdatable* updatable);
		virtual void AttachTickable(ITickable* tickable);
		virtual void DetachTickable(ITickable* tickable);
		virtual void Update();

	private:
		LinkedList<GuiWidget> mWidgets;
		IUpdateProcessor* mUpdateProcessor;

	private:
		Vector2 mPosition;
		Vector2 mSize;
		GuiStyle mStyle;
	};
	
	// Script integration

	extern int CanvasGroup_Factory(lua_State* L);
	extern int CanvasGroup_Load(lua_State* L);
	extern int CanvasGroup_SetStyle(lua_State* L);
	extern int CanvasGroup_AddWidget(lua_State* L);
	extern int CanvasGroup_RemoveWidget(lua_State* L);
	static luaL_Reg CanvasGroup_Methods[] = {
		{ LUA_CONSTRUCTOR, CanvasGroup_Factory },
		{ "Load", CanvasGroup_Load },
		{ "SetStyle", CanvasGroup_SetStyle },
		{ "AddWidget", CanvasGroup_AddWidget },
		{ "RemoveWidget", CanvasGroup_RemoveWidget },
		{ NULL, NULL }
	};
}
