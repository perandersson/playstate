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
		virtual void Error(const playstate::character* msg, ...);
		virtual void Info(const playstate::character* msg, ...);
		virtual void Debug(const playstate::character* msg, ...);
		virtual void Warn(const playstate::character* msg, ...);
	};
}
