#pragma once

#include "../resources/resource_object.h"
#include "../script/scriptable.h"
#include "../linked_list.h"
#include "../types.h"

namespace playstate
{
	class ISoundEngine;

	//
	// Class which represents a music resource in the game engine. 
	class Music : public ResourceObject, public Scriptable
	{
	public:
		Music(float32 duration);
		virtual ~Music();

		//
		// @return The duration of this music resource
		inline float32 GetDuration() const {
			return mDuration;
		}

	private:
		float32 mDuration;
	};
}
