#include "memory/memory.h"
#include "kernel.h"
using namespace playstate;

template<> playstate::IKernel* playstate::Singleton<playstate::IKernel>::gSingleton = NULL;
