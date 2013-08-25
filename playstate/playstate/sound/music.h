#pragma once

#include "../resources/resource_object.h"
#include "../script/scriptable.h"
#include "../linked_list.h"
#include <AL/al.h>

namespace playstate
{
	class ISoundEngine;

	//
	// Class responsible for playing music. 
	class Music : public ResourceObject, public Scriptable
	{
	public:
		Music();
		~Music();

	private:
		ALuint mBuffers[3];
	};
}
