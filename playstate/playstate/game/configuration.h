#pragma once
#include "../singleton.h"
#include "../types.h"

namespace playstate
{
	class IConfiguration : public Singleton<IConfiguration>
	{
	public:
		virtual ~IConfiguration() {}

	public:
		virtual int32 FindInt(const char* key, int32 defaultVal = 0) const = 0;
		virtual float32 FindFloat(const char* key, float32 defaultVal = 0.0f) const  = 0;
		virtual bool FindBool(const char* key, bool defaultVal = false) const = 0;
		virtual std::string FindString(const char* key, const char* defaultVal = "") const = 0;
	};
}
