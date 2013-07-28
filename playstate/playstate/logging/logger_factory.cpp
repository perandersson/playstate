#include "../memory/memory.h"
#include "logger_factory.h"
using namespace playstate;

template<> playstate::ILoggerFactory* playstate::Singleton<playstate::ILoggerFactory>::gSingleton = NULL;
