#include "VulkanShader.h"

#include "log\Log.h"

namespace Quartz
{
	VulkanShader::VulkanShader(VulkanDevice& device, const Array<Byte>& shaderCode, const char* entryName)
		: VulkanShader()
	{
		mParentDevice = &device;

		if (entryName == nullptr)
		{
			Log.Error("Failed to create shader: no shader entry name specified!");
			return;
		}

		mEntryName = entryName;

		VkShaderModuleCreateInfo shaderInfo{};
		shaderInfo.sType	= VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shaderInfo.codeSize = shaderCode.Size();
		shaderInfo.pCode	= reinterpret_cast<const uint32_t*>(shaderCode.Data());

		if (vkCreateShaderModule(device.GetDeviceHandle(), &shaderInfo, nullptr, &mShaderModule) != VK_SUCCESS)
		{
			Log.Error("Failed to create shader: vkCreateShaderModule failed!");
			return;
		}

		mIsValidShader = true;
	}

	void VulkanShader::Destroy()
	{
		vkDestroyShaderModule(mParentDevice->GetDeviceHandle(), mShaderModule, nullptr);
		mIsValidShader = false;
	}
}

