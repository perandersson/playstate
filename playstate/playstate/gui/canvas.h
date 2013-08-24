#pragma once

#include "canvas_group.h"
#include "../linked_list.h"
#include "../window/window.h"
#include "../input/input_system.h"
#include "gui_geometry_builder.h"
#include "../math/matrix4x4.h"

namespace playstate
{
	//
	// The canvas is the main drawing area for all user interface related objects. The internal positioning system
	// is a uniform [0, 1] coordinate system where the top left is 0, 0 and bottom right is 1, 1. 
	class Canvas
	{
	public:
		Canvas(IWindow& window, IInputSystem& inputSystem);
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
		// Update this canvas and all it's canvas groups.
		void Update();
		
		//
		// Convert mouse coordinates to canvas uniform coordinates [0, 1]
		Vector2 GetMousePositionAsUniform() const;
		
		//
		// Prepares the building blocks for the user interface contained in this canvas.
		//
		// @param builder
		bool PreRender(GuiGeometryBuilder& builder);

		//
		// Sets the base size for the entire canvas. All positions and sizes are in uniform size internally. 
		// 
		void SetSize(const Vector2& size);

		//
		// @return The size of this canvas
		const Vector2& GetSize() const;

		//
		// @return The project matrix generated using the supplied canvas size
		const Matrix4x4& GetProjectionMatrix() const;

	private:
		IWindow& mWindow;
		IInputSystem& mInputSystem;
		LinkedList<CanvasGroup> mGroups;
		Vector2 mSize;
		Matrix4x4 mProjectionMatrix;
	};
	
	// Script integration

	extern int Canvas_AddCanvasGroup(lua_State* L);
	extern int Canvas_RemoveCanvasGroup(lua_State* L);
	extern int Canvas_SetSize(lua_State* L);
	static luaL_Reg Canvas_Methods[] = {
		{ "AddCanvasGroup", Canvas_AddCanvasGroup },
		{ "RemoveCanvasGroup", Canvas_RemoveCanvasGroup },
		{ "SetSize", Canvas_SetSize },
		{ NULL, NULL }
	};
}
