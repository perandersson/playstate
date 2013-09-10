#pragma once
#include "../script/script_collection.h"

namespace playstate
{
	class GuiStyle : public ScriptCollection
	{
	public:
		GuiStyle();
		GuiStyle(lua_State* L, uint32 scriptRef);
		GuiStyle(const GuiStyle& s);
		virtual ~GuiStyle();


		//
		//
		GuiStyle& operator = (const GuiStyle& s);

	private:
	};
}
