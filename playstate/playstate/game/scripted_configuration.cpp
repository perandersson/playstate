#include "../memory/memory.h"
#include "scripted_configuration.h"
#include "../functions.h"
using namespace playstate;

template<> playstate::IConfiguration* playstate::Singleton<playstate::IConfiguration>::gSingleton = NULL;

ScriptedConfiguration::ScriptedConfiguration(ScriptCollection* collection)
	: mScriptCollection(collection)
{
}

ScriptedConfiguration::~ScriptedConfiguration()
{
	delete mScriptCollection;
	mScriptCollection = NULL;
}

int32 ScriptedConfiguration::FindInt(const char* key, int32 defaultVal) const
{
	return mScriptCollection->FindInt(key, defaultVal);
}

float32 ScriptedConfiguration::FindFloat(const char* key, float32 defaultVal) const
{
	return mScriptCollection->FindFloat(key, defaultVal);
}

bool ScriptedConfiguration::FindBool(const char* key, bool defaultVal) const
{
	return mScriptCollection->FindBool(key, defaultVal);
}

playstate::string ScriptedConfiguration::FindString(const char* key, const char* defaultVal) const
{
	return mScriptCollection->FindString(key, defaultVal);
}
