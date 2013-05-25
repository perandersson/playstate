#pragma once

#include "../types.h"
#include "../singleton.h"
#include "../script/scriptable.h"

#include "window_closed_listener.h"
#include "window_resized_listener.h"

namespace playstate
{
	class IWindow : public Singleton<IWindow>, public Scriptable
	{
	public:
		virtual uint32 GetWidth() const = 0;
		virtual uint32 GetHeight() const = 0;
		virtual void Resize(uint32 width, uint32 height) = 0;

		virtual const std::string& GetTitle() const = 0;
		virtual void SetTitle(const std::string& title) = 0;

		virtual void AddWindowClosedListener(IWindowClosedListener* listener) = 0;
		virtual void RemoveWindowClosedListener(IWindowClosedListener* listener) = 0;

		virtual void AddWindowResizedListener(IWindowResizedListener* listener) = 0;
		virtual void RemoveWindowResizedListener(IWindowResizedListener* listener) = 0;

		virtual float32 GetTimeSinceLastUpdate() const = 0;
		
		virtual void HandleEvents() = 0;
	};

}
