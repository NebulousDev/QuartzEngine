#pragma once

#include "Common.h"
#include "util/String.h"

namespace Quartz
{
	struct ModuleInfo
	{
		StringW name;
	};

	class QUARTZ_API Module
	{
	private:
		ModuleInfo mModuleInfo;

	protected:
		Module(const ModuleInfo& info);

	public:
		virtual Bool8 PreInit() { return true; };
		virtual Bool8 Init() { return true; };
		virtual Bool8 PostInit() { return true; };

		virtual void Update(Float32 delta) {};
		virtual void Tick(UInt32 ticks) {};

		virtual void PreShutdown() {};
		virtual void Shutdown() {};

		const StringW& GetModuleName() const { return mModuleInfo.name; }
	};
}