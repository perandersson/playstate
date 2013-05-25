#include "../memory/memory.h"
#include "render_processor_factory.h"

template<> playstate::IRenderProcessorFactory* playstate::Singleton<playstate::IRenderProcessorFactory>::gSingleton = NULL;
