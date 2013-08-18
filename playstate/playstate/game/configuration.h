#pragma once
#include "../singleton.h"
#include "../types.h"

namespace playstate
{
	//
	// Service used reading configuration properties into various forms.
	//
	// Example: {@code
	//  const int32 defaultValue = 1024;
	//	IConfiguration::Get().FindInt("window.width", defaultValue);
	// }
	class IConfiguration : public Singleton<IConfiguration>
	{
	public:
		//
		// Locates and returns the supplied key's value. If the value isn't found or not an int type then
		// return the default value.
		// @param key
		// @param defaultValue
		virtual int32 FindInt(const char* key, int32 defaultVal = 0) const = 0;

		virtual float32 FindFloat(const char* key, float32 defaultVal = 0.0f) const  = 0;

		virtual bool FindBool(const char* key, bool defaultVal = false) const = 0;

		virtual playstate::string FindString(const char* key, const char* defaultVal = "") const = 0;
	};
}
