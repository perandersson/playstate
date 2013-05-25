#pragma once

#include <vector>
#include <string>

namespace playstate
{
	// 
	// Split a string and return a vector containing all it's parts
	// @param str
	// @param delim
	// @return 
	std::vector<std::string> Split(const std::string& str, const char delim);
	
#ifdef _UNICODE
	// 
	// Split a string and return a vector containing all it's parts
	// @param str
	// @param delim
	// @return 
	std::vector<std::wstring> Split(const std::wstring& str, const wchar_t delim);
#endif
}

