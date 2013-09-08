#pragma once

#include <playstate/timer/timer.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace playstate
{
	namespace win32
	{
		class Win32Timer : public ITimer
		{
		public:
			Win32Timer();
			virtual ~Win32Timer();

		public:
			virtual void Start();
			virtual float32 GetElapsedAndRestart();
			virtual float64 GetTotalTime();
			virtual void Stop();

		private:
			float64 mFrequency;
			float32 mTimeSinceLastUpdate;
			float64 mTotalTime;
			LONGLONG mLastTime;			
			LONGLONG mStartTime;
		};
	}
}
