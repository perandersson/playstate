#include "memory/memory.h"
#include "functions.h"

#include <sstream>

namespace playstate
{
	std::vector<playstate::string>& Split(const playstate::string& str, const char delim, std::vector<playstate::string>& elems) {
		playstate::stringstream ss(str);
		playstate::string item;
		while(std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}

	std::vector<playstate::string> Split(const playstate::string& str, const char delim) {
		std::vector<playstate::string> elems;
		return Split(str, delim, elems);
	}

	playstate::string ReplaceString(const playstate::string& value, const char replace, const playstate::string& newval)
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
	
#ifdef _UNICODE
	std::vector<std::wstring>& Split(const std::wstring& str, const wchar_t delim, std::vector<std::wstring>& elems) {
		std::wstringstream ss(str);
		std::wstring item;
		while(std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}

	std::vector<std::wstring> Split(const std::wstring& str, const wchar_t delim) {
		std::vector<std::wstring> elems;
		return Split(str, delim, elems);
	}
#endif
}
