#include "../memory/memory.h"
#include "timer_factory.h"

template<> playstate::ITimerFactory* playstate::Singleton<playstate::ITimerFactory>::gSingleton = NULL;
