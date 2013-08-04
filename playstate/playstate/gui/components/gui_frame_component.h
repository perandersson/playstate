#pragma once

#include "../../component/component.h"
#include "../../processor/renderable.h"

namespace playstate
{
	class GuiFrameComponent : public Component, public Renderable
	{
	public:
		GuiFrameComponent(const std::string& title, uint32 width, uint32 height);
		virtual ~GuiFrameComponent();

		//
		// Change the size
		void SetSize(uint32 width, uint32 height);

		//
		// Change the title
		void SetTitle(const std::string& title);

	// Component
	public:
		virtual void OnComponentAdded();
		virtual void OnComponentRemoved();

	// Renderable
	public:
		virtual void Collect(const RenderState& state, RenderBlockResultSet* resultSet);

	private:
		VertexBuffer* mVertexBuffer;
		std::string mTitle;
		uint32 mWidth;
		uint32 mHeight;
	};
}
