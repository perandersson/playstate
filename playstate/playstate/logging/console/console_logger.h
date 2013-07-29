#pragma once
#include "../logger.h"
#include "../../types.h"

namespace playstate
{
	class ConsoleLogger : public ILogger
	{
	public:
		ConsoleLogger();
		virtual ~ConsoleLogger();

	// ILogger
	public:
		virtual void Error(const char* msg, ...);
		virtual void Info(const char* msg, ...);
		virtual void Debug(const char* msg, ...);
	};
}
