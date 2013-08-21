#pragma once
#include "../component.h"
#include "../../processor/renderable.h"
#include "../../font/font.h"

namespace playstate
{
	//
	// Component for displaying a billboarded text at the attached scene nodes location
	class BillboardedText : public Component, public Renderable
	{
	public:
		BillboardedText(Resource<Font> font);
		BillboardedText(Resource<Font> font, const playstate::string& text);

	private:
	};
}
