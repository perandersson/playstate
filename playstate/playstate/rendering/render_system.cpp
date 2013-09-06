#include "../memory/memory.h"
#include "render_system.h"
using namespace playstate;

template<> playstate::RenderSystem* playstate::Singleton<playstate::RenderSystem>::gSingleton = NULL;
