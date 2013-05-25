#include "../memory/memory.h"
#include "update_processor_factory.h"

template<> playstate::IUpdateProcessorFactory* playstate::Singleton<playstate::IUpdateProcessorFactory>::gSingleton = NULL;