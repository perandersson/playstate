#include "../memory/memory.h"
#include "sound_effect.h"
#include "../resources/resource_manager.h"
#include "../script/luam.h"
using namespace playstate;

SoundEffect::SoundEffect(SoundFormat::Enum format, float32 duration)
	: ResourceObject(), mFormat(format), mDuration(duration)
{
}

SoundEffect::~SoundEffect()
{
}

int playstate::SoundEffect_Load(lua_State* L)
{
	playstate::string path = lua_tostring(L, -1); lua_pop(L, 1);
	Resource<SoundEffect> effect = IResourceManager::Get().GetResource<SoundEffect>(path);
	luaM_pushresource(L, effect.GetResourceData());
	return 1;
}

int playstate::SoundEffect_GetDuration(lua_State* L)
{
	if(lua_gettop(L) < 1) {
		luaM_printerror(L, "Expected: SoundEffect.GetDuration(effect)");
		lua_pushnumber(L, 0.0);
		return 1;
	}

	Resource<SoundEffect> effect = luaM_popresource<SoundEffect>(L);
	if(effect.IsNotNull()) {
		lua_pushnumber(L, effect->GetDuration());
	} else {
		luaM_printerror(L, "Expected: SoundEffect.GetDuration(effect)");
		lua_pushnumber(L, 0.0);
	}
	return 1;
}
