#pragma once
#include "../types.h"

namespace playstate
{
	class Math
	{
	public:
		//
		// Perform linear interpolation between v0 and v1
		//
		// @param v0 The first value
		// @param v1 The second value
		// @param t Delta between [0, 1]
		static float32 Lerp(float32 v0, float32 v1, float32 t);

		//
		// Calculates the maximum value of the two supplied values
		template<typename T>
		static T Max(T val1, T val2) {
			return val1 > val2 ? val1 : val2;
		}

		//
		// Calculates the minimum value of the two supplied values
		template<typename T>
		static T Min(T val1, T val2) {
			return val1 < val2 ? val1 : val2;
		}
	};
}
