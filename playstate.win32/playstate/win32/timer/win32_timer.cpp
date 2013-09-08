#include <playstate/memory/memory.h>
#include "win32_timer.h"
using namespace playstate;
using namespace playstate::win32;

Win32Timer::Win32Timer()
	: mTimeSinceLastUpdate(0), mTotalTime(0), mFrequency(0), mLastTime(0)
{
	LARGE_INTEGER performanceFrequency;
	QueryPerformanceFrequency(&performanceFrequency);
	mFrequency = (double)(performanceFrequency.QuadPart);
}

Win32Timer::~Win32Timer()
{
}

void Win32Timer::Start()
{
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	mLastTime = now.QuadPart;
	mStartTime = now.QuadPart;
}

float32 Win32Timer::GetElapsedAndRestart()
{
	if(mLastTime <= 0) {
		Start();
	}

	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	
	LONGLONG quadPartDelta = now.QuadPart - mLastTime;
	double dt = double(quadPartDelta) / mFrequency;
	mTimeSinceLastUpdate = (float)(dt);
	mLastTime = now.QuadPart;

	return mTimeSinceLastUpdate;
}

float64 Win32Timer::GetTotalTime()
{
	return mTotalTime;
}

void Win32Timer::Stop()
{
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	
	LONGLONG quadPartDelta = now.QuadPart - mStartTime;
	double dt = double(quadPartDelta) / mFrequency;
	mTotalTime = dt;
	mLastTime = 0;
	mStartTime = 0;
}
