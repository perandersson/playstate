#pragma once

#include <stdarg.h>
#include "types.h"

#define THROW_EXCEPTION(E, Message, ...) throw E(__FUNCTION__, __LINE__, __FILE__, Message, __VA_ARGS__)

namespace playstate
{
	//
	// Base class for all exception in the playstate game engine.
	class Exception
	{
	public:
		Exception(const char* function, const long line, const char* file);
		virtual ~Exception();

		//
		// @return The exception message
		const playstate::string& GetMessage() const;

		//
		// @return The function where the exception originated
		const playstate::string& GetFunction() const;

		//
		// @return The file where the exception originated
		const playstate::string& GetFile() const;

		//
		// @return The line where the exception originated
		const uint64 GetLine() const;

	protected:
		void SetMessage(const char* format, va_list argp);

	private:
		playstate::string mFunction;
		playstate::string mFile;
		playstate::string mMessage;
		uint64 mLine;
	};
}
