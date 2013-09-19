#pragma once

#include "canvas_group.h"
#include "../linked_list.h"
#include "../window/window.h"
#include "../input/input_system.h"
#include "gui_geometry_builder.h"
#include "../math/matrix4x4.h"
#include "../processor/update_processor.h"
#include "gui_mouse_state.h"

namespace playstate
{
	//
	// The canvas is the main drawing area for all user interface related objects. The internal positioning system
	// is a uniform [0, 1] coordinate system where the top left is 0, 0 and bottom right is 1, 1. 
	class Canvas : public IUpdateProcessor
	{
	public:
		Canvas(IWindow& window, IInputSystem& inputSystem, std::auto_ptr<IUpdateProcessor> updateProcessor);
		~Canvas();

		//
		// Adds the supplied canvas group
		//
		// @param group
		void AddCanvasGroup(CanvasGroup* group);

		//
		// Removes the supplied canvas group - but doesn't delete it
		//
		// @param group
		void RemoveCanvasGroup(CanvasGroup* group);

		//
		// @param name The name of the canvas group we are looking for
		// @return The canvas group if found; NULL otherwise
		CanvasGroup* GetCanvasGroupByName(const playstate::string& name);

		//
		// Convert mouse coordinates to canvas uniform coordinates [0, 1]
		Vector2 GetMousePositionAsUniform() const;

		//
		// @return The mouse position. The position is normalized as if the window has the base-line size (Canvas.Size)
		Point GetMousePosition() const;

		//
		// @return The mouse button that's clicked
		MouseButtons::Enum GetMouseClick() const;
		
		//
		// @return The mouse button that's pressed down
		MouseButtons::Enum GetMouseDown() const;

		//
		// @param rect
		// @return The mouse state depending on supplied rect area
		GuiMouseState::Enum GetMouseState(const Rect& rect) const;
		
		//
		// Prepares the building blocks for the user interface contained in this canvas.
		//
		// @param builder
		bool ProcessCanvas(GuiGeometryBuilder& builder);

		//
		// Sets the base size for the entire canvas. All positions and sizes are in uniform size internally. 
		// 
		void SetSize(const Size& size);

		//
		// @return The size of this canvas
		const Size& GetSize() const;

		//
		// @return The project matrix generated using the supplied canvas size
		const Matrix4x4& GetProjectionMatrix() const;

	// IUpdateProcessor
	public:
		virtual void AttachUpdatable(IUpdatable* updatable);
		virtual void DetachUpdatable(IUpdatable* updatable);
		virtual void AttachTickable(ITickable* tickable);
		virtual void DetachTickable(ITickable* tickable);
		virtual void Update();

	private:
		IWindow& mWindow;
		IInputSystem& mInputSystem;
		LinkedList<CanvasGroup> mGroups;
		Size mSize;
		Matrix4x4 mProjectionMatrix;
		std::auto_ptr<IUpdateProcessor> mUpdateProcessor;

		MouseButtons::Enum mLastButtonDown;
		MouseButtons::Enum mButtonClicked;
	};
	
	// Script integration

	extern int Canvas_AddCanvasGroup(lua_State* L);
	extern int Canvas_RemoveCanvasGroup(lua_State* L);
	extern int Canvas_SetSize(lua_State* L);
	extern int Canvas_Show(lua_State* L);
	static luaL_Reg Canvas_Methods[] = {
		{ "AddCanvasGroup", Canvas_AddCanvasGroup },
		{ "RemoveCanvasGroup", Canvas_RemoveCanvasGroup },
		{ "SetSize", Canvas_SetSize },
		{ "Show" },
		{ NULL, NULL }
	};
}
