#pragma once
#include "configuration.h"
#include "../script/script_collection.h"

namespace playstate
{
	class ScriptedConfiguration : public IConfiguration
	{
	public:
		ScriptedConfiguration(ScriptCollection* collection);
		~ScriptedConfiguration();

	// IConfiguration
	public:
		virtual int32 FindInt(const char* key, int32 defaultVal = 0) const;
		virtual float32 FindFloat(const char* key, float32 defaultVal = 0.0f) const;
		virtual bool FindBool(const char* key, bool defaultVal = false) const;
		virtual playstate::string FindString(const char* key, const char* defaultVal = "") const;

	private:
		ScriptCollection* mScriptCollection;
	};
}

