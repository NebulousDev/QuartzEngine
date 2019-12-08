#pragma once

#include "util\Singleton.h"
#include "platform\System.h"

namespace Quartz
{
	class QUARTZ_API Engine : public Singleton<Engine>
	{
	public:
		friend class Singleton<Engine>;

	private:
		Bool8 mInitialized;
		Bool8 mRunning;

		Time64 mDeltaTime;

		Engine() = default;

	public:
		void Start();
		void Stop();

		void Update();

		FORCE_INLINE Time64 GetDeltaTime() { return mDeltaTime; }
		FORCE_INLINE Time64 GetUptime() { return System::GetUptimeNanoseconds(); }

		FORCE_INLINE Bool8 IsInitialized() { return mInitialized; }
		FORCE_INLINE Bool8 IsRunning() { return mRunning; }
	};
}