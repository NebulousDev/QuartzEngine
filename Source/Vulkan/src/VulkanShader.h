#pragma once

#include "Common.h"
#include "VulkanDevice.h"
#include "graphics\GFXShaders.h"
#include "util\Array.h"

#include <vulkan\vulkan.h>

namespace Quartz
{
	class QUARTZ_API VulkanShader
	{
	private:
		VulkanDevice*			mParentDevice;
		VkShaderModule			mShaderModule;
		const char*				mEntryName;
		Bool8					mIsValidShader;

	public:
		VulkanShader() = default;
		VulkanShader(VulkanDevice& device, const Array<Byte>& shaderCode, const char* entryName);

		VulkanDevice& GetParentDevice() { return *mParentDevice; }
		VkShaderModule GetShaderModuleHandle() { return mShaderModule; }
		const char* GetEntryName() const { return mEntryName; }
		Bool8 IsValidShader() const { return mIsValidShader; }

		void Destroy();
	};

	class QUARTZ_API VulkanVertexShader : public GFXVertexShader, public VulkanShader
	{
	public:
		VulkanVertexShader(VulkanDevice& device, const Array<Byte>& shaderCode, const char* entryName)
			: VulkanShader(device, shaderCode, entryName) {}
	};

	class QUARTZ_API VulkanPixelShader : public GFXPixelShader, public VulkanShader
	{
	public:
		VulkanPixelShader(VulkanDevice& device, const Array<Byte>& shaderCode, const char* entryName)
			: VulkanShader(device, shaderCode, entryName) {}
	};
}