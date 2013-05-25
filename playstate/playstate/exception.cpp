#include "memory/memory.h"
#include "exception.h"

#include <stdio.h>
#include <stdarg.h>

using playstate::Exception;
using playstate::uint64;

Exception::Exception(const char* function, const long line, const char* file)
	: mFunction(function), mLine(line), mFile(file)
{
	std::string::size_type index = mFile.find(std::string("\\playstate\\"));
	if(index == std::string::npos) {
		index = mFile.find(std::string("/playstate/"));
	}

	mFile = mFile.substr(index);
}

Exception::~Exception()
{
}

void Exception::SetMessage(const char* format, va_list argp)
{
	char tmp[5096];
    vsprintf_s(tmp, 5096, format, argp);
	mMessage = tmp;
}

const std::string& Exception::GetMessage() const
{
	return mMessage;
}

const std::string& Exception::GetFunction() const
{
	return mFunction;
}

const std::string& Exception::GetFile() const
{
	return mFile;
}

const uint64 Exception::GetLine() const
{
	return mLine;
}
