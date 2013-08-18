#pragma once

#include <vector>
#include "types.h"

namespace playstate
{
	// 
	// Split a string and return a vector containing all it's parts
	//
	// @param str
	// @param delim
	// @return 
	std::vector<playstate::string> Split(const playstate::string& str, const char delim);

	//
	// Replaces a supplied string with a new one.
	playstate::string ReplaceString(const playstate::string& value, const char replace, const playstate::string& newval);
	
#ifdef _UNICODE
	// 
	// Split a string and return a vector containing all it's parts
	// @param str
	// @param delim
	// @return 
	std::vector<std::wstring> Split(const std::wstring& str, const wchar_t delim);
#endif
}

