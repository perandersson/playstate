#pragma once

#include "../linked_list.h"
#include "../processor/update_processor.h"
#include "../math/vector2.h"
#include "../math/point.h"
#include "gui_geometry_builder.h"
#include "../script/scriptable.h"
#include "gui_style.h"

#include <stack>

namespace playstate
{
	class Canvas;

	//
	// A CanvasGroup is a container which manages collections of CanvasNodes and their related action types.
	// 
	class CanvasGroup : public Scriptable
	{
	public:
		LinkedListLink<CanvasGroup> CanvasGroupLink;

	public:
		CanvasGroup();
		virtual ~CanvasGroup();

		//
		// This group is added to the supplied canvas
		void AddedToCanvas(Canvas* canvas);

		//
		// This group is being removed from the supplied canvas
		void RemovingFromCanvas(Canvas* canvas);

		inline Canvas* GetCanvas() {
			return mCanvas;
		}

		inline const Canvas* GetCanvas() const {
			return mCanvas;
		}
		
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
		// Process this CanvasGroup and prepare it to generate GUI geometry to be drawn.
		//
		// @param builder The Geometry Builder
		void ProcessCanvas(GuiGeometryBuilder* builder);

		//
		// Calculates the absolute position for the supplied relative position
		//
		// @param relativePosition
		// @return
		Vector2 GetAbsolutePosition(const Vector2& relativePosition) const;
		
	public:
		//
		// Begins a frame
		void BeginFrame(const Size& size, const Vector2& position, const playstate::string& title);

		//
		// Ends the current frame
		void EndFrame();

		//
		// 
		bool Button(const Size& size, const Vector2& position, const playstate::string& text);

	protected:
		//
		// Method called when this canvas groups user interface is to be processed.
		virtual void OnProcessCanvas();

		virtual void OnAddedToCanvas();

		virtual void OnRemovingFromCanvas();

	protected:
		Canvas* mCanvas;
		GuiGeometryBuilder* mGeometryBuilder;

		GuiStyle mStyle;
		Color mBackColorTop;
		Color mBackColorBottom;
		Color mFrontColor;
		Resource<Font> mFont;

		std::stack<Vector2> mPositions;
	};
}
