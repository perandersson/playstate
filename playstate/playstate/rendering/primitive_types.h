#pragma once
#include "../types.h"

namespace playstate
{
	//
	// 
	class PrimitiveType
	{
	public:
		PrimitiveType(uint32 elementCount, uint32 innerType);
		~PrimitiveType();

		inline uint32 GetElementCount() const {
			return mElementCount;
		}

		inline uint32 GetInnerType() const {
			return mInnerType;
		}

	private:
		uint32 mElementCount;
		uint32 mInnerType;

	public:
		static const PrimitiveType Triangle;
		static const PrimitiveType LineLoop;
	};

#ifndef PrimitiveTypeEnum
#define PrimitiveTypeEnum const PrimitiveType&
#endif

}
