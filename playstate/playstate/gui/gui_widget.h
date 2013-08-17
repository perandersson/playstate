#pragma once
#include "../linked_list.h"
#include "../script/scriptable.h"
#include "gui_event.h"
#include "../math/vector2.h"
#include "gui_geometry_builder.h"
#include <vector>

namespace playstate
{
	class CanvasGroup;

	class GuiWidget : public Scriptable
	{
		typedef std::vector<GuiWidget*> Children;

	public:
		LinkedListLink<GuiWidget> GuiWidgetLink;

	public:
		GuiWidget();
		GuiWidget(GuiWidget* parent);
		GuiWidget(CanvasGroup* group);
		virtual ~GuiWidget();

		//
		// @return The canvas group this node is attached to
		CanvasGroup* GetCanvasGroup();

		//
		// Adds a new event listener for this widget.
		// 
		// @param event
		void AddEvent(GuiEvent* event);

		//
		// Removes the supplied event from this widget
		//
		// @param event
		void RemoveEvent(GuiEvent* event);

		//
		// Removes this widget from the canvas.
		void RemoveFromCanvas();
		
		//
		// Notifies this widget that it's been attached to the supplied group
		// 
		// This method might be called while it's already attached to a group. This
		//	means that this widget will be moved from one group to the next.
		//
		// @param group
		void WidgetAttachedToCanvasGroup(CanvasGroup* group);

		//
		// Notifies this widget that it's being detached from the supplied group
		void DetachingWidgetFromCanvasGroup(CanvasGroup* group);

		//
		// @return TRUE if this widget control is attached to a canvas group.
		bool IsAttachedToCanvasGroup() const;

		//
		// @return The top-left uniform position for this widget.
		const Vector2& GetPosition() const;

		//
		// @return The top-left 
		const Vector2& GetAbsolutePosition() const;

		//
		// Sets this gui widgets position.
		void SetPosition(const Vector2& position);

		//
		// @return The uniform size of this widget
		const Vector2& GetSize() const;

		//
		// Sets this widgets size
		void SetSize(const Vector2& size);

		//
		// Builds this GUI widgets geometry in preperation for drawing it onto the screen.
		virtual const void BuildWidgetGeometry(GuiGeometryBuilder& builder) const;

		void AddWidget(GuiWidget* widget);

		void RemoveWidget(GuiWidget* widget);

	protected:
		//
		// Updates the absolute position value based on the parents position
		void UpdatePosition();

	private:
		CanvasGroup* mCanvasGroup;
		GuiWidget* mParent;
		Children mChildren;
		LinkedList<GuiEvent> mEvents;

	protected:
		Vector2 mPosition;
		Vector2 mAbsolutePosition;
		Vector2 mSize;
	};
	
	// Script integration

	extern int GuiWidget_SetPosition(lua_State* L);
	extern int GuiWidget_SetSize(lua_State* L);
	extern int GuiWidget_AddWidget(lua_State* L);
	extern int GuiWidget_RemoveWidget(lua_State* L);
	static luaL_Reg GuiWidget_Methods[] = {
		{ "SetPosition", GuiWidget_SetPosition },
		{ "SetSize", GuiWidget_SetSize },
		{ "AddWidget", GuiWidget_AddWidget },
		{ "RemoveWidget", GuiWidget_RemoveWidget },
		{ NULL, NULL }
	};
}
