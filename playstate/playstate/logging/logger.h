#pragma once
#include "../singleton.h"
#include "../types.h"

namespace playstate
{
	//
	//
	class ILogger : public Singleton<ILogger>
	{
	public:
		virtual void Error(const playstate::character* msg, ...) = 0;
		virtual void Info(const playstate::character* msg, ...) = 0;
		virtual void Debug(const playstate::character* msg, ...) = 0;
	};
}
