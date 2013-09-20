#pragma once

#include "../linked_list.h"
#include "../processor/update_processor.h"
#include "../math/rect.h"
#include "../math/point.h"
#include "gui_geometry_builder.h"
#include "../script/scriptable.h"
#include "gui_style.h"
#include "control/slider_control.h"
#include "control/label_control.h"
#include "control/checkbox_control.h"
#include "control/button_control.h"
#include "control/frame_control.h"

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
		// @param name The name of this CanvasGroup
		void SetName(const playstate::string& name);

		//
		// @return The name of this CanvasGroup.
		inline const playstate::string& GetName() const {
			return mName;
		}

		//
		// Process this CanvasGroup and prepare it to generate GUI geometry to be drawn.
		//
		// @param builder The Geometry Builder
		void ProcessCanvas(GuiGeometryBuilder* builder);

		//
		// Calculates the absolute coordinates for the supplied relative position
		//
		// @param relativePosition
		// @return The absolute position.
		Rect GetAbsoluteCoordinates(const Rect& rect) const;
		
	public:
		//
		// Begins a frame
		void BeginFrame(const Rect& rect, const playstate::string& title);

		//
		// Ends the current frame
		void EndFrame();

		//
		// 
		bool Button(const Rect& rect, const playstate::string& text);
		
		//
		// Creates a label
		void Label(const Rect& rect, const playstate::string& text);

		//
		// 
		bool Checkbox(const Rect& rect, bool toggled, const playstate::string& text);

		//
		// 
		float32 Slider(const Rect& rect, float32 value, float32 leftValue, float32 rightValue,
			float32 stepValue);

		//
		// Creates a combobox and returns the current index
		// 
		uint32 ComboBox(const Rect& rect, uint32 selectedIndex);

	protected:
		//
		// Method called when this canvas groups user interface is to be processed.
		//
		// @param builder The canvas geometry builder used for creating geometry visible in the user interface
		virtual void OnProcessCanvas(GuiGeometryBuilder* builder);

		//
		// Method called when this canvas group is added to the canvas
		virtual void OnAddedToCanvas();

		//
		// Method called right before this canvas group is removed from the canvas
		virtual void OnRemovingFromCanvas();

	private:
		Rect AddShadowRect(const Rect& rect);
		Rect AddBorderRect(const Rect& rect);

	protected:
		Canvas* mCanvas;
		playstate::string mName;
		GuiGeometryBuilder* mGeometryBuilder;

		GuiStyle mStyle;
		int32 mShadowOffset;
		Color mShadowColor;

		SliderControl mSliderControl;
		LabelControl mLabelControl;
		CheckboxControl mCheckboxControl;
		ButtonControl mButtonControl;
		FrameControl mFrameControl;

		std::stack<Point> mPositions;
	};
}
