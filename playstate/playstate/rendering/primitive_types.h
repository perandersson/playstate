#pragma once
#include "../types.h"
#include <gl/glew.h>

namespace playstate
{
	//
	// 
	class PrimitiveType
	{
	public:
		PrimitiveType(uint32 elementCount, GLenum type);
		~PrimitiveType();

		inline uint32 GetElementCount() const {
			return mElementCount;
		}

		inline uint32 GetType() const {
			return mType;
		}

	private:
		uint32 mElementCount;
		GLenum mType;

	public:
		static const PrimitiveType Point;
		static const PrimitiveType Triangle;
		static const PrimitiveType LineLoop;
	};

#ifndef PrimitiveTypeEnum
#define PrimitiveTypeEnum const PrimitiveType&
#endif

}
