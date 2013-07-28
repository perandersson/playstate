#pragma once
#include "../logger_factory.h"
#include "../../types.h"

namespace playstate
{
	class ConsoleLogger : public ILogger
	{
	public:
		ConsoleLogger(const std::string& name);
		virtual ~ConsoleLogger();

	// ILogger
	public:
		virtual void Error(const char* msg, ...);
		virtual void Info(const char* msg, ...);
		virtual void Debug(const char* msg, ...);

	private:
		std::string mName;
	};

	class ConsoleLoggerFactory : public ILoggerFactory
	{
		typedef std::hash_map<std::string, ILogger*> Loggers;

	public:
		ConsoleLoggerFactory();
		virtual ~ConsoleLoggerFactory();

	// ILoggerFactory
	public:
		virtual ILogger& GetLogger(const char* name);

	private:
		Loggers mLoggers;
	};
}
