#include "../memory/memory.h"
#include "resource_manager.h"
using namespace playstate;

template<> playstate::IResourceManager* playstate::Singleton<playstate::IResourceManager>::gSingleton = NULL;

