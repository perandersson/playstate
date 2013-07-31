#include "../memory/memory.h"
#include "logger.h"
using namespace playstate;

template<> playstate::ILogger* playstate::Singleton<playstate::ILogger>::gSingleton = NULL;
