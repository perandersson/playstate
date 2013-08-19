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
	std::vector<playstate::string> Split(const playstate::string& str, const playstate::character delim);

	//
	// Replaces a supplied string with a new one.
	playstate::string ReplaceString(const playstate::string& value, const playstate::character replace, const playstate::string& newval);
}

