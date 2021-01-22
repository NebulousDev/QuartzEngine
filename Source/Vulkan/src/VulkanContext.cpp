#include "VulkanContext.h"

#include "log/Log.h"
#include "VulkanUtil.h"
#include "SPIRVUtil.h"

#include "Engine.h"

#include <cstdio>

namespace Quartz
{
	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT		messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT				messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		{
			Log::Error("%s\n", pCallbackData->pMessage);
		}
		else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
		{
			Log::Warning("%s\n", pCallbackData->pMessage);
		}
		else
		{
			//Log::Debug("%s\n", pCallbackData->pMessage);
		}

		return VK_FALSE;
	}

	static Bool8 EnumerateVkLayerProperties(Array<VkLayerProperties>& layerProperties)
	{
		VkResult result;
		UInt32 layerCount = 0;

		result = vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		layerProperties.Resize(layerCount);
		result = vkEnumerateInstanceLayerProperties(&layerCount, layerProperties.Data());

		return result == VK_SUCCESS;
	}

	static Bool8 EnumerateVkExtensionProperties(Array<VkExtensionProperties>& extensionProperties)
	{
		VkResult result;
		UInt32 extensionCount = 0;

		result = vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);
		extensionProperties.Resize(extensionCount);
		result = vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, extensionProperties.Data());

		return result == VK_SUCCESS;
	}

	static Bool8 EnumerateVkPhysicalDevices(VkInstance instance, Array<VkPhysicalDevice>& physicalDevices)
	{
		VkResult result;
		UInt32 physicalDeviceCount;

		result = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
		physicalDevices.Resize(physicalDeviceCount);
		result = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.Data());

		return result == VK_SUCCESS;
	}

	Bool8 VPLVulkanContext::InitInstance(const StringW& appName, const Array<String>& extensions,
		const Array<String>& validationLayers)
	{
		String appNameA = StringWToStringA(appName).Str();

		if (!EnumerateVkLayerProperties(mAvailableLayers))
		{
			Log::Warning("Failed to create a vulkan instance: Unable to enumerate validation layers!");
		}

		for (const String& layerName : validationLayers)
		{
			for (const VkLayerProperties& layerProp : mAvailableLayers)
			{
				if (layerName == layerProp.layerName)
				{
					mEnabledLayerNames.PushBack(layerName.Str());
					goto layerFound;
				}
			}

			Log::Warning("Attempted to enable unsupported vulkan instance layer [\'%s\']!", layerName.Str());

			layerFound:;
		}

		if (!EnumerateVkExtensionProperties(mAvailableExtensions))
		{
			Log::Warning("Failed to create a vulkan instance: Unable to enumerate extensions!");
		}

		for (const String& extName : extensions)
		{
			for (const VkExtensionProperties& extProp : mAvailableExtensions)
			{
				if (extName == extProp.extensionName)
				{
					mEnabledExtensionNames.PushBack(extName.Str());
					goto extFound;
				}
			}

			Log::Warning("Attempted to enable unsupported vulkan instance extension [\'%s\']!", extName.Str());

			extFound:;
		}

		VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo = {};
		debugMessengerInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugMessengerInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugMessengerInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
			| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugMessengerInfo.pfnUserCallback = DebugCallback;
		debugMessengerInfo.pUserData = NULL;

		VkApplicationInfo appInfo = {};
		appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName = appNameA.Str();
		appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName = "Quartz Engine";
		appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion = VK_API_VERSION_1_2; // NOTE: 1_2 is not yet needed

		VkInstanceCreateInfo createInfo = {};
		createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo = &appInfo;
		createInfo.enabledLayerCount = mEnabledLayerNames.Size();
		createInfo.ppEnabledLayerNames = mEnabledLayerNames.Data();
		createInfo.enabledExtensionCount = mEnabledExtensionNames.Size();
		createInfo.ppEnabledExtensionNames = mEnabledExtensionNames.Data();
		createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugMessengerInfo;

		if (vkCreateInstance(&createInfo, NULL, &mVkInstance) != VK_SUCCESS)
		{
			Log::Critical("Failed to create a vulkan instance: vkCreateInstance failed!");
			return false;
		}

		VkDebugUtilsMessengerEXT mDebugMessenger;
		PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>
			(vkGetInstanceProcAddr(mVkInstance, "vkCreateDebugUtilsMessengerEXT"));

		if (!vkCreateDebugUtilsMessengerEXT)
		{
			Log::Warning("Failed to address for \"PFN_vkCreateDebugUtilsMessengerEXT\"! No validation messages will be displayed!");
		}
		else if (vkCreateDebugUtilsMessengerEXT(mVkInstance, &debugMessengerInfo, NULL, &mDebugMessenger) != VK_SUCCESS)
		{
			Log::Warning("Failed to create validation debug messenger! No validation messages will be displayed!");
		}

		return true;
	}

	Bool8 VPLVulkanContext::InitDevices()
	{
		if (!EnumerateVkPhysicalDevices(mVkInstance, mAvailablePhysicalDevices))
		{
			Log::Error("Failed to initialize graphics device: vkEnumeratePhysicalDevices failed!");
			return false;
		}

		mPhysicalDevices.Resize(mAvailablePhysicalDevices.Size());

		Int32 maxDeviceScore = -1;
		VulkanPhysicalDevice* pPhyiscalDeviceCandidate = nullptr;

		Int32 deviceIndex = 0;
		for (const VkPhysicalDevice physicalDevice : mAvailablePhysicalDevices)
		{
			mPhysicalDevices[deviceIndex] = VulkanPhysicalDevice(physicalDevice);

			Int32 deviceScore = mPhysicalDevices[deviceIndex].GetDeviceScore();

			if (deviceScore > maxDeviceScore)
			{
				maxDeviceScore = deviceScore;
				pPhyiscalDeviceCandidate = &mPhysicalDevices[deviceIndex];
			}

			++deviceIndex;
		}

		if (pPhyiscalDeviceCandidate == nullptr)
		{
			Log::Critical("Failed to initialize graphics device: No suitable graphics adapter was detected!");
			return false;
		}

		Array<String> deviceExtensions;
		deviceExtensions.PushBack(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

		mpDevice = new VulkanDevice(pPhyiscalDeviceCandidate, deviceExtensions);

		if (!mpDevice->IsValidDevice())
		{
			Log::Critical("Failed to initialize graphics device: No valid device was created!");
			return false;
		}

		return true;
	}

	VPLVulkanContext::VPLVulkanContext() :
		mVkInstance(VK_NULL_HANDLE),
		mPhysicalDevices(),
		mpDevice(nullptr),
		mAvailableLayers(),
		mAvailableExtensions(),
		mAvailablePhysicalDevices(),
		mEnabledLayerNames(),
		mEnabledExtensionNames()
	{
		// Nothing
	}

	void VPLVulkanContext::CreateImageImpl(VkImage* pVkImage, VulkanDeviceMemoryAllocation* pMemory, VkImageType vkImageType,
		VkImageUsageFlags vkUsageFlags, VkFormat vkFormat, UInt32 width, UInt32 height, UInt32 depth, UInt32 mipLevels, UInt32 layers)
	{
		VkImage vkImage;
		VulkanDeviceMemoryAllocation* pMemoryImpl;

		VkImageCreateInfo vkImageInfo = {};
		vkImageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		vkImageInfo.imageType = vkImageType;
		vkImageInfo.format = vkFormat;
		vkImageInfo.extent.width = width;
		vkImageInfo.extent.height = height;
		vkImageInfo.extent.depth = depth;
		vkImageInfo.mipLevels = mipLevels;
		vkImageInfo.arrayLayers = layers;
		vkImageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
		vkImageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		vkImageInfo.usage = vkUsageFlags;
		vkImageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
		vkImageInfo.samples = VK_SAMPLE_COUNT_1_BIT;

		if (vkCreateImage(mpDevice->GetDeviceHandle(), &vkImageInfo, nullptr, &vkImage) != VK_SUCCESS)
		{
			Log::Error("Failed to create vulkan image: vkCreateImage failed!");
			return;
		}

		VkMemoryRequirements vkMemRequirements;
		vkGetImageMemoryRequirements(mpDevice->GetDeviceHandle(), vkImage, &vkMemRequirements);

		VulkanDeviceMemoryAllocator& allocator = mpDevice->GetDeviceMemoryAllocator();
		pMemoryImpl = allocator.Allocate(vkMemRequirements.size, vkMemRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		if (pMemoryImpl == nullptr)
		{
			Log::Error("Failed to create vulkan image: Failed to allocate device memory!");
			return;
		}

		if (vkBindImageMemory(mpDevice->GetDeviceHandle(), vkImage, pMemoryImpl->GetDeviceMemoryHandle(), 0) != VK_SUCCESS)
		{
			Log::Error("Failed to create vulkan buffer object: vkBindBufferMemory failed!");

			allocator.Free(pMemoryImpl);
			vkDestroyImage(mpDevice->GetDeviceHandle(), vkImage, nullptr);

			return;
		}

		*pVkImage = vkImage;
		pMemory = pMemoryImpl;
	}

	void VPLVulkanContext::CreateImageViewImpl(VkImageView* pVkImageView, VkImage vkImage, VkImageViewType vkImageViewType,
		VkImageAspectFlags vkAspectFlags, VkFormat vkFormat, UInt32 mipStart, UInt32 mipLevels, UInt32 layerStart, UInt32 layers)
	{
		VkImageView vkImageView;

		VkImageViewCreateInfo vkImageViewInfo = {};
		vkImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		vkImageViewInfo.flags = 0;
		vkImageViewInfo.image = vkImage;
		vkImageViewInfo.viewType = vkImageViewType;
		vkImageViewInfo.format = vkFormat;

		vkImageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
		vkImageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
		vkImageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
		vkImageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

		vkImageViewInfo.subresourceRange.aspectMask = vkAspectFlags;
		vkImageViewInfo.subresourceRange.baseMipLevel = mipStart;
		vkImageViewInfo.subresourceRange.levelCount = mipLevels;
		vkImageViewInfo.subresourceRange.baseArrayLayer = layerStart;
		vkImageViewInfo.subresourceRange.layerCount = layers;

		if (vkCreateImageView(mpDevice->GetDeviceHandle(), &vkImageViewInfo, nullptr, &vkImageView) != VK_SUCCESS)
		{
			Log::Error("Failed to create vulkan image view: vkCreateImageView failed!");
			return;
		}

		*pVkImageView = vkImageView;
	}

	void VPLVulkanContext::CreateDescriptorSetLayoutImpl(VkDescriptorSetLayout* pVkDescriptorSetLayout, const Array<VkDescriptorSetLayoutBinding>& bindings)
	{
		VkDescriptorSetLayout vkDescriptorSetLayout;

		VkDescriptorSetLayoutCreateInfo vkDescriptorSetLayoutInfo = {};
		vkDescriptorSetLayoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		vkDescriptorSetLayoutInfo.flags = 0;
		vkDescriptorSetLayoutInfo.bindingCount = bindings.Size();
		vkDescriptorSetLayoutInfo.pBindings = bindings.Data();

		if (vkCreateDescriptorSetLayout(mpDevice->GetDeviceHandle(), &vkDescriptorSetLayoutInfo, nullptr, &vkDescriptorSetLayout) != VK_SUCCESS)
		{
			Log::Error("Failed to create vulkan descriptor set layout: vkCreateDescriptorSetLayout failed!");
			return;
		}

		*pVkDescriptorSetLayout = vkDescriptorSetLayout;
	}

	void VPLVulkanContext::CreatePipelineLayoutImpl(VkPipelineLayout* pVkPipelineLayout, const Array<VkDescriptorSetLayout>& desciptorSets)
	{
		VkPipelineLayout vkPipelineLayout;

		VkPipelineLayoutCreateInfo vkLayoutInfo = {};
		vkLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		vkLayoutInfo.flags = 0;
		vkLayoutInfo.setLayoutCount = desciptorSets.Size();
		vkLayoutInfo.pSetLayouts = desciptorSets.Data();

		// TODO: Support push constants?
		vkLayoutInfo.pushConstantRangeCount = 0;
		vkLayoutInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(mpDevice->GetDeviceHandle(), &vkLayoutInfo, nullptr, &vkPipelineLayout) != VK_SUCCESS)
		{
			Log::Error("Failed to create vulkan pipeline layout: vkCreatePipelineLayout failed!");
			return;
		}

		*pVkPipelineLayout = vkPipelineLayout;
	}

	void VPLVulkanContext::CreateGraphicsPipelineImpl(VkPipeline* pPipeline, const VulkanGraphicsPipelineInfo& info, VkPipeline parentId)
	{
		VkPipeline vkPipeline;

		VkGraphicsPipelineCreateInfo vkPipelineInfo = {};
		vkPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;

		if (parentId != VK_NULL_HANDLE /* && pParentPipeline->mPipelineState.flags & VK_PIPELINE_CREATE_ALLOW_DERIVATIVES_BIT*/)
		{
			vkPipelineInfo.flags |= VK_PIPELINE_CREATE_DERIVATIVE_BIT;
			vkPipelineInfo.basePipelineHandle = parentId;
		}

		/* Shader Stage State */

		Array<VkPipelineShaderStageCreateInfo> shaderStageInfos;

		VkPipelineShaderStageCreateInfo vkShaderStageInfo = {};
		vkShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vkShaderStageInfo.flags = 0;
		vkShaderStageInfo.pSpecializationInfo = nullptr;
		vkShaderStageInfo.pNext = nullptr;

		if (info.vertexShader.vkShader != VK_NULL_HANDLE)
		{
			vkShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
			vkShaderStageInfo.module = info.vertexShader.vkShader;
			vkShaderStageInfo.pName = info.vertexShader.entryPoint.Str();

			shaderStageInfos.PushBack(vkShaderStageInfo);
		}
		else
		{
			Log::Error("Failed to create vulkan graphics pipeline: No required vertex shader attached!");
			return;
		}

		if (info.pixelShader.vkShader != VK_NULL_HANDLE)
		{
			vkShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			vkShaderStageInfo.module = info.pixelShader.vkShader;
			vkShaderStageInfo.pName = info.pixelShader.entryPoint.Str();

			shaderStageInfos.PushBack(vkShaderStageInfo);
		}
		else
		{
			Log::Error("Failed to create vulkan graphics pipeline: No required pixel shader attached!");
			return;
		}

		vkPipelineInfo.stageCount = shaderStageInfos.Size();
		vkPipelineInfo.pStages = shaderStageInfos.Data();

		/* Vertex Input State */

		VkPipelineVertexInputStateCreateInfo vkVertexInputStateInfo = {};
		vkVertexInputStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vkVertexInputStateInfo.flags = 0;

		Array<VkVertexInputBindingDescription> bindingDescriptions;
		for (const VulkanVertexBinding& binding : info.vertexBindings)
		{
			VkVertexInputBindingDescription vkBindingDescription;
			vkBindingDescription.binding = binding.binding;
			vkBindingDescription.stride = binding.stride;
			vkBindingDescription.inputRate = binding.inputRate;

			bindingDescriptions.PushBack(vkBindingDescription);
		}

		vkVertexInputStateInfo.vertexBindingDescriptionCount = bindingDescriptions.Size();
		vkVertexInputStateInfo.pVertexBindingDescriptions = bindingDescriptions.Data();

		Array<VkVertexInputAttributeDescription> attributeDescriptions;
		for (const VulkanVertexAttribute& attrib : info.vertexAttributes)
		{
			VkVertexInputAttributeDescription vkAttributeDescription;
			vkAttributeDescription.binding = attrib.binding;
			vkAttributeDescription.location = attrib.location;
			vkAttributeDescription.format = attrib.format;
			vkAttributeDescription.offset = attrib.offset;

			attributeDescriptions.PushBack(vkAttributeDescription);
		}

		vkVertexInputStateInfo.vertexAttributeDescriptionCount = attributeDescriptions.Size();
		vkVertexInputStateInfo.pVertexAttributeDescriptions = attributeDescriptions.Data();

		vkPipelineInfo.pVertexInputState = &vkVertexInputStateInfo;

		/* Input Assembly State */

		VkPipelineInputAssemblyStateCreateInfo vkInputAssemblyInfo = {};
		vkInputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		vkInputAssemblyInfo.flags = 0;
		vkInputAssemblyInfo.topology = info.topology;

		vkPipelineInfo.pInputAssemblyState = &vkInputAssemblyInfo;

		/* Tesselation State */

		vkPipelineInfo.pTessellationState = nullptr;

		/* Viewport State */

		VkPipelineViewportStateCreateInfo vkViewportInfo = {};
		vkViewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		vkViewportInfo.flags = 0;

		Array<VkViewport> viewports;
		for (const VkViewport& viewport : info.viewports)
		{
			viewports.PushBack(viewport);
		}

		vkViewportInfo.viewportCount = viewports.Size();
		vkViewportInfo.pViewports = viewports.Data();

		Array<VkRect2D> scissors;
		for (const VkRect2D& scissor : info.scissors)
		{
			scissors.PushBack(scissor);
		}

		vkViewportInfo.scissorCount = scissors.Size();
		vkViewportInfo.pScissors = scissors.Data();

		vkPipelineInfo.pViewportState = &vkViewportInfo;

		/* Rasterization State */

		VkPipelineRasterizationStateCreateInfo vkRasterizationInfo = {};
		vkRasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		vkRasterizationInfo.flags = 0;
		vkRasterizationInfo.depthClampEnable = VK_FALSE;
		vkRasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
		vkRasterizationInfo.polygonMode = info.polygonMode;
		vkRasterizationInfo.cullMode = info.cullMode;
		vkRasterizationInfo.frontFace = info.frontFace;
		vkRasterizationInfo.depthBiasEnable = 0;
		vkRasterizationInfo.depthBiasConstantFactor = 0;
		vkRasterizationInfo.depthBiasClamp = 0;
		vkRasterizationInfo.depthBiasSlopeFactor = 0;
		vkRasterizationInfo.lineWidth = info.lineWidth;

		vkPipelineInfo.pRasterizationState = &vkRasterizationInfo;

		/* Multisample State */

		UInt32 multisamples = info.multisamples;
		UInt32 maxMultisamples = mpDevice->GetPhysicalDevice().GetPhysicalDeviceProperties().limits.framebufferColorSampleCounts;

		if (multisamples == 0)
		{
			multisamples = 1;
			Log::Warning("Invalid zero multisample value in pipeline creation, setting multisamples to 1");
		}

		if (info.multisamples > maxMultisamples)
		{
			multisamples = maxMultisamples;
			Log::Warning("Invalid maximum multisample value in pipeline creation, setting multisamples to maxMultisamples");
		}

		VkPipelineMultisampleStateCreateInfo vkMultisampleInfo = {};
		vkMultisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		vkMultisampleInfo.flags = 0;
		vkMultisampleInfo.rasterizationSamples = (VkSampleCountFlagBits)multisamples;
		vkMultisampleInfo.sampleShadingEnable = VK_FALSE;
		vkMultisampleInfo.minSampleShading = 0.0f;
		vkMultisampleInfo.pSampleMask = nullptr;
		vkMultisampleInfo.alphaToCoverageEnable = VK_FALSE;
		vkMultisampleInfo.alphaToOneEnable = VK_FALSE;

		vkPipelineInfo.pMultisampleState = &vkMultisampleInfo;

		/* Depth Stencil State */

		VkPipelineDepthStencilStateCreateInfo vkDepthStencilInfo = {};
		vkDepthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
		vkDepthStencilInfo.flags = 0;
		vkDepthStencilInfo.depthTestEnable = info.enableDepthTest;
		vkDepthStencilInfo.depthWriteEnable = info.enableDepthWrite;
		vkDepthStencilInfo.depthCompareOp = info.depthCompareOp;
		vkDepthStencilInfo.depthBoundsTestEnable = info.enableDepthBoundsTest;
		vkDepthStencilInfo.stencilTestEnable = info.enableStencilTest;
		vkDepthStencilInfo.front.failOp = info.frontFailOp;
		vkDepthStencilInfo.front.passOp = info.frontPassOp;
		vkDepthStencilInfo.front.depthFailOp = info.frontDepthFailOp;
		vkDepthStencilInfo.front.compareOp = info.frontCompareOp;
		vkDepthStencilInfo.front.compareMask = info.frontCompareMask;
		vkDepthStencilInfo.front.writeMask = info.frontWriteMask;
		vkDepthStencilInfo.front.reference = info.frontReference;
		vkDepthStencilInfo.back.failOp = info.backFailOp;
		vkDepthStencilInfo.back.passOp = info.backPassOp;
		vkDepthStencilInfo.back.depthFailOp = info.backDepthFailOp;
		vkDepthStencilInfo.back.compareOp = info.backCompareOp;
		vkDepthStencilInfo.back.compareMask = info.backCompareMask;
		vkDepthStencilInfo.back.writeMask = info.backWriteMask;
		vkDepthStencilInfo.back.reference = info.backReference;
		vkDepthStencilInfo.minDepthBounds = info.minDepth;
		vkDepthStencilInfo.maxDepthBounds = info.maxDepth;

		vkPipelineInfo.pDepthStencilState = &vkDepthStencilInfo;

		/* Color Blend State */

		VkPipelineColorBlendStateCreateInfo vkColorBlendInfo = {};
		vkColorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		vkColorBlendInfo.flags = 0;
		vkColorBlendInfo.logicOpEnable = info.enableColorBlendLogicOp;
		vkColorBlendInfo.logicOp = info.colorBlendLogicOp;

		Array<VkPipelineColorBlendAttachmentState> blendAttachments;
		for (const VulkanBlendAttachment& blendAttachment : info.colorBlendAttachments)
		{
			VkPipelineColorBlendAttachmentState vkBlendAttachmentState = {};
			vkBlendAttachmentState.blendEnable = blendAttachment.enableBlend;
			vkBlendAttachmentState.srcColorBlendFactor = blendAttachment.srcColorBlendFactor;
			vkBlendAttachmentState.dstColorBlendFactor = blendAttachment.dstColorBlendFactor;
			vkBlendAttachmentState.colorBlendOp = blendAttachment.colorBlendOp;
			vkBlendAttachmentState.srcAlphaBlendFactor = blendAttachment.srcAlphaBlendFactor;
			vkBlendAttachmentState.dstAlphaBlendFactor = blendAttachment.dstAlphaBlendFactor;
			vkBlendAttachmentState.alphaBlendOp = blendAttachment.alphaBlendOp;
			vkBlendAttachmentState.colorWriteMask = blendAttachment.colorWriteMask;

			blendAttachments.PushBack(vkBlendAttachmentState);
		}

		vkColorBlendInfo.attachmentCount = blendAttachments.Size();
		vkColorBlendInfo.pAttachments = blendAttachments.Data();

		// TODO: expose blend constants to the pipeline state?
		vkColorBlendInfo.blendConstants[0] = 1.0f;
		vkColorBlendInfo.blendConstants[1] = 1.0f;
		vkColorBlendInfo.blendConstants[2] = 1.0f;
		vkColorBlendInfo.blendConstants[3] = 1.0f;

		vkPipelineInfo.pColorBlendState = &vkColorBlendInfo;

		/* Dynamic States */

		// TODO: Support dynamic states
		vkPipelineInfo.pDynamicState = nullptr;

		/* Pipeline Layout */

		vkPipelineInfo.layout = info.pipelineLayout;

		/* Render Pass */

		vkPipelineInfo.renderPass = info.renderPass;
		vkPipelineInfo.subpass = info.subpassIndex;

		if (vkCreateGraphicsPipelines(mpDevice->GetDeviceHandle(), VK_NULL_HANDLE, 1, &vkPipelineInfo, nullptr, &vkPipeline) != VK_SUCCESS)
		{
			Log::Error("Failed to create vulkan graphics pipeline: vkCreateGraphicsPipelines failed!");
			return;
		}

		// TEMP

		VkSampler vkSampler;

		VkSamplerCreateInfo vkSamplerInfo{};
		vkSamplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		vkSamplerInfo.magFilter = VK_FILTER_LINEAR;
		vkSamplerInfo.minFilter = VK_FILTER_LINEAR;
		vkSamplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		vkSamplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		vkSamplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		vkSamplerInfo.anisotropyEnable = VK_TRUE;
		vkSamplerInfo.maxAnisotropy = 16.0f;
		vkSamplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		vkSamplerInfo.unnormalizedCoordinates = VK_FALSE;
		vkSamplerInfo.compareEnable = VK_FALSE;
		vkSamplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		vkSamplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		vkSamplerInfo.mipLodBias = 0.0f;
		vkSamplerInfo.minLod = 0.0f;
		vkSamplerInfo.maxLod = 0.0f;

		if (vkCreateSampler(mpDevice->GetDeviceHandle(), &vkSamplerInfo, nullptr, &vkSampler) != VK_SUCCESS)
		{
			// Failed
		}

		*pPipeline = vkPipeline;
	}

	HGFXImage VPLVulkanContext::CreateImage(GFXImageType type, GFXImageUsages usages, GFXImageFormat imageFormat,
		UInt32 width, UInt32 height, UInt32 depth, UInt32 mipLevels, UInt32 layers)
	{
		VkImage vkImage = VK_NULL_HANDLE;
		VulkanDeviceMemoryAllocation* pMemory = nullptr;

		VkImageType vkImageType = ImageTypeToVkImageType(type);
		VkImageUsageFlags vkUsage = ImageUsagesToVkImageUsage(usages);
		VkFormat vkFormat = ImageFormatToVkFormat(imageFormat);

		CreateImageImpl(&vkImage, pMemory, vkImageType, vkUsage, vkFormat, width, height, depth, mipLevels, layers);

		if (vkImage == VK_NULL_HANDLE)
		{
			return HGFX_NULL_HANDLE;
		}

		VulkanImage* pImage = new VulkanImage();
		pImage->vkImage = vkImage;
		pImage->vkFormat = vkFormat;
		pImage->vkUsage = vkUsage;
		pImage->vkImageType = vkImageType;
		pImage->pMemory = pMemory;
		pImage->width = width;
		pImage->height = height;
		pImage->depth = depth;
		pImage->mipLevels = mipLevels;
		pImage->layers = layers;

		return HGFXImage(pImage);
	}

	void VPLVulkanContext::DestroyImage(HGFXImage image)
	{
		VulkanImage* pImage = static_cast<VulkanImage*>(image);

		vkFreeMemory(mpDevice->GetDeviceHandle(), pImage->pMemory->GetDeviceMemoryHandle(), VK_NULL_HANDLE);
		mpDevice->GetDeviceMemoryAllocator().Free(pImage->pMemory);

		delete pImage;
	}

	HGFXImageView VPLVulkanContext::CreateImageView(HGFXImage image, GFXImageViewType viewType, GFXImageUsage usage, 
		UInt32 mipStart, UInt32 mipLevels, UInt32 layerStart, UInt32 layers)
	{
		VkImageView vkImageView = VK_NULL_HANDLE;
		VulkanImage* pImage = static_cast<VulkanImage*>(image);

		VkImageViewType vkViewType = ImageViewTypeToVkImageViewType(viewType);
		VkImageAspectFlags vkAspectMask = ImageUsageToVkImageAspects(usage);
		
		CreateImageViewImpl(&vkImageView, pImage->vkImage, vkViewType, vkAspectMask,
			pImage->vkFormat, mipStart, mipLevels, layerStart, layers);

		if (vkImageView == VK_NULL_HANDLE)
		{
			return HGFX_NULL_HANDLE;
		}

		VulkanImageView* pImageView = new VulkanImageView();
		pImageView->vkImageView = vkImageView;
		pImageView->vkFormat = pImage->vkFormat;
		pImageView->vkViewType = vkViewType;
		pImageView->vkAspects = vkAspectMask;
		pImageView->mipStart = mipStart;
		pImageView->mipLevels = mipLevels;
		pImageView->layerStart = layerStart;
		pImageView->layers = layers;

		return HGFXImageView(pImageView);
	}

	void VPLVulkanContext::DestroyImageView(HGFXImageView imageView)
	{
		VulkanImageView* pImageView = static_cast<VulkanImageView*>(imageView);

		vkDestroyImageView(mpDevice->GetDeviceHandle(), pImageView->vkImageView, VK_NULL_HANDLE);

		delete pImageView;
	}

	HGFXRenderPass VPLVulkanContext::CreateRenderPass(const GFXRenderPassInfo& info)
	{
		VkRenderPass vkRenderPass;

		VkRenderPassCreateInfo vkRenderPassCreateInfo;

		Array<VkAttachmentDescription> attachmentDescriptions;
		Array<VkSubpassDescription> subpassDescriptions;

		Array<Array<VkAttachmentReference>> subpassColorAttachmentReferences;
		Array<Array<VkAttachmentReference>> subpassInputAttachmentReferences;
		Array<VkAttachmentReference> subpassDepthAttachmentReferences;

		subpassDescriptions.Resize(info.subpasses.Size());

		subpassColorAttachmentReferences.Resize(info.subpasses.Size());
		subpassInputAttachmentReferences.Resize(info.subpasses.Size());
		subpassDepthAttachmentReferences.Resize(info.subpasses.Size());

		Array<GFXRenderAttachment*> repeatAttachments;

		for (UInt32 i = 0; i < info.subpasses.Size(); i++)
		{
			const GFXRenderSubpassInfo& subpass = info.subpasses[i];

			VkSubpassDescription& vkSubpassDescription = subpassDescriptions[i];
			vkSubpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			vkSubpassDescription.flags = 0;

			for (UInt32 j = 0; j < subpass.attachments.Size(); j++)
			{
				const GFXRenderSubpassAttachment& subpassAttachemnt = subpass.attachments[j];
				GFXRenderAttachment* pAttachment = subpassAttachemnt.pAttachment;

				UInt32 index = repeatAttachments.IndexOf(pAttachment);

				if (index >= repeatAttachments.Size())
				{
					VkAttachmentDescription vkAttachmentDescription;
					vkAttachmentDescription.flags = 0;
					vkAttachmentDescription.format = ImageFormatToVkFormat(pAttachment->format);
					vkAttachmentDescription.loadOp = LoadOperationToVkAttachmentLoadOp(pAttachment->loadOp);
					vkAttachmentDescription.storeOp = StoreOperationToVkAttachmentStoreOp(pAttachment->storeOp);
					vkAttachmentDescription.stencilLoadOp = LoadOperationToVkAttachmentLoadOp(pAttachment->stencilLoadOp);
					vkAttachmentDescription.stencilStoreOp = StoreOperationToVkAttachmentStoreOp(pAttachment->stencilStoreOp);
					vkAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;
					vkAttachmentDescription.initialLayout = ImageLayoutToVkImageLayout(pAttachment->initalLayout);
					vkAttachmentDescription.finalLayout = ImageLayoutToVkImageLayout(pAttachment->finalLayout);

					attachmentDescriptions.PushBack(vkAttachmentDescription);
					repeatAttachments.PushBack(pAttachment);
				}

				VkAttachmentReference vkAttachmentReference;
				vkAttachmentReference.attachment = index;
				vkAttachmentReference.layout = ImageLayoutToVkImageLayout(subpassAttachemnt.layout);

				if (vkAttachmentReference.layout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL)
				{
					vkSubpassDescription.colorAttachmentCount++;
					subpassColorAttachmentReferences[i].PushBack(vkAttachmentReference);
				}
				else if ((UInt32)vkAttachmentReference.layout < (UInt32)VK_IMAGE_LAYOUT_DEPTH_READ_ONLY_OPTIMAL)
				{
					vkSubpassDescription.pDepthStencilAttachment = &vkAttachmentReference;
					//TODO: throw error if multiple depth-stencil buffers are present?
				}
				else
				{
					vkSubpassDescription.inputAttachmentCount++;
					subpassInputAttachmentReferences[i].PushBack(vkAttachmentReference);
				}
			}

			vkSubpassDescription.pColorAttachments = subpassColorAttachmentReferences[i].Data();
			vkSubpassDescription.pInputAttachments = subpassInputAttachmentReferences[i].Data();

			vkSubpassDescription.preserveAttachmentCount = 0;
			vkSubpassDescription.pPreserveAttachments = nullptr;
		}

		Array<VkSubpassDependency> subpassDependancies(info.dependancies.Size());

		for (UInt32 i = 0; i < info.dependancies.Size(); i++)
		{
			const GFXSubpassDependancy& dependancy = info.dependancies[i];

			VkSubpassDependency vkSubpassDependancy = {};
			vkSubpassDependancy.srcSubpass = SubpassIndexToVkSubpassIndex(dependancy.subpassSrcIndex);
			vkSubpassDependancy.dstSubpass = SubpassIndexToVkSubpassIndex(dependancy.subpassDestIndex);
			vkSubpassDependancy.srcStageMask = PipelineStagesToVkPipelineStageFlags(dependancy.srcStage);
			vkSubpassDependancy.dstStageMask = PipelineStagesToVkPipelineStageFlags(dependancy.destStage);
			vkSubpassDependancy.srcAccessMask = PipelineAccessToVkAccessFlags(dependancy.srcAccess);
			vkSubpassDependancy.dstAccessMask = PipelineAccessToVkAccessFlags(dependancy.destAccess);
			vkSubpassDependancy.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

			subpassDependancies[i] = vkSubpassDependancy;
		}

		vkRenderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		vkRenderPassCreateInfo.flags = 0;
		vkRenderPassCreateInfo.pNext = nullptr;
		vkRenderPassCreateInfo.attachmentCount = attachmentDescriptions.Size();
		vkRenderPassCreateInfo.pAttachments = attachmentDescriptions.Data();
		vkRenderPassCreateInfo.subpassCount = subpassDescriptions.Size();
		vkRenderPassCreateInfo.pSubpasses = subpassDescriptions.Data();
		vkRenderPassCreateInfo.dependencyCount = subpassDependancies.Size();
		vkRenderPassCreateInfo.pDependencies = subpassDependancies.Data();

		if (vkCreateRenderPass(mpDevice->GetDeviceHandle(), &vkRenderPassCreateInfo, nullptr, &vkRenderPass) != VK_SUCCESS)
		{
			Log::Error("Failed to create vulkan render pass: vkCreateRenderPass failed!");
			return HGFX_NULL_HANDLE;
		}

		VulkanRenderPass* pRenderPass = new VulkanRenderPass;
		pRenderPass->vkRenderPass = vkRenderPass;

		return HGFXRenderPass(pRenderPass);
	}

	HGFXFramebuffer VPLVulkanContext::CreateFramebuffer(HGFXRenderPass renderPass, const Array<HGFXImageView> imageViews, UInt32 width, UInt32 height)
	{
		Array<VkImageView> vkImageViews(imageViews.Size());
		VkFramebuffer vkFramebuffer;

		for (UInt32 i = 0; i < imageViews.Size(); i++)
		{
			vkImageViews[i] = static_cast<VulkanImageView*>(imageViews[i])->vkImageView;
		}

		VkFramebufferCreateInfo framebufferInfo{};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = static_cast<VulkanRenderPass*>(renderPass)->vkRenderPass;
		framebufferInfo.attachmentCount = vkImageViews.Size();
		framebufferInfo.pAttachments = vkImageViews.Data();
		framebufferInfo.width = width;
		framebufferInfo.height = height;
		framebufferInfo.layers = 1;

		if (vkCreateFramebuffer(mpDevice->GetDeviceHandle(), &framebufferInfo, nullptr, &vkFramebuffer) != VK_SUCCESS)
		{
			Log::Error("Failed to create vulkan frame buffer: vkCreateFramebuffer failed!");
			return HGFX_NULL_HANDLE;
		}

		VulkanFramebuffer* pFramebuffer = new VulkanFramebuffer;
		pFramebuffer->vkFramebuffer = vkFramebuffer;
		pFramebuffer->width = width;
		pFramebuffer->height = height;

		return HGFXFramebuffer(pFramebuffer);
	}

	void VPLVulkanContext::GenerateDescriptorSets(Array<VkDescriptorSetLayout>& descriptorSetLayouts, const Array<VulkanShader>& shaders)
	{
		// @Todo: querry max supported sets
		Array<Map<String, VkDescriptorSetLayoutBinding>> descriptorSetList(16);

		USize maxSetIndex = 0;

		for (VulkanShader& shader : shaders)
		{
			for (UInt32 i = 0; i < shader.uniforms.Size(); i++)
			{
				const SpirvUniform& uniform = shader.uniforms[i];

				if (descriptorSetList[uniform.set].Contains(uniform.name))
				{
					descriptorSetList[uniform.set][uniform.name].stageFlags |= shader.stage;
				}
				else
				{
					VkDescriptorSetLayoutBinding vkBinding;
					vkBinding.binding = uniform.binding;
					vkBinding.descriptorCount = 1;
					vkBinding.descriptorType = uniform.descriptorType;
					vkBinding.stageFlags = shader.stage;
					vkBinding.pImmutableSamplers = nullptr;

					// @Safty: check set bounds < 16
					descriptorSetList[uniform.set].Put(uniform.name, vkBinding);
				}

				if (uniform.set > maxSetIndex)
				{
					maxSetIndex = uniform.set;
				}
			}

			// @Todo: samplers, etc.
		}

		descriptorSetLayouts.Resize(maxSetIndex + 1);

		UInt32 setIndex = 0;
		for (const Map<String, VkDescriptorSetLayoutBinding>& set : descriptorSetList)
		{
			if (setIndex > maxSetIndex)
			{
				break;
			}

			Array<VkDescriptorSetLayoutBinding> setBindings;

			for (const MapPair<String, VkDescriptorSetLayoutBinding>& binding : set)
			{
				setBindings.PushBack(binding.value);
			}

			VkDescriptorSetLayout vkDescriptorSetLayout = VK_NULL_HANDLE;
			CreateDescriptorSetLayoutImpl(&vkDescriptorSetLayout, setBindings);

			descriptorSetLayouts[setIndex] = vkDescriptorSetLayout;

			setIndex++;
		}
	}

	void VPLVulkanContext::PreInitialize()
	{
		Log::Debug("Pre-initializing Vulkan Graphics...");
		// Nothing
	}

	void VPLVulkanContext::Initialize()
	{
		Log::Debug("Initializing Vulkan Graphics...");

		Array<String> extensions;
		extensions.PushBack(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		extensions.PushBack(VK_KHR_SURFACE_EXTENSION_NAME);
		extensions.PushBack("VK_KHR_win32_surface");

		Array<String> validationLayers;
		validationLayers.PushBack("VK_LAYER_KHRONOS_validation");

		InitInstance(Engine::GetInstance().GetGameInfo().name, extensions, validationLayers);
		InitDevices();
	}

    HGFXGraphicsPipeline VPLVulkanContext::CreateGraphicsPipeline(const GFXGraphicsPipelineInfo& info, HGFXRenderPass renderPass, UInt32 renderPassIndex)
    {
		VkPipeline vkPipeline = VK_NULL_HANDLE;

		VkPrimitiveTopology vkTopology = PrimitiveTopologyToVkPrimitiveTopology(info.topology);
		VkPolygonMode vkPolygonMode = PolygonModeToVkPolygonMode(info.polygonMode);
		VkCullModeFlags vkCullMode = CullModeToVkCullMode(info.cullMode);
		VkFrontFace vkFrontFace = FaceWindToVkFrontFace(info.faceWind);
		UInt32 multisamples = MultisampleToVkMultisample(info.multisample);

		VulkanGraphicsPipelineInfo pipelineInfo = {};
		pipelineInfo.flags = 0;
		pipelineInfo.vertexShader = *static_cast<VulkanShader*>(info.vertexShader);
		pipelineInfo.pixelShader = *static_cast<VulkanShader*>(info.pixelShader);
		pipelineInfo.topology = vkTopology;

		VkViewport vkViewport;
		vkViewport.x = info.viewport.bounds.start.x;
		vkViewport.y = info.viewport.bounds.Height() - info.viewport.bounds.start.y;
		vkViewport.width = info.viewport.bounds.Width();
		vkViewport.height = -info.viewport.bounds.Height();
		vkViewport.minDepth = info.viewport.minDepth;
		vkViewport.maxDepth = info.viewport.maxDepth;

		VkRect2D vkScissor;
		vkScissor.offset.x = info.scissor.bounds.start.x;
		vkScissor.offset.y = info.scissor.bounds.start.y;
		vkScissor.extent.width = info.scissor.bounds.Width();
		vkScissor.extent.height = info.scissor.bounds.Height();

		pipelineInfo.viewports.PushBack(vkViewport);
		pipelineInfo.scissors.PushBack(vkScissor);
		pipelineInfo.polygonMode = vkPolygonMode;
		pipelineInfo.cullMode = vkCullMode;
		pipelineInfo.frontFace = vkFrontFace;
		
		pipelineInfo.lineWidth = info.lineWidth;
		
		pipelineInfo.multisamples = multisamples;

		pipelineInfo.enableDepthTest = info.depthTesting;
		pipelineInfo.enableDepthWrite = true;
		pipelineInfo.enableDepthBoundsTest = false;
		pipelineInfo.enableStencilTest = false;

		pipelineInfo.depthCompareOp = VK_COMPARE_OP_LESS;
		pipelineInfo.frontFailOp;
		pipelineInfo.frontPassOp;
		pipelineInfo.frontDepthFailOp;
		pipelineInfo.frontCompareOp;
		pipelineInfo.frontCompareMask;
		pipelineInfo.frontWriteMask;
		pipelineInfo.frontReference;
		pipelineInfo.backFailOp;
		pipelineInfo.backPassOp;
		pipelineInfo.backDepthFailOp;
		pipelineInfo.backCompareOp;
		pipelineInfo.backCompareMask;
		pipelineInfo.backWriteMask;
		pipelineInfo.backReference;

		pipelineInfo.minDepth = 0.0f;
		pipelineInfo.maxDepth = 1.0f;

		pipelineInfo.enableColorBlendLogicOp = false;
		pipelineInfo.colorBlendLogicOp = VK_LOGIC_OP_COPY;

		for (GFXBufferAttachent bufferAttachment : info.bufferAttachemnts)
		{
			VulkanVertexBinding bufferBinding;
			bufferBinding.binding = bufferAttachment.binding;
			bufferBinding.stride = bufferAttachment.stride;
			bufferBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			pipelineInfo.vertexBindings.PushBack(bufferBinding);
		}

		UInt32 elementOffset = 0;
		for (GFXVertexAttribute vertexAttribute : info.vertexAttributes)
		{
			VkFormat vkFormat = AttributeTypeToVkFormat(vertexAttribute.type);

			VulkanVertexAttribute vertexAttrib;
			vertexAttrib.binding = vertexAttribute.binding;
			vertexAttrib.location = vertexAttribute.location;
			vertexAttrib.format = vkFormat;
			vertexAttrib.offset = elementOffset;

			elementOffset += AttributeSize(vertexAttribute.type);

			pipelineInfo.vertexAttributes.PushBack(vertexAttrib);
		}

		for (GFXBlendAttachment blendAttachment : info.blendAttachments)
		{
			VulkanBlendAttachment blend;
			blend.enableBlend = VK_TRUE;
			blend.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
			blend.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
			blend.colorBlendOp = VK_BLEND_OP_ADD;
			blend.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
			blend.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
			blend.alphaBlendOp = VK_BLEND_OP_ADD;
			blend.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

			pipelineInfo.colorBlendAttachments.PushBack(blend);
		}

		Array<VkDescriptorSetLayout> descriptorSetLayouts;
		Array<VulkanShader> shaders = { pipelineInfo.vertexShader, pipelineInfo.pixelShader };
		GenerateDescriptorSets(descriptorSetLayouts, shaders);

		VkPipelineLayout vkPipelineLayout;
		CreatePipelineLayoutImpl(&vkPipelineLayout, descriptorSetLayouts);

		if (vkPipelineLayout == VK_NULL_HANDLE)
		{
			return HGFX_NULL_HANDLE;
		}

		pipelineInfo.pipelineLayout = vkPipelineLayout;

		VulkanRenderPass* pVulkanRenderPass = static_cast<VulkanRenderPass*>(renderPass);

		pipelineInfo.renderPass = pVulkanRenderPass->vkRenderPass;
		pipelineInfo.subpassIndex = renderPassIndex;

		CreateGraphicsPipelineImpl(&vkPipeline, pipelineInfo, VK_NULL_HANDLE);

		if (vkPipeline == VK_NULL_HANDLE)
		{
			return HGFX_NULL_HANDLE;
		}

		// @Todo: Properly implement SetBackbufferCount()
		mBackbufferCount = info.backbufferCount;
		Array<VkDescriptorSet> descriptorSets(mBackbufferCount);

		for (UInt32 i = 0; i < mBackbufferCount; i++)
		{
			VkDescriptorSetAllocateInfo allocInfo{};
			allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
			allocInfo.descriptorPool = mpDevice->GetDescriptorPool();
			allocInfo.descriptorSetCount = descriptorSetLayouts.Size();
			allocInfo.pSetLayouts = descriptorSetLayouts.Data();

			if (vkAllocateDescriptorSets(mpDevice->GetDeviceHandle(), &allocInfo, &descriptorSets[i]) != VK_SUCCESS)
			{
				// @Todo: error message
			}
		}

        VulkanGraphicsPipeline* pPipeline = new VulkanGraphicsPipeline();
        pPipeline->vkPipeline = vkPipeline;
		pPipeline->pipelineInfo = pipelineInfo;
		pPipeline->descriptorSets = descriptorSets;

        return HGFXGraphicsPipeline(pPipeline);
    }

	HGFXCommandBuffer VPLVulkanContext::CreateCommandBuffer(GFXCommnadBufferUsages usage)
	{
		VkCommandBuffer vkCommandBuffer;
		VkCommandPool vkCommandPool = VK_NULL_HANDLE;
		
		if (usage == 0)
		{
			return HGFX_NULL_HANDLE;
		}

		if (usage & GFX_COMMAND_BUFFER_USAGE_GRAPHICS_BIT)
		{
			vkCommandPool = mpDevice->GetGraphicsCommandPoolHandle();
		}
		else if (usage & GFX_COMMAND_BUFFER_USAGE_COMPUTE_BIT)
		{
			vkCommandPool = mpDevice->GetComputeCommandPoolHandle();
		}
		else
		{
			vkCommandPool = mpDevice->GetTransferCommandPoolHandle();
		}

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = vkCommandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		if (vkAllocateCommandBuffers(mpDevice->GetDeviceHandle(), &allocInfo, &vkCommandBuffer) != VK_SUCCESS)
		{
			Log::Error("Failed to allocate command buffer: vkAllocateCommandBuffers failed!");
			return HGFX_NULL_HANDLE;
		}

		VulkanCommandBuffer* pCommandBuffer = new VulkanCommandBuffer();
		pCommandBuffer->vkCommandBuffer = vkCommandBuffer;
		pCommandBuffer->vkCommandPool = vkCommandPool;
		pCommandBuffer->usage = usage;

		return HGFXCommandBuffer(pCommandBuffer);
	}

	HGFXShader VPLVulkanContext::CreateShader(GFXShaderStage shaderStage, const Array<Byte>& shaderData, const char* entryPoint)
	{
		VkShaderModule vkShader;
		SpirvReflection reflection;

		//TODO: remove entrypoint specification
		if (entryPoint == nullptr)
		{
			Log::Error("Failed to create shader: no shader entry name specified!");
			return HGFX_NULL_HANDLE;
		}

		VkShaderModuleCreateInfo shaderInfo{};
		shaderInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shaderInfo.codeSize = shaderData.Size();
		shaderInfo.pCode = reinterpret_cast<const uint32_t*>(shaderData.Data());

		if (vkCreateShaderModule(mpDevice->GetDeviceHandle(), &shaderInfo, nullptr, &vkShader) != VK_SUCCESS)
		{
			Log::Error("Failed to create shader: vkCreateShaderModule failed!");
			return HGFX_NULL_HANDLE;
		}

		if (!SpirvParseReflection(&reflection, shaderData))
		{
			Log::Error("Failed to create shader: Failed to parse SPIR-V reflection data!");
			return HGFX_NULL_HANDLE;
		}

		Array<SpirvUniform> uniforms;
		SpirvExtractUniforms(uniforms, reflection);

		VulkanShader* pShader = new VulkanShader;
		pShader->vkShader = vkShader;
		pShader->stage = reflection.shaderStage;
		pShader->entryPoint = entryPoint;
		pShader->uniforms = uniforms;

		return HGFXShader(pShader);
	}

	HGFXBuffer VPLVulkanContext::CreateBuffer(GFXBufferUsages usages, GFXBufferAccess access, UInt32 sizeBytes)
	{
		VkBuffer vkBuffer;

		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size = sizeBytes;
		bufferInfo.usage = BufferUsagesToVkBufferUsageFlags(usages);
		bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(mpDevice->GetDeviceHandle(), &bufferInfo, nullptr, &vkBuffer) != VK_SUCCESS)
		{
			Log::Error("Failed to create vulkan buffer object: vkCreateBuffer failed!");
			return HGFX_NULL_HANDLE;
		}

		VkMemoryRequirements memRequirements;
		vkGetBufferMemoryRequirements(mpDevice->GetDeviceHandle(), vkBuffer, &memRequirements);

		VulkanDeviceMemoryAllocator& allocator = mpDevice->GetDeviceMemoryAllocator();
		VulkanDeviceMemoryAllocation* pMemory = allocator.Allocate(memRequirements.size, 
			memRequirements.memoryTypeBits, BufferAccessToVkMemoryPropertyFlags(access));

		if (pMemory == nullptr)
		{
			Log::Error("Failed to create vulkan buffer object: Failed to allocate device memory!");
			return HGFX_NULL_HANDLE;
		}

		if (vkBindBufferMemory(mpDevice->GetDeviceHandle(), vkBuffer, pMemory->GetDeviceMemoryHandle(), 0) != VK_SUCCESS)
		{
			Log::Error("Failed to create vulkan buffer object: vkBindBufferMemory failed!");

			allocator.Free(pMemory);
			vkDestroyBuffer(mpDevice->GetDeviceHandle(), vkBuffer, nullptr);

			return HGFX_NULL_HANDLE;
		}

		VulkanBuffer* pVulkanBuffer = new VulkanBuffer;
		pVulkanBuffer->vkBuffer = vkBuffer;
		pVulkanBuffer->pMemory = pMemory;
		pVulkanBuffer->sizeBytes = sizeBytes;

		return HGFXBuffer(pVulkanBuffer);
	}

	HGFXUniform VPLVulkanContext::CreateUniform(GFXUniformType uniformType, HGFXBuffer buffer, UInt32 offset)
	{
		// Unfinished
		return HGFXUniform();
	}

	HGFXSampler VPLVulkanContext::CreateSampler(GFXSamplerFilter filter, GFXSamplerFilter mipmapFilter, GFXSamplerMode mode, UInt32 anisotropy)
	{
		VkSampler vkSampler;

		VkSamplerCreateInfo samplerInfo {};
		samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerInfo.magFilter = VK_FILTER_NEAREST;
		samplerInfo.minFilter = VK_FILTER_NEAREST;
		samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerInfo.anisotropyEnable = VK_TRUE;
		samplerInfo.maxAnisotropy = anisotropy;
		samplerInfo.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
		samplerInfo.unnormalizedCoordinates = VK_FALSE;
		samplerInfo.compareEnable = VK_FALSE;
		samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;

		if (vkCreateSampler(mpDevice->GetDeviceHandle(), &samplerInfo, nullptr, &vkSampler) != VK_SUCCESS)
		{
			Log::Error("Failed to create sapler: vkCreateSampler failed!");
		}

		VulkanSampler* pVulkanSampler = new VulkanSampler;
		pVulkanSampler->vkSampler = vkSampler;

		return HGFXSampler(pVulkanSampler);
	}

	void* VPLVulkanContext::MapBuffer(HGFXBuffer buffer)
	{
		VulkanBuffer* pVulkanBuffer = static_cast<VulkanBuffer*>(buffer);

		//TODO: clean this up
		void* pMapData = pVulkanBuffer->pMemory->MapMemory(0, pVulkanBuffer->sizeBytes);

		return pMapData;
	}

	void VPLVulkanContext::UnmapBuffer(HGFXBuffer buffer)
	{
		VulkanBuffer* pVulkanBuffer = static_cast<VulkanBuffer*>(buffer);

		//TODO: clean this up
		pVulkanBuffer->pMemory->UnmapMemory();
	}

	void VPLVulkanContext::CopyBufferToImage(HGFXBuffer buffer, HGFXImage image)
	{
		// @TODO: Probably clean this up to not have to recreate command buffers

		VulkanBuffer* pVulkanBuffer = static_cast<VulkanBuffer*>(buffer);
		VulkanImage* pVulkanImage = static_cast<VulkanImage*>(image);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = mpDevice->GetTransferCommandPoolHandle();
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(mpDevice->GetDeviceHandle(), &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		VkBufferImageCopy imageCopy{};
		imageCopy.bufferOffset = 0;
		imageCopy.bufferRowLength = 0;
		imageCopy.bufferImageHeight = 0;

		imageCopy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageCopy.imageSubresource.mipLevel = 0;
		imageCopy.imageSubresource.baseArrayLayer = 0;
		imageCopy.imageSubresource.layerCount = 1;

		imageCopy.imageOffset = { 0, 0, 0 };
		imageCopy.imageExtent = { pVulkanImage->width, pVulkanImage->height, 1 };

		vkCmdCopyBufferToImage(
			commandBuffer,
			pVulkanBuffer->vkBuffer,
			pVulkanImage->vkImage,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			1,
			&imageCopy
		);

		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(mpDevice->GetTransferQueue().GetQueueHandle(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(mpDevice->GetTransferQueue().GetQueueHandle());

		vkFreeCommandBuffers(mpDevice->GetDeviceHandle(), mpDevice->GetTransferCommandPoolHandle(), 1, &commandBuffer);
	}

	void VPLVulkanContext::TransitionImage(HGFXImage image, GFXImageLayout oldLayout, GFXImageLayout newLayout)
	{
		// @TODO: Probably clean this up to not have to recreate command buffers

		VulkanImage* pVulkanImage = static_cast<VulkanImage*>(image);

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = mpDevice->GetGraphicsCommandPoolHandle();
		allocInfo.commandBufferCount = 1;

		VkCommandBuffer commandBuffer;
		vkAllocateCommandBuffers(mpDevice->GetDeviceHandle(), &allocInfo, &commandBuffer);

		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(commandBuffer, &beginInfo);

		VkImageMemoryBarrier barrier{};
		barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		barrier.oldLayout = ImageLayoutToVkImageLayout(oldLayout);
		barrier.newLayout = ImageLayoutToVkImageLayout(newLayout);
		barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		barrier.image = pVulkanImage->vkImage;
		barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		barrier.subresourceRange.baseMipLevel = 0;
		barrier.subresourceRange.levelCount = 1;
		barrier.subresourceRange.baseArrayLayer = 0;
		barrier.subresourceRange.layerCount = 1;
		barrier.srcAccessMask = 0;
		barrier.dstAccessMask = 0;

		VkPipelineStageFlags sourceStage;
		VkPipelineStageFlags destinationStage;

		if (barrier.oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && 
			barrier.newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
		{
			barrier.srcAccessMask = 0;
			barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

			sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
			destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
		}
		else if (barrier.oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
			&& barrier.newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
		{
			barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

			sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
		}
		else
		{
			// @TODO: Implement more
			Log::Critical("Failed to transition image: Invalid layouts");
		}

		vkCmdPipelineBarrier(commandBuffer, sourceStage, destinationStage, 
			0, 0, nullptr, 0, nullptr, 1, &barrier);

		vkEndCommandBuffer(commandBuffer);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffer;

		vkQueueSubmit(mpDevice->GetGraphicsQueue().GetQueueHandle(), 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(mpDevice->GetGraphicsQueue().GetQueueHandle());

		vkFreeCommandBuffers(mpDevice->GetDeviceHandle(), mpDevice->GetGraphicsCommandPoolHandle(), 1, &commandBuffer);
	}

	void VPLVulkanContext::BeginCommandBuffer(HGFXCommandBuffer commandBuffer)
	{
		VulkanCommandBuffer* pCommandBuffer = static_cast<VulkanCommandBuffer*>(commandBuffer);

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(pCommandBuffer->vkCommandBuffer, &beginInfo) != VK_SUCCESS)
		{
			Log::Error("Failed to begin command buffer recording: vkBeginCommandBuffer failed!");
		}
	}

	void VPLVulkanContext::EndCommandBuffer(HGFXCommandBuffer commandBuffer)
	{
		VulkanCommandBuffer* pCommandBuffer = static_cast<VulkanCommandBuffer*>(commandBuffer);

		if (vkEndCommandBuffer(pCommandBuffer->vkCommandBuffer) != VK_SUCCESS)
		{
			Log::Error("Failed to end command buffer recording: vkBeginCommandBuffer failed!");
		}
	}

	void VPLVulkanContext::BeginRenderPass(HGFXCommandBuffer commandBuffer, HGFXRenderPass renderPass, HGFXFramebuffer framebuffer)
	{
		VulkanCommandBuffer* pCommandBuffer = static_cast<VulkanCommandBuffer*>(commandBuffer);
		VulkanRenderPass* pVulkanRenderPass = static_cast<VulkanRenderPass*>(renderPass);
		VulkanFramebuffer* pVulkanFramebuffer = static_cast<VulkanFramebuffer*>(framebuffer);

		VkClearValue clearColor = { 0.02f, 0.05f, 0.05f, 1.0f };
		VkClearValue clearDepth = { 1.0f, 0 };

		VkClearValue clearValues[] = { clearColor, clearDepth };

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = pVulkanRenderPass->vkRenderPass;
		renderPassInfo.framebuffer = pVulkanFramebuffer->vkFramebuffer;
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = { pVulkanFramebuffer->width, pVulkanFramebuffer->height };
		renderPassInfo.clearValueCount = 2;
		renderPassInfo.pClearValues = clearValues;

		vkCmdBeginRenderPass(pCommandBuffer->vkCommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void VPLVulkanContext::EndRenderPass(HGFXCommandBuffer commandBuffer)
	{
		VulkanCommandBuffer* pCommandBuffer = static_cast<VulkanCommandBuffer*>(commandBuffer);
		vkCmdEndRenderPass(pCommandBuffer->vkCommandBuffer);
	}

	void VPLVulkanContext::BindGraphicsPipeline(HGFXCommandBuffer commandBuffer, HGFXGraphicsPipeline pipeline, UInt32 bufferIndex)
	{
		VulkanCommandBuffer* pCommandBuffer = static_cast<VulkanCommandBuffer*>(commandBuffer);
		VulkanGraphicsPipeline* pGraphicsPipeline = static_cast<VulkanGraphicsPipeline*>(pipeline);

		vkCmdBindPipeline(pCommandBuffer->vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pGraphicsPipeline->vkPipeline);
	
		// @Todo: This may need to be moved out of binding the pipeline
		vkCmdBindDescriptorSets(pCommandBuffer->vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, 
			pGraphicsPipeline->pipelineInfo.pipelineLayout, 0, 1, &pGraphicsPipeline->descriptorSets[bufferIndex], 0, nullptr);
	}

	//TODO: Implement offsets?
	void VPLVulkanContext::BindVertexBuffers(HGFXCommandBuffer commandBuffer, const Array<HGFXBuffer>& buffers)
	{
		VulkanCommandBuffer* pCommandBuffer = static_cast<VulkanCommandBuffer*>(commandBuffer);

		Array<VkBuffer> vkBuffers(buffers.Size());
		Array<VkDeviceSize> vkOffsetSizes(buffers.Size());

		for (UInt32 i = 0; i < buffers.Size(); i++)
		{
			vkBuffers[i] = static_cast<VulkanBuffer*>(buffers[i])->vkBuffer;
			vkOffsetSizes[i] = 0;
		}

		vkCmdBindVertexBuffers(pCommandBuffer->vkCommandBuffer, 0, buffers.Size(), vkBuffers.Data(), vkOffsetSizes.Data());
	}

	//TODO: Implement offsets and index type?
	void VPLVulkanContext::BindIndexBuffer(HGFXCommandBuffer commandBuffer, HGFXBuffer buffer)
	{
		VulkanCommandBuffer* pCommandBuffer = static_cast<VulkanCommandBuffer*>(commandBuffer);
		VulkanBuffer* pBuffer = static_cast<VulkanBuffer*>(buffer);

		vkCmdBindIndexBuffer(pCommandBuffer->vkCommandBuffer, pBuffer->vkBuffer, 0, VK_INDEX_TYPE_UINT32);
	}

	void VPLVulkanContext::DrawIndexed(HGFXCommandBuffer commandBuffer, UInt32 count, UInt32 start)
	{
		VulkanCommandBuffer* pCommandBuffer = static_cast<VulkanCommandBuffer*>(commandBuffer);
		vkCmdDrawIndexed(pCommandBuffer->vkCommandBuffer, count, 1, start, 0, 0);
	}

	void VPLVulkanContext::Submit(HGFXCommandBuffer commandBuffer, HGFXSwapchain swapchain)
	{
		VulkanCommandBuffer* pCommandBuffer = static_cast<VulkanCommandBuffer*>(commandBuffer);
		VulkanSwapchain* pVulkanSwapchain = static_cast<VulkanSwapchain*>(swapchain);

		VkCommandBuffer commandBuffers[] = { pCommandBuffer->vkCommandBuffer };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		VkSemaphore waitSemaphores[] = { pVulkanSwapchain->imageAcquiredSemaphores[pVulkanSwapchain->frameIndex] };
		VkSemaphore signalSemaphores[] = { pVulkanSwapchain->imageCompleteSemaphores[pVulkanSwapchain->frameIndex] };

		//vkWaitForFences(mpDevice->GetDeviceHandle(), 1, &pVulkanSwapchain->allFences[pVulkanSwapchain->frameIndex], VK_TRUE, UINT64_MAX);
		vkResetFences(mpDevice->GetDeviceHandle(), 1, &pVulkanSwapchain->allFences[pVulkanSwapchain->frameIndex]);

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = commandBuffers;

		VulkanQueue& graphicsQueue = mpDevice->GetGraphicsQueue();

		if (vkQueueSubmit(graphicsQueue.GetQueueHandle(), 1, &submitInfo, pVulkanSwapchain->allFences[pVulkanSwapchain->frameIndex]) != VK_SUCCESS)
		{
			Log::Critical("Failed to submit queue: vkQueueSubmit failed!");
		}
	}

	// Move to Win32 Vulkan
	void VPLVulkanContext::Present(HGFXSwapchain swapchain)
	{
		VulkanSwapchain* pVulkanSwapchain = static_cast<VulkanSwapchain*>(swapchain);

		VkSemaphore signalSemaphores[] = { pVulkanSwapchain->imageCompleteSemaphores[pVulkanSwapchain->frameIndex] };
		VkSwapchainKHR swapChains[] = { pVulkanSwapchain->vkSwapchain };
		UInt32 imageIndices[] = { pVulkanSwapchain->imageIndex };

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = imageIndices;
		presentInfo.pResults = nullptr;

		VulkanQueue& presentQueue = mpDevice->GetPresentQueue();

		vkQueuePresentKHR(presentQueue.GetQueueHandle(), &presentInfo);

		// @TODO: Might not be % imageCount
		pVulkanSwapchain->frameIndex = (pVulkanSwapchain->frameIndex + 1) % pVulkanSwapchain->imageCount;
	}

	void VPLVulkanContext::SetUniformBuffer(HGFXGraphicsPipeline pipeline, UInt32 set, UInt32 binding, HGFXBuffer buffer, UInt32 bufferIndex)
	{
		VulkanGraphicsPipeline* pGraphicsPipeline = static_cast<VulkanGraphicsPipeline*>(pipeline);
		VulkanBuffer* pVulkanBuffer = static_cast<VulkanBuffer*>(buffer);

		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = pVulkanBuffer->vkBuffer;
		bufferInfo.offset = 0;
		bufferInfo.range = pVulkanBuffer->sizeBytes;

		VkWriteDescriptorSet descriptorWrite {};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = pGraphicsPipeline->descriptorSets[bufferIndex];
		descriptorWrite.dstBinding = binding;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = &bufferInfo;

		vkUpdateDescriptorSets(mpDevice->GetDeviceHandle(), 1, &descriptorWrite, 0, nullptr);
	}

	void VPLVulkanContext::SetUniformSampledImage(HGFXGraphicsPipeline pipeline, UInt32 set, UInt32 binding, HGFXSampler sampler, HGFXImageView imageView, UInt32 bufferIndex)
	{
		VulkanGraphicsPipeline* pGraphicsPipeline = static_cast<VulkanGraphicsPipeline*>(pipeline);
		VulkanSampler* pSampler = static_cast<VulkanSampler*>(sampler);
		VulkanImageView* pImageView = static_cast<VulkanImageView*>(imageView);

		VkDescriptorImageInfo imageInfo{};
		imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		imageInfo.imageView = pImageView->vkImageView;
		imageInfo.sampler = pSampler->vkSampler;

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = pGraphicsPipeline->descriptorSets[bufferIndex];
		descriptorWrite.dstBinding = binding;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.pBufferInfo = nullptr;
		descriptorWrite.pImageInfo = &imageInfo;

		vkUpdateDescriptorSets(mpDevice->GetDeviceHandle(), 1, &descriptorWrite, 0, nullptr);
	}

	// Move to Win32 Vulkan
	UInt32 VPLVulkanContext::AcquireSwapchainImageIndex(HGFXSwapchain swapchain)
	{
		VulkanSwapchain* pVulkanSwapchain = static_cast<VulkanSwapchain*>(swapchain);

		vkWaitForFences(mpDevice->GetDeviceHandle(), 1, &pVulkanSwapchain->allFences[pVulkanSwapchain->frameIndex], VK_TRUE, UINT64_MAX);

		UInt32 imageIndex;
		if (vkAcquireNextImageKHR(mpDevice->GetDeviceHandle(), pVulkanSwapchain->vkSwapchain, UINT64_MAX,
			pVulkanSwapchain->imageAcquiredSemaphores[pVulkanSwapchain->frameIndex], VK_NULL_HANDLE, &imageIndex) != VK_SUCCESS)
		{
			Log::Error("Error acquiring next image index: vkAcquireNextImageKHR failed!");
			return (UInt32)-1;
		}

		if (pVulkanSwapchain->imageFenceMap[imageIndex] != VK_NULL_HANDLE)
		{
			vkWaitForFences(mpDevice->GetDeviceHandle(), 1, &pVulkanSwapchain->imageFenceMap[imageIndex], VK_TRUE, UINT64_MAX);
		}

		pVulkanSwapchain->imageFenceMap[imageIndex] = pVulkanSwapchain->allFences[pVulkanSwapchain->frameIndex];
		pVulkanSwapchain->imageIndex = imageIndex;

		return imageIndex;
	}

	HGFXImageView VPLVulkanContext::GetSwapchainImageView(HGFXSwapchain swapchain, UInt32 imageIndex)
	{
		VulkanSwapchain* pVulkanSwapchain = static_cast<VulkanSwapchain*>(swapchain);

		if (pVulkanSwapchain->imageViews.Size() < imageIndex)
		{
			return HGFX_NULL_HANDLE;
		}

		return HGFXImageView(&pVulkanSwapchain->imageViews[imageIndex]);
	}
}

