#include "../memory/memory.h"
#include "font.h"
#include "../resources/resource_manager.h"
using namespace playstate;

Font::Font(ITexture2D* texture, const FontCharInfoMap& infoMap, float32 spaceWidth, float32 lineHeight) 
	: mTexture(texture), mInfo(infoMap), mSpaceWidth(spaceWidth), mLineHeight(lineHeight)
{
	assert(texture != NULL && "You are not allowed to create a NULL texture font");
}

Font::~Font()
{
	FontCharInfoMap::iterator it = mInfo.begin();
	FontCharInfoMap::iterator end = mInfo.end();
	for(; it != end; ++it) {
		delete it->second;
	}

	delete mTexture;
	mTexture = NULL;
}

Vector2 Font::GetSizeOfString(const playstate::string& str) const
{
	float32 currentWidth = 0.f;
	Vector2 totalSize;
	playstate::string::size_type numChars = str.length();
	for(playstate::string::size_type i = 0; i < numChars; ++i) {
		playstate::character c = str[i];
		if(c == '\n') {
			totalSize.Y += mLineHeight;
			currentWidth = 0;
			continue;
		} else if(c == '\r') {
			continue; // ignore
		} else if(c == ' ') {
			totalSize.X += mSpaceWidth;
			continue;
		}
		const FontCharInfo& info = GetFontCharInfo(c);
		
		currentWidth += info.Size.Y + info.Offset.X;
		if(totalSize.X < currentWidth)
			totalSize.X = currentWidth;
	}

	return totalSize;
}

Vector2 Font::GetSizeOfString(const playstate::string& str, uint32 maxLength) const
{
	assert(false && "NOT IMPLEMENTED");
	return Vector2::Zero;
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
