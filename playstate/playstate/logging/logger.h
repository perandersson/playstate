#pragma once
#include "../singleton.h"

namespace playstate
{
	//
	//
	class ILogger : public Singleton<ILogger>
	{
	public:
		virtual void Error(const char* msg, ...) = 0;
		virtual void Info(const char* msg, ...) = 0;
		virtual void Debug(const char* msg, ...) = 0;
	};
}
