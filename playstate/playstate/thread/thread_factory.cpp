#include "../memory/memory.h"
#include "thread_factory.h"
using namespace playstate;

template<> playstate::IThreadFactory* playstate::Singleton<playstate::IThreadFactory>::gSingleton = NULL;
