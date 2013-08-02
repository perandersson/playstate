#pragma once
#include "singleton.h"

namespace playstate
{
	//
	// 
	class IKernel : public Singleton<IKernel>
	{
	public:
		//
		// Initializes the game engine kernel. Used to create the neccessary service dependencies, such as the script engine.
		// @throw playstate::Exception If the initialization of the kernel failed.
		virtual void Initialize() = 0;

		//
		// Releases this kernels internal resources.
		virtual void Release() = 0;

		//
		// Process the kernels internal data
		virtual void Process() = 0;

		//
		// Method called during the script registration process
		virtual void RegisterScript() = 0;
	};
}
