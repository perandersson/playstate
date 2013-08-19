#pragma once

#include "../types.h"

namespace playstate
{
	//
	// Base class for a scripted lambda expression. This is used so that we can allow users to supply lambda expressions
	// to the game engine, instead of instances of classes.
	//
	// {@code
	//   local add = function(a, b) 
	//      return a + b
	//   end
	//   myObject:SetAdd(add)
	//
	//   -- or you can do --
	//
	//   myObject:SetAdd(function(a, b)
	//      return a + b
	//   end)
	// }
	//
	// You can then use it inside compiled C++ code using:
	//
	// {@code
	//   Lambda add;
	//   uint32 result = add.Invoke<uint32>(10, 20); // returns 30
	// }
	// @todo IMPLEMENT THIS
	class Lambda
	{
	public:
		template<typename R>
		R Invoke() {
		}

		template<typename R, typename P1>
		R Invoke(P1 p1) {
		}

		template<typename R, typename P1, typename P2>
		R Invoke(P1 p1, P2 p2) {
		}

		template<typename R, typename P1, typename P2, typename P3>
		R Invoke(P1 p1, P2 p2, P3 p3) {
		}
	};
}
