#include "../../memory/memory.h"
#include "console_logger.h"
#include <iostream>
#include <stdarg.h>
using namespace playstate;

ConsoleLogger::ConsoleLogger()
{
}

ConsoleLogger::~ConsoleLogger()
{
}

void ConsoleLogger::Error(const char* msg, ...)
{
	va_list arglist;
    va_start(arglist, msg);
	char tmp[5096];
    vsprintf_s(tmp, 5096, msg, arglist);
    va_end(arglist);

	std::cout << "[ERROR] " << tmp << std::endl;
}

void ConsoleLogger::Info(const char* msg, ...)
{
	va_list arglist;
    va_start(arglist, msg);
	char tmp[5096];
    vsprintf_s(tmp, 5096, msg, arglist);
    va_end(arglist);

	std::cout << "[INFO] " << tmp << std::endl;
}

void ConsoleLogger::Debug(const char* msg, ...)
{
	va_list arglist;
    va_start(arglist, msg);
	char tmp[5096];
    vsprintf_s(tmp, 5096, msg, arglist);
    va_end(arglist);
	
	std::cout << "[DEBUG] " << tmp << std::endl;
}
