#include "../memory/memory.h"
#include "render_system.h"
using namespace playstate;

template<> playstate::IRenderSystem* playstate::Singleton<playstate::IRenderSystem>::gSingleton = NULL;
