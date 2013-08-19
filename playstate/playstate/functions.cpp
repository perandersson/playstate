#include "memory/memory.h"
#include "functions.h"
#include <sstream>

namespace playstate
{
	std::vector<playstate::string>& Split(const playstate::string& str, const playstate::character delim, std::vector<playstate::string>& elems) {
		playstate::stringstream ss(str);
		playstate::string item;
		while(std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}

	std::vector<playstate::string> Split(const playstate::string& str, const playstate::character delim) {
		std::vector<playstate::string> elems;
		return Split(str, delim, elems);
	}

	playstate::string ReplaceString(const playstate::string& value, const playstate::character replace, const playstate::string& newval)
	{
		playstate::string result;
		for(size_t i = 0; i < value.size(); ++i) {
			if(value[i] == replace) {
				result += newval;
			} else {
				result += value[i];
			}
		}
		return result;
	}
}
