#pragma once

#include "Common.h"
#include "GFXResource.h"
#include "util\String.h"

namespace Quartz
{
	class QUARTZ_API GFXPhysicalDevice : public GFXResource
	{
	protected:
		String	mDeviceName;
		String	mVendorName;

		Bool8	mIsDescrete;

	protected:
		GFXPhysicalDevice() = default;

	public:
		const String& GetDeviceName() const { return mDeviceName; }
		const String& GetVendorName() const { return mVendorName; }

		Bool8 IsDiscrete() const { return mIsDescrete; }
	};
}