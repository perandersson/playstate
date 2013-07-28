#pragma once

#include "../singleton.h"

namespace playstate
{
	//
	//
	class ILogger
	{
	public:
		virtual ~ILogger() {}

	public:
		virtual void Error(const char* msg, ...) = 0;
		virtual void Info(const char* msg, ...) = 0;
		virtual void Debug(const char* msg, ...) = 0;
	};

	//
	// 
	class ILoggerFactory : public Singleton<ILoggerFactory>
	{
	public:
		//
		// @return A logger based on the supplied name
		virtual ILogger& GetLogger(const char* name) = 0;
	};

}
