#pragma once
#include "gui_widget.h"
#include "../singleton.h"
#include "gui_geometry_builder.h"

namespace playstate
{
	//
	// 
	class IWidgetRenderProcessor
	{
	public:
		virtual ~IWidgetRenderProcessor() {}

	public:
		//
		// Attaches a widget to receive render events.
		//
		// @param widget
		virtual void AttachWidget(GuiWidget* widget) = 0;

		//
		// Detaches the supplied widget instancer so that it no longer receives any render events.
		//
		// @param widget
		virtual void DetachWidget(GuiWidget* widget) = 0;

		//
		virtual bool BuildWidgetGeometry(GuiGeometryBuilder& builder) const = 0;
	};

	//
	// Factory that's responsible for creating stand-alone render processors
	class IWidgetRenderProcessorFactory : public Singleton<IWidgetRenderProcessorFactory>
	{
	public:
		virtual ~IWidgetRenderProcessorFactory() {}

	public:
		//
		// @return A render processor
		virtual IWidgetRenderProcessor* Create() const = 0;
	};
}
