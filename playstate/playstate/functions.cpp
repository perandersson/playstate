#include "memory/memory.h"
#include "functions.h"

#include <sstream>

namespace playstate
{
	std::vector<std::string>& Split(const std::string& str, const char delim, std::vector<std::string>& elems) {
		std::stringstream ss(str);
		std::string item;
		while(std::getline(ss, item, delim)) {
			elems.push_back(item);
		}
		return elems;
	}

	std::vector<std::string> Split(const std::string& str, const char delim) {
		std::vector<std::string> elems;
		return Split(str, delim, elems);
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
