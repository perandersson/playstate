#include "memory/memory.h"
#include "exception.h"

#include <stdio.h>
#include <stdarg.h>

using playstate::Exception;
using playstate::uint64;

Exception::Exception(const char* function, const long line, const char* file)
	: mFunction(function), mLine(line), mFile(file)
{
	playstate::string::size_type index = mFile.find(playstate::string("\\playstate\\"));
	if(index == playstate::string::npos) {
		index = mFile.find(playstate::string("/playstate/"));
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

const playstate::string& Exception::GetMessage() const
{
	return mMessage;
}

const playstate::string& Exception::GetFunction() const
{
	return mFunction;
}

const playstate::string& Exception::GetFile() const
{
	return mFile;
}

const uint64 Exception::GetLine() const
{
	return mLine;
}
