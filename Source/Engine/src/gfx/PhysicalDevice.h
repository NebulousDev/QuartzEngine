#pragma once

#include "Common.h"
#include "util\Castable.h"
#include "util\String.h"

namespace Quartz
{
	class QUARTZ_API PhysicalDevice : public Castable
	{
	protected:
		String	mDeviceName;
		String	mVendorName;

		Bool8	mIsDescrete;

	protected:
		PhysicalDevice() = default;

	public:
		const String& GetDeviceName() const { return mDeviceName; }
		const String& GetVendorName() const { return mVendorName; }

		Bool8 IsDiscrete() const { return mIsDescrete; }
	};
}