#pragma once
#include <playstate/kernel.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>

namespace playstate
{
	namespace win32
	{
		//
		// The default kernel for the windows version of this game engine. Use this unless you are absolutely sure what you are doing
		class Win32DefaultKernel : public IKernel
		{
		public:
			Win32DefaultKernel(HINSTANCE app, const std::string& fileSystemPath);
			virtual ~Win32DefaultKernel();

		// IKernel
		public:
			virtual void Initialize();
			virtual void Release();

		private:
			HINSTANCE mAppInstance;
		};
	}
}
