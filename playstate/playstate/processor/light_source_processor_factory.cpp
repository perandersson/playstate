#include "../memory/memory.h"
#include "light_source_processor_factory.h"

template<> playstate::ILightSourceProcessorFactory* playstate::Singleton<playstate::ILightSourceProcessorFactory>::gSingleton = NULL;
