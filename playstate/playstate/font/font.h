#pragma once

#include "../math/vector2.h"
#include "../script/luam.h"
#include "../resources/resource_object.h"
#include "../rendering/texture2d.h"

namespace playstate
{
	struct FontCharInfo
	{
		// The size of the actual character (width and height)
		Vector2 Size; 

		// This characters offset in X,Y. Needed so that the text looks nice.
		Vector2 Offset;

		// The texture coordinates for this character
		Vector2 BottomLeftTexCoord;
		Vector2 TopRightTexCoord;
	};

	class Font : public Texture2D
	{
	public:
		typedef std::hash_map<playstate::character, FontCharInfo*> FontCharInfoMap;

	public:
		Font(GLuint textureId, uint32 width, uint32 height, const FontCharInfoMap& infoMap);
		virtual ~Font();

		//
		// Calculates and returns the length of the supplied string
		//
		// @param str
		// @return
		Vector2 GetSizeOfString(const playstate::string& str) const;

		//
		// Calculates and returns the length of the supplied string. If the string is longer than @{code uint32 maxLength} 
		//	then it will put the new characters on a new line.
		//
		// @param str
		// @param maxLength
		// @return
		Vector2 GetSizeOfString(const playstate::string& str, uint32 maxLength) const;

		//
		// @return Information aboiut the supplied character. 
		//	Returns a "[]" character if the supplied character wasn't one of the loaded ones.
		const FontCharInfo& GetFontCharInfo(playstate::character c) const;

		//
		// Update this font so that the supplied characters are loaded into the font memory.
		//
		// @param characters The characters that we want to load into memory
		void Update(const playstate::string& characters);

	private:
		FontCharInfoMap mInfo;
		FontCharInfo mCharInfoNotFound;
	};

	//
	// Script integration
	//

	extern int Font_Load(lua_State* L);
	static luaL_Reg Font_Methods[] = {
		{ "Load", Font_Load },
		{ NULL, NULL }
	};
}
