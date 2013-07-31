#include "../memory/memory.h"
#include "file_system.h"
using namespace playstate;

template<> playstate::IFileSystem* playstate::Singleton<playstate::IFileSystem>::gSingleton = NULL;
