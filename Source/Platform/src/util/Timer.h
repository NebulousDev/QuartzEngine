#pragma once

#include "Common.h"

namespace Quartz
{
	class QUARTZ_API Timer
	{
	private:
		Time64 mStartTime;
		Time64 mLastTime;

	public:
		Timer();

		Time64 Start();
		Time64 Reset();
	
		Time64 GetStartTime();
		Time64 GetLapsedTime();
		Time64 GetLastTime();
	};
}