#include "../memory/memory.h"
#include "font.h"
#include "../resources/resource_manager.h"
using namespace playstate;

Font::Font(GLuint textureId, uint32 width, uint32 height, const FontCharInfoMap& infoMap) 
	: Texture2D(textureId, width, height, TextureFormat::R), mInfo(infoMap)
{
}

Font::~Font()
{
	FontCharInfoMap::iterator it = mInfo.begin();
	FontCharInfoMap::iterator end = mInfo.end();
	for(; it != end; ++it) {
		delete it->second;
	}
}

Vector2 Font::GetSizeOfString(const playstate::string& str) const
{
	Vector2 totalSize;
	playstate::string::size_type numChars = str.length();
	for(playstate::string::size_type i = 0; i < numChars; ++i) {
		playstate::character c = str[i];
		if(c == '\n') {
			// New line
			continue;
		} else if(c == '\r') {
			continue; // ignore
		}
		const FontCharInfo& info = GetFontCharInfo(c);

		float32 width = info.Size.Y + info.Offset.X;
		float32 height = info.Size.Y + info.Offset.Y;

		totalSize.X += width;
		if(totalSize.Y < height) {
			totalSize.Y = height;
		}
	}

	return totalSize;
}

Vector2 Font::GetSizeOfString(const playstate::string& str, uint32 maxLength) const
{
	Vector2 totalSize;
	playstate::string::size_type numChars = str.length();
	for(playstate::string::size_type i = 0; i < numChars; ++i) {
		playstate::character c = str[i];
		if(c == '\n') {
			// New line
			continue;
		} else if(c == '\r') {
			continue; // ignore
		}
		const FontCharInfo& info = GetFontCharInfo(c);

		float32 width = info.Size.Y + info.Offset.X;
		float32 height = info.Size.Y + info.Offset.Y;

		// Have we reached the maximum length?
		if((uint32)(totalSize.X + width + 0.5f) > maxLength) {
			// new line
		} else {
			totalSize.X += width;
		}

		if(totalSize.Y < height) {
			totalSize.Y = height;
		}
	}

	return totalSize;
}

const FontCharInfo& Font::GetFontCharInfo(playstate::character c) const
{
	FontCharInfoMap::const_iterator it = mInfo.find(c);
	if(it == mInfo.end()) {
		return mCharInfoNotFound;
	}
	return *it->second;
}


void Font::Update(const playstate::string& characters)
{
	assert(false && "NOT IMPLEMENTED");
}

int playstate::Font_Load(lua_State* L)
{
	playstate::string path = lua_tostring(L, -1); lua_pop(L, 1);
	Resource<Font> font = IResourceManager::Get().GetResource<Font>(path);
	luaM_pushresource(L, font.GetResourceData());
	return 1;
}
