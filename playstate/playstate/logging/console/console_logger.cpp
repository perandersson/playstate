#include "../../memory/memory.h"
#include "console_logger.h"
#include <iostream>
#include <stdarg.h>
using namespace playstate;

ConsoleLogger::ConsoleLogger(const std::string& name)
	: mName(name)
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

	std::cout << "[ERROR " << mName << "] " << tmp << std::endl;
}

void ConsoleLogger::Info(const char* msg, ...)
{
	va_list arglist;
    va_start(arglist, msg);
	char tmp[5096];
    vsprintf_s(tmp, 5096, msg, arglist);
    va_end(arglist);

	std::cout << "[INFO " << mName << "] " << tmp << std::endl;
}

void ConsoleLogger::Debug(const char* msg, ...)
{
	va_list arglist;
    va_start(arglist, msg);
	char tmp[5096];
    vsprintf_s(tmp, 5096, msg, arglist);
    va_end(arglist);
	
	std::cout << "[DEBUG " << mName << "] " << tmp << std::endl;
}

//////////////////////////////////////////

ConsoleLoggerFactory::ConsoleLoggerFactory()
{
}

ConsoleLoggerFactory::~ConsoleLoggerFactory()
{
	Loggers::iterator it = mLoggers.begin();
	Loggers::iterator end = mLoggers.end();
	for(; it != end; ++it) {
		delete it->second;
	}
}

ILogger& ConsoleLoggerFactory::GetLogger(const char* name)
{
	std::string sName(name);
	Loggers::iterator it = mLoggers.find(sName);
	if(it != mLoggers.end()) {
		return *it->second;
	}

	ConsoleLogger* logger = new ConsoleLogger(sName);
	mLoggers.insert(std::make_pair(sName, logger));
	return *logger;
}
