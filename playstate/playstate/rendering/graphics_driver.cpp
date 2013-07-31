#include "../memory/memory.h"
#include "graphics_driver.h"
using namespace playstate;

template<> playstate::IGraphicsDriver* playstate::Singleton<playstate::IGraphicsDriver>::gSingleton = NULL;
