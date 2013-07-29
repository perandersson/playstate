#pragma once

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
}
