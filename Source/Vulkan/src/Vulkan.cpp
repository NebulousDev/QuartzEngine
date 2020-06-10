#include "Vulkan.h"
#include "VulkanUtil.h"

//#include "io///Log.h"
#include "gfx/GFXUtil.h"

namespace Quartz
{
	/********************************************************************************************/


	PFN_vkDebugMarkerSetObjectNameEXT vkDebugMarkerSetObjectNameEXT = NULL;
	PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT = NULL;


	/********************************************************************************************/


	Bool8 VulkanGFXModule::InitModuleImpl(const Char** pFailMessage)
	{
		return true;
	}


	/********************************************************************************************/


	Bool8 VulkanGFXModule::DestroyModuleImpl(const Char** pFailMessage)
	{
		return true;
	}


	/********************************************************************************************/


	const Char* VulkanGFXModule::GetModuleNameImpl() const
	{
		return "VulkanGFX";
	}


	/********************************************************************************************/


	void VulkanGFXModule::EnumerateValidationLayers()
	{
		vkEnumerateInstanceLayerProperties(&mAvaliableLayerCount, nullptr);

		mAvailableLayers.Resize(mAvaliableLayerCount);

		vkEnumerateInstanceLayerProperties(&mAvaliableLayerCount, mAvailableLayers.Data());
	}


	/********************************************************************************************/


	Bool8 VulkanGFXModule::CheckValidationLayer(const Char* layerName)
	{
		for (const VkLayerProperties layerProperty : mAvailableLayers)
		{
			if (strcmp(layerName, layerProperty.layerName) == 0)
			{
				return true;
			}
		}

		return false;
	}


	/********************************************************************************************/


	void VulkanGFXModule::EnumerateExtensions()
	{
		vkEnumerateInstanceExtensionProperties(NULL, &mEnabledExtensionCount, NULL);

		mAvailableExtensions.Resize(mEnabledExtensionCount);

		vkEnumerateInstanceExtensionProperties(NULL, &mEnabledExtensionCount, mAvailableExtensions.Data());
	}


	/********************************************************************************************/


	Bool8 VulkanGFXModule::CheckExtension(const Char* extensionName)
	{
		for (const VkExtensionProperties extensionProperty : mAvailableExtensions)
		{
			if (strcmp(extensionName, extensionProperty.extensionName) == 0)
			{
				return true;
			}
		}

		return false;
	}


	/********************************************************************************************/


	void VulkanGFXModule::EnumerateDevices(VkInstance instance)
	{
		Array<VkPhysicalDevice> physicalDevices;
		UInt32 physicalDeviceCount;

		vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);

		physicalDevices.Resize(physicalDeviceCount);
		
		vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.Data());

		for (VkPhysicalDevice device : physicalDevices)
		{
			mPhysicalDeviceList.PushBack(new VulkanGFXPhysicalDevice(device));
		}
	}


	/********************************************************************************************/


	Bool8 VulkanGFXModule::ValidateDevice(GFXPhysicalDevice* pDevice)
	{
		//TODO: Add more requirements
		return true;
	}


	/********************************************************************************************/


	UInt32 VulkanGFXModule::CalcDeviceScore(GFXPhysicalDevice* pDevice)
	{
		UInt32 score = 0;

		const VkPhysicalDeviceProperties& properties 
			= ((VulkanGFXPhysicalDevice*)pDevice)->GetVkPhysicalDeviceProperties();

		if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
		{
			score += 1000;
		}

		score += properties.limits.maxImageDimension2D;

		return score;
	}


	/********************************************************************************************/


	GFXPhysicalDevice* VulkanGFXModule::FindPrimaryPhysicalDevice()
	{
		GFXPhysicalDevice* pPrimaryDevice = NULL;
		UInt32 bestScore = 0;

		for (GFXPhysicalDevice* pDevice : mPhysicalDeviceList)
		{
			UInt32 score = CalcDeviceScore(pDevice);

			if (score > bestScore)
			{
				pPrimaryDevice = pDevice;
				bestScore = score;
			}
		}

		return pPrimaryDevice;
	}


	/********************************************************************************************/


	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT		messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT				messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void*										pUserData)
	{
		if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		{
			//Log.Error(pCallbackData->pMessage);
		}
		else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
		{
			//Log.Warning(pCallbackData->pMessage);
		}
		else
		{
			//Log.Debug(pCallbackData->pMessage);
		}

		return VK_FALSE;
	}


	/********************************************************************************************/


	void SetDebugName(VkDevice device, VkDebugReportObjectTypeEXT objectType, UInt64 object, const Char* name)
	{
		#if USE_DEBUG_NAMES
			
			PFN_vkDebugMarkerSetObjectNameEXT vkDebugMarkerSetObjectNameEXT =
				reinterpret_cast<PFN_vkDebugMarkerSetObjectNameEXT>(vkGetDeviceProcAddr(device, "vkDebugMarkerSetObjectNameEXT"));

			if (vkDebugMarkerSetObjectNameEXT && name)
			{
				VkDebugMarkerObjectNameInfoEXT debugNameInfo = {};
				debugNameInfo.sType = VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT;
				debugNameInfo.pNext = VK_NULL_HANDLE;
				debugNameInfo.objectType = objectType;
				debugNameInfo.object = object;
				debugNameInfo.pObjectName = name;

				if (vkDebugMarkerSetObjectNameEXT(device, &debugNameInfo) != VK_SUCCESS)
				{
					//Log.Warning("Failed to set debug name '%s'", name);
				}
			}

		#endif
	}


	/********************************************************************************************/


	void SetDebugName(GFXDevice* pDevice, VkDebugReportObjectTypeEXT objectType, UInt64 object, const Char* name)
	{
		VkDevice device = static_cast<VulkanGFXDevice*>(pDevice)->GetVkDevice();
		SetDebugName(device, objectType, object, name);
	}


	/********************************************************************************************/


	Bool8 VulkanGFXModule::InitGraphicsContext()
	{
		Bool8 errors = false;

		/* Application Information */

		VkApplicationInfo appInfo	= {};
		appInfo.sType				= VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName	= "Vulkan Test";
		appInfo.applicationVersion	= VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName			= "Quartz Engine";
		appInfo.engineVersion		= VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion			= VK_API_VERSION_1_1;

		/* Validation Layers */

		EnumerateValidationLayers();
		mValidationLayers.PushBack("VK_LAYER_LUNARG_standard_validation");
		mEnabledLayerCount = mValidationLayers.Size();

		/* Extensions */

		EnumerateExtensions();

#if USE_VALIDATION

		mExtensions.PushBack(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		mExtensions.PushBack("VK_EXT_debug_report");

#endif

		mExtensions.PushBack("VK_KHR_surface");
		mExtensions.PushBack("VK_KHR_win32_surface");
		mEnabledExtensionCount = mExtensions.Size();

		/* Debug Callback */

		VkDebugUtilsMessengerCreateInfoEXT debugMessengerInfo = {};
		debugMessengerInfo.sType			= VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		debugMessengerInfo.messageSeverity	= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
												| VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT
												| VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		debugMessengerInfo.messageType		= VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT
												| VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT
												| VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		debugMessengerInfo.pfnUserCallback	= DebugCallback;
		debugMessengerInfo.pUserData		= NULL;

		/* Create Vulkan Instance */

		VkInstanceCreateInfo createInfo		= {};
		createInfo.sType					= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo			= &appInfo;

#if USE_VALIDATION

		createInfo.enabledLayerCount		= mEnabledLayerCount;
		createInfo.ppEnabledLayerNames		= mValidationLayers.Data();
		createInfo.pNext					= (VkDebugUtilsMessengerCreateInfoEXT*)&debugMessengerInfo;

#else

		createInfo.enabledLayerCount = 0;
		createInfo.ppEnabledLayerNames = NULL;

#endif

		createInfo.enabledExtensionCount	= mEnabledExtensionCount;
		createInfo.ppEnabledExtensionNames	= mExtensions.Data();

		VkResult result = vkCreateInstance(&createInfo, NULL, &mVkInstance);

		if (result != VK_SUCCESS)
		{
			//Log.Critical("Failed to create Vulkan instance: vkCreateInstance failed!");
			return false;
		}

		/* Debug Callback Cont. */

#if USE_VALIDATION

		vkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>
			(vkGetInstanceProcAddr(mVkInstance, "vkCreateDebugUtilsMessengerEXT"));

		if (!vkCreateDebugUtilsMessengerEXT)
		{
			//Log.Warning("Failed to address for \"PFN_vkCreateDebugUtilsMessengerEXT\". No validation messages will be shown!!!");
			errors = true;
		}
		else if (vkCreateDebugUtilsMessengerEXT(mVkInstance, &debugMessengerInfo, NULL, &mDebugMessenger) != VK_SUCCESS)
		{
			//Log.Warning("Failed to create validation debug messenger. No validation messages will be shown!!!");
			errors = true;
		}

#endif

		/* Physical Device Creation */

		EnumerateDevices(mVkInstance);

		if (mPhysicalDeviceList.Size() == 0)
		{
			//Log.Warning("No vulkan compatable devices detected!!!");
			errors = true;
		}

		mpPrimaryPhysicalDevice = FindPrimaryPhysicalDevice();

		/* Post-Init */

		mVulkanInitialized = true;

		if (!errors)
		{
			//Log.Info("Initialized Vulkan instance successfully!");
		}
		else
		{
			//Log.Info("Initialized Vulkan instance with warnings/errors.");
		}

		return true;
	}


	/********************************************************************************************/


	Bool8 VulkanGFXModule::DestroyGraphicsContext()
	{
		if (!mVulkanInitialized)
		{
			//Log.Error("Failed to destroy Vulkan instance: Instance was never created!");
			return false;
		}

		PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT =
			(PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(mVkInstance, "vkDestroyDebugUtilsMessengerEXT");

		if (!vkDestroyDebugUtilsMessengerEXT)
		{
			//Log.Warning("Failed to address for \"PFN_vkDestroyDebugUtilsMessengerEXT\". Debug messenger will not be destroyed!");
		}
		else
		{
			vkDestroyDebugUtilsMessengerEXT(mVkInstance, mDebugMessenger, NULL);
		}

		vkDestroyInstance(mVkInstance, nullptr);

		mVulkanInitialized = false;

		//Log.Info("Destroyed Vulkan instance successfully!");

		return true;
	}


	/********************************************************************************************/


	GFXDevice* VulkanGFXModule::CreateDevice(GFXPhysicalDevice* pPhysicalDevice, GFXDeviceInfo info)
	{
		if (!pPhysicalDevice)
		{
			//Log.Warning("Null physical device passed into CreateDevice!");
			return NULL;
		}

		VkPhysicalDevice physicalDevice = static_cast<VulkanGFXPhysicalDevice*>(pPhysicalDevice)->GetVkPhysicalDevice();

		Array<VkDeviceQueueCreateInfo> queueCreateInfos;

		Array<GFXQueueInfoList*> queueArray;
		Array<Array<Float32>*> priorityArray;

		UInt32 queuesCreated = 0;
		UInt32 familyIndex = 0;

		while (queuesCreated < info.pQueueList->Size())
		{
			GFXQueueInfoList* pSubQueueInfoList = new GFXQueueInfoList();
			Array<Float32>* pSubPriorities = new Array<Float32>();

			for (GFXQueueInfo queueInfo : *info.pQueueList)
			{
				if (queueInfo.familyIndex == familyIndex)
				{
					pSubQueueInfoList->PushBack(queueInfo);
					pSubPriorities->PushBack(queueInfo.priority);

					queuesCreated++;
				}
			}

			if (pSubQueueInfoList->Size() > 0)
			{
				VkDeviceQueueCreateInfo queueCreateInfo = {};
				queueCreateInfo.sType					= VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				queueCreateInfo.queueFamilyIndex		= familyIndex;
				queueCreateInfo.queueCount				= pSubQueueInfoList->Size();
				queueCreateInfo.pQueuePriorities		= pSubPriorities->Data();

				queueCreateInfos.PushBack(queueCreateInfo);
			}

			queueArray.PushBack(pSubQueueInfoList);
			priorityArray.PushBack(pSubPriorities);

			familyIndex++;
		}

		VkPhysicalDeviceFeatures deviceFeatures = {};

		Array<const Char*> deviceExtensions;
		deviceExtensions.PushBack(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

#if USE_VALIDATION

		deviceExtensions.PushBack("VK_EXT_debug_marker");

#endif

		VkDeviceCreateInfo deviceInfo		= {};
		deviceInfo.sType					= VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		deviceInfo.pQueueCreateInfos		= queueCreateInfos.Data();
		deviceInfo.queueCreateInfoCount		= queueCreateInfos.Size();
		deviceInfo.enabledExtensionCount	= deviceExtensions.Size();
		deviceInfo.ppEnabledExtensionNames	= deviceExtensions.Data();
		deviceInfo.pEnabledFeatures			= &deviceFeatures;

		VkDevice device;

		if (vkCreateDevice(physicalDevice, &deviceInfo, nullptr, &device) != VK_SUCCESS)
		{
			//Log.Error("Failed to create logical device: vkCreateDevice failed!");
			return NULL;
		}

		for (UInt32 i = 0; i < queueArray.Size(); i++)
		{
			delete queueArray[i];
			delete priorityArray[i];
		}

		GFXQueueMatrix queueMatrix;
		queueMatrix.Resize(familyIndex + 1, GFXQueueList(16)); //TODO: Calculate

		for (const GFXQueueInfo queueInfo : *info.pQueueList)
		{
			VkQueue queue;

			vkGetDeviceQueue(device, queueInfo.familyIndex, queueInfo.queueIndex, &queue);

			SetDebugName(device, VK_DEBUG_REPORT_OBJECT_TYPE_QUEUE_EXT, (UInt64)queue, 
				(String("[Queue] ") + queueInfo.debugName).Str());

			queueMatrix[queueInfo.familyIndex][queueInfo.queueIndex] = new VulkanGFXQueue(queue, queueInfo);
		}

		VulkanGFXDevice* pDevice = new VulkanGFXDevice(device, pPhysicalDevice, queueMatrix);

		// TODO: Find better naming convention for devices
		SetDebugName(pDevice, VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT, (UInt64)device, 
			(String("[Device] ") + pPhysicalDevice->GetDeviceName()).Str());

		return pDevice;
	}


	/********************************************************************************************/


	void VulkanGFXModule::DestroyDevice(GFXDevice* pDevice)
	{
		VkDevice device = static_cast<VulkanGFXDevice*>(pDevice)->GetVkDevice();

		vkDestroyDevice(device, nullptr);

		delete pDevice;
	}


	/********************************************************************************************/


	GFXBuffer* VulkanGFXModule::CreateBuffer(GFXDevice* pDevice, GFXBufferInfo info)
	{
		VkBuffer buffer;
		VkDeviceMemory deviceMemory;

		VkBufferCreateInfo bufferInfo = {};
		bufferInfo.sType		= VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferInfo.size			= info.size;
		bufferInfo.usage		= GetVkUsageFlags(info.usageFlags);
		bufferInfo.sharingMode	= GetVkSharingMode(info.sharingMode);

		VulkanGFXDevice* pVulkanDevice = static_cast<VulkanGFXDevice*>(pDevice);
		VkDevice device = pVulkanDevice->GetVkDevice();

		if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
		{
			//Log.Error("Failed to create buffer: vkCreateBuffer failed!");
			return NULL;
		}

		VulkanGFXPhysicalDevice* pPhysicalDevice = static_cast<VulkanGFXPhysicalDevice*>(pVulkanDevice->GetPhysicalDevice());
		VkPhysicalDeviceMemoryProperties physicalDeviceMemoryProperties = pPhysicalDevice->GetVkPhysicalDeviceMemoryProperties();

		VkMemoryRequirements memoryRequirements;
		vkGetBufferMemoryRequirements(device, buffer, &memoryRequirements);

		VkMemoryPropertyFlags propertyFlags = GetVkMemoryPropertyFlags(info.memoryFlags);
		UInt32 memoryTypeIndex = 0;

		for (UInt32 i = 0; i < physicalDeviceMemoryProperties.memoryTypeCount; i++)
		{
			if ((memoryRequirements.memoryTypeBits & (1 << i)) 
				&& (physicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & propertyFlags) == propertyFlags)
			{
				memoryTypeIndex = i;
			}
		}

		VkMemoryAllocateInfo allocInfo = {};
		allocInfo.sType				= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize	= memoryRequirements.size;
		allocInfo.memoryTypeIndex	= memoryTypeIndex;

		VkResult result;
		if ((result = vkAllocateMemory(device, &allocInfo, VK_NULL_HANDLE, &deviceMemory)) != VK_SUCCESS)
		{
			if (result == VK_ERROR_OUT_OF_HOST_MEMORY)
			{
				//Log.Error("Failed to create buffer: Out of host memory!");
			}
			else if (result == VK_ERROR_OUT_OF_HOST_MEMORY)
			{
				//Log.Error("Failed to create buffer: Out of device memory!");
			}
			else
			{
				//Log.Error("Failed to create buffer: vkAllocateMemory failed!");
			}

			vkDestroyBuffer(device, buffer, VK_NULL_HANDLE);

			return NULL;
		}

		if (vkBindBufferMemory(device, buffer, deviceMemory, 0) != VK_SUCCESS)
		{
			//Log.Error("Failed to create buffer: vkBindBufferMemory failed!");

			vkDestroyBuffer(device, buffer, VK_NULL_HANDLE);
			vkFreeMemory(device, deviceMemory, VK_NULL_HANDLE);

			return NULL;
		}

		SetDebugName(pDevice, VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT, (UInt64)buffer, 
			(String("[BUFFER] ") + info.debugName).Str());

		return new VulkanGFXBuffer(buffer, pDevice, deviceMemory, memoryRequirements, info);
	}


	/********************************************************************************************/


	void VulkanGFXModule::DestroyBuffer(GFXBuffer* pBuffer)
	{
		VkDevice device = static_cast<VulkanGFXDevice*>(pBuffer->GetParentDevice())->GetVkDevice();
		VkBuffer buffer = static_cast<VulkanGFXBuffer*>(pBuffer)->GetVkBuffer();
		VkDeviceMemory memory = static_cast<VulkanGFXBuffer*>(pBuffer)->GetVkDeviceMemory();

		vkDestroyBuffer(device, buffer, VK_NULL_HANDLE);
		vkFreeMemory(device, memory, VK_NULL_HANDLE);
	}

	/********************************************************************************************/

	

	GFXImageView* VulkanGFXModule::CreateImageView(GFXImage* pImage, GFXImageViewInfo info)
	{
		VulkanGFXImage* pVulkanImage = static_cast<VulkanGFXImage*>(pImage);
		VulkanGFXDevice* pVulkanDevice = static_cast<VulkanGFXDevice*>(pVulkanImage->GetParentDevice());
		VkDevice device = pVulkanDevice->GetVkDevice();
		VkImage image = pVulkanImage->GetVkImage();

		VkImageView imageView;

		VkImageViewCreateInfo imageViewInfo				= {};
		imageViewInfo.sType								= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewInfo.image								= image;
		imageViewInfo.viewType							= VK_IMAGE_VIEW_TYPE_2D;
		imageViewInfo.format							= GetVkFormat(info.imageFormat);
		imageViewInfo.components.r						= VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.g						= VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.b						= VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.components.a						= VK_COMPONENT_SWIZZLE_IDENTITY;
		imageViewInfo.subresourceRange.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT;
		imageViewInfo.subresourceRange.baseMipLevel		= 0;
		imageViewInfo.subresourceRange.levelCount		= 1;
		imageViewInfo.subresourceRange.baseArrayLayer	= 0;
		imageViewInfo.subresourceRange.layerCount		= 1;

		if (vkCreateImageView(device, &imageViewInfo, nullptr, &imageView) != VK_SUCCESS)
		{
			//Log.Warning("Failed to create swapchain image views: vkCreateImageView failed!");
		}

		SetDebugName(pVulkanDevice, VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT, (UInt64)imageView, 
			(String("[ImageView] ") + info.debugName).Str());

		return new VulkanGFXImageView(pVulkanImage->GetParentDevice(), pImage, imageView, info);
	}

	/********************************************************************************************/


	void VulkanGFXModule::DestroyImageView(GFXImageView* pImageView)
	{
		VulkanGFXImageView* pVulkanImageView = static_cast<VulkanGFXImageView*>(pImageView);
		VkImageView imageView = pVulkanImageView->GetVkImageView();
		VkDevice device = static_cast<VulkanGFXDevice*>(pImageView->GetImage()->GetParentDevice())->GetVkDevice();

		vkDestroyImageView(device, imageView, VK_NULL_HANDLE);
	}


	/********************************************************************************************/


	GFXShader* VulkanGFXModule::CreateShader(GFXDevice* pDevice, GFXShaderInfo info)
	{
		VkDevice device = static_cast<VulkanGFXDevice*>(pDevice)->GetVkDevice();

		VkShaderModule shaderModule;

		VkShaderModuleCreateInfo moduleInfo = {};
		moduleInfo.sType		= VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		moduleInfo.codeSize		= info.codeSize;
		moduleInfo.pCode		= reinterpret_cast<const UInt32*>(info.pCode);

		if (vkCreateShaderModule(device, &moduleInfo, nullptr, &shaderModule) != VK_SUCCESS)
		{
			//Log.Error("Failed to create shader '%s': vkCreateShaderModule failed!", info.debugName);
		}

		VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
		vertShaderStageInfo.sType	= VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage	= GetVkShaderStageFlagBits(info.shaderStage);
		vertShaderStageInfo.module	= shaderModule;
		vertShaderStageInfo.pName	= info.entryName;

		SetDebugName(pDevice, VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT, (UInt64)shaderModule, 
			(String("[Shader] ") + info.debugName).Str());

		return new VulkanGFXShader(pDevice, shaderModule, vertShaderStageInfo, info);
	}


	/********************************************************************************************/


	void VulkanGFXModule::DestroyShader(GFXShader* pShader)
	{
		VkDevice device = static_cast<VulkanGFXDevice*>(pShader->GetParentDevice())->GetVkDevice();
		VkShaderModule module = static_cast<VulkanGFXShader*>(pShader)->GetVkShaderModule();

		vkDestroyShaderModule(device, module, VK_NULL_HANDLE);
	}


	/********************************************************************************************/


	GFXVertexInputState* VulkanGFXModule::CreateVertexInputState(GFXVertexInputStateInfo info)
	{
		VkVertexInputBindingDescription*	pVertexInputBindingDescriptions		= NULL;
		VkVertexInputAttributeDescription*	pVertexInputAttributeDescriptions	= NULL;

		if (info.vertexBindingCount)
		{
			pVertexInputBindingDescriptions = new VkVertexInputBindingDescription[info.vertexBindingCount];

			for (UInt32 i = 0; i < info.vertexBindingCount; i++)
			{
				pVertexInputBindingDescriptions[0].binding		= info.pVertexBindingInfos[0].binding;
				pVertexInputBindingDescriptions[0].stride		= info.pVertexBindingInfos[0].stride;
				pVertexInputBindingDescriptions[0].inputRate	= GetVkVertexInputRate(info.pVertexBindingInfos[0].inputRate);
			}
		}

		if (info.vertexAttributeCount)
		{
			pVertexInputAttributeDescriptions = new VkVertexInputAttributeDescription[info.vertexBindingCount];

			for (UInt32 i = 0; i < info.vertexAttributeCount; i++)
			{
				pVertexInputAttributeDescriptions[0].binding	= info.pVertexAttributeInfos[0].binding;
				pVertexInputAttributeDescriptions[0].location	= info.pVertexAttributeInfos[0].location;
				pVertexInputAttributeDescriptions[0].format		= GetVkFormat(info.pVertexAttributeInfos[0].format);
				pVertexInputAttributeDescriptions[0].offset		= info.pVertexAttributeInfos[0].offset;
			}
		}

		VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
		vertexInputInfo.sType							= VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount	= info.vertexBindingCount;
		vertexInputInfo.pVertexBindingDescriptions		= pVertexInputBindingDescriptions;
		vertexInputInfo.vertexAttributeDescriptionCount = info.vertexAttributeCount;
		vertexInputInfo.pVertexAttributeDescriptions	= pVertexInputAttributeDescriptions;

		return new VulkanGFXVertexInputState(vertexInputInfo, pVertexInputBindingDescriptions, pVertexInputAttributeDescriptions, info);
	}


	/********************************************************************************************/


	void VulkanGFXModule::DestroyVertexInputState(GFXVertexInputState* pVertexInputState)
	{
		VulkanGFXVertexInputState* pVulkanGFXVertexInputState = static_cast<VulkanGFXVertexInputState*>(pVertexInputState);

		delete[] pVulkanGFXVertexInputState->GetVkVertexInputBindingDescriptions();
		delete[] pVulkanGFXVertexInputState->GetVkVertexInputAttributeDescriptions();
		delete pVertexInputState;
	}


	/********************************************************************************************/


	GFXInputAssemblyState* VulkanGFXModule::CreateInputAssemblyState(GFXInputAssemblyStateInfo info)
	{
		VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
		inputAssembly.sType						= VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology					= GetVkPrimitiveTopology(info.topology);
		inputAssembly.primitiveRestartEnable	= VK_FALSE;

		return new VulkanGFXInputAssemblyState(inputAssembly, info);
	}


	/********************************************************************************************/


	void VulkanGFXModule::DestroyInputAssemblyState(GFXInputAssemblyState* pInputAssemblyState)
	{
		delete pInputAssemblyState;
	}


	/********************************************************************************************/


	GFXViewportState* VulkanGFXModule::CreateViewportState(GFXViewportStateInfo info)
	{
		VkViewport* pViewport	= new VkViewport {};
		pViewport->x			= info.viewport.x;
		pViewport->y			= info.viewport.y;
		pViewport->width		= info.viewport.width;
		pViewport->height		= info.viewport.height;
		pViewport->minDepth		= info.viewport.minDepth;
		pViewport->maxDepth		= info.viewport.maxDepth;

		VkRect2D* pScissor	= new VkRect2D {};
		pScissor->offset	= { info.scissor.x, info.scissor.y };
		pScissor->extent	= { info.scissor.width , info.scissor.height };

		VkPipelineViewportStateCreateInfo viewportState = {};
		viewportState.sType				= VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount		= 1;
		viewportState.pViewports		= pViewport;
		viewportState.scissorCount		= 1;
		viewportState.pScissors			= pScissor;

		return new VulkanGFXViewportState(viewportState, pViewport, pScissor, info);
	}


	/********************************************************************************************/


	void VulkanGFXModule::DestroyViewportState(GFXViewportState* pViewportState)
	{
		delete pViewportState;
	}


	/********************************************************************************************/


	GFXRasterizationState* VulkanGFXModule::CreateRasterizationState(GFXRasterizationStateInfo info)
	{
		VkPipelineRasterizationStateCreateInfo rasterizationState = {};
		rasterizationState.sType					= VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizationState.depthClampEnable			= VK_FALSE;
		rasterizationState.rasterizerDiscardEnable	= VK_FALSE;
		rasterizationState.polygonMode				= GetVkPolygonMode(info.fillMode);
		rasterizationState.lineWidth				= info.lineWidth;
		rasterizationState.cullMode					= GetVkCullModeFlagBits(info.cullFace);
		rasterizationState.frontFace				= GetVkFrontFace(info.frontFace);
		rasterizationState.depthBiasEnable			= VK_FALSE;
		rasterizationState.depthBiasConstantFactor	= 0.0f;
		rasterizationState.depthBiasClamp			= 0.0f;
		rasterizationState.depthBiasSlopeFactor		= 0.0f;

		return new VulkanGFXRasterizationState(rasterizationState, info);
	}


	/********************************************************************************************/


	void VulkanGFXModule::DestroyRasterizationState(GFXRasterizationState* pRasterizationState)
	{
		delete pRasterizationState;
	}


	/********************************************************************************************/


	GFXMultisampleState* VulkanGFXModule::CreateMultisampleState(GFXMultisampleStateInfo info)
	{
		VkPipelineMultisampleStateCreateInfo multisampleState = {};
		multisampleState.sType					= VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampleState.sampleShadingEnable	= info.enableMultisampling;
		multisampleState.rasterizationSamples	= GetVkSampleCountFlagBits(info.multisamples);
		multisampleState.minSampleShading		= 1.0f;
		multisampleState.pSampleMask			= NULL; 
		multisampleState.alphaToCoverageEnable	= VK_FALSE;
		multisampleState.alphaToOneEnable		= VK_FALSE;

		return new VulkanGFXMultisampleState(multisampleState, info);
	}


	/********************************************************************************************/


	void VulkanGFXModule::DestroyMultisampleState(GFXMultisampleState* pMultisampleState)
	{
		delete pMultisampleState;
	}


	/********************************************************************************************/


	GFXColorBlendState* VulkanGFXModule::CreateColorBlendState(GFXColorBlendStateInfo info)
	{
		VkPipelineColorBlendAttachmentState* pColorBlendAttachments = new VkPipelineColorBlendAttachmentState[info.attachmentCount];

		for (UInt32 i = 0; i < info.attachmentCount; i++)
		{
			pColorBlendAttachments[i] = {};
			pColorBlendAttachments[i].colorWriteMask		= VkGetColorComponentFlags(info.pAttachments[i].colorMask);
			pColorBlendAttachments[i].blendEnable			= info.pAttachments[i].blendEnable;
			pColorBlendAttachments[i].srcColorBlendFactor	= GetVkBlendFactor(info.pAttachments[i].srcColorBlendFactor);
			pColorBlendAttachments[i].dstColorBlendFactor	= GetVkBlendFactor(info.pAttachments[i].destColorBlendFactor);
			pColorBlendAttachments[i].colorBlendOp			= GetVkBlendOp(info.pAttachments[i].colorBlendOp);
			pColorBlendAttachments[i].srcAlphaBlendFactor	= GetVkBlendFactor(info.pAttachments[i].srcAlphaBlendFactor);
			pColorBlendAttachments[i].dstAlphaBlendFactor	= GetVkBlendFactor(info.pAttachments[i].destAlphaBlendFactor);
			pColorBlendAttachments[i].alphaBlendOp			= GetVkBlendOp(info.pAttachments[i].alphaBlendOp);
		}

		VkPipelineColorBlendStateCreateInfo colorBlending = {};
		colorBlending.sType				= VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable		= info.logicalOpEnable;
		colorBlending.logicOp			= GetVkLogicalOp(info.logicalOp);
		colorBlending.attachmentCount	= info.attachmentCount;
		colorBlending.pAttachments		= pColorBlendAttachments;
		colorBlending.blendConstants[0] = info.blendConstants[0];
		colorBlending.blendConstants[1] = info.blendConstants[1];
		colorBlending.blendConstants[2] = info.blendConstants[2];
		colorBlending.blendConstants[3] = info.blendConstants[3];

		return new VulkanGFXColorBlendState(colorBlending, pColorBlendAttachments, info);
	}


	/********************************************************************************************/


	void VulkanGFXModule::DestroyColorBlendState(GFXColorBlendState* pColorBlendState)
	{
		delete pColorBlendState;
	}


	/********************************************************************************************/


	GFXPipelineLayout* VulkanGFXModule::CreatePipelineLayout(GFXDevice* pDevice, GFXPipelineLayoutInfo info)
	{
		VkDevice device = static_cast<VulkanGFXDevice*>(pDevice)->GetVkDevice();

		VkDescriptorSetLayout* pDescriptorSetLayouts = nullptr;

		if (info.layoutCount > 0)
		{
			pDescriptorSetLayouts = new VkDescriptorSetLayout[info.layoutCount];

			for (UInt32 i = 0; i < info.layoutCount; i++)
			{
				VulkanGFXDescriptorSetLayout* pDescriptorSetLayout = static_cast<VulkanGFXDescriptorSetLayout*>(&info.pDescriptorSetLayouts[i]);
				pDescriptorSetLayouts[i] = pDescriptorSetLayout->GetVkDescriptorSetLayout();
			}
		}

		VkPipelineLayout pipelineLayout;

		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
		pipelineLayoutInfo.sType					= VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount			= info.layoutCount;
		pipelineLayoutInfo.pSetLayouts				= pDescriptorSetLayouts;
		pipelineLayoutInfo.pushConstantRangeCount	= 0;
		pipelineLayoutInfo.pPushConstantRanges		= nullptr;

		if (vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		{
			//Log.Error("Failed to create pipeline layout: vkCreatePipelineLayout failed!");

			delete[] pDescriptorSetLayouts;

			return NULL;
		}

		delete[] pDescriptorSetLayouts; //TODO

		return new VulkanGFXPipelineLayout(pDevice, pipelineLayout, info);
	}


	/********************************************************************************************/


	void VulkanGFXModule::DestroyPipelineLatout(GFXPipelineLayout* pPipelineLayout)
	{
		VkDevice device = static_cast<VulkanGFXDevice*>(pPipelineLayout->GetParentDevice())->GetVkDevice();
		VkPipelineLayout pipelineLayout = static_cast<VulkanGFXPipelineLayout*>(pPipelineLayout)->GetVkPipelineLayout();

		vkDestroyPipelineLayout(device, pipelineLayout, VK_NULL_HANDLE);
	}


	/********************************************************************************************/


	GFXRenderAttachment* VulkanGFXModule::CreateRenderAttachment(GFXRenderAttachmentInfo info)
	{
		VkAttachmentDescription attachment = {};
		attachment.format			= GetVkFormat(info.imageFormat);
		attachment.samples			= GetVkSampleCountFlagBits(info.multisamples);
		attachment.loadOp			= GetVkAttachmentLoadOp(info.loadOp);
		attachment.storeOp			= GetVkAttachmentStoreOp(info.storeOp);
		attachment.stencilLoadOp	= GetVkAttachmentLoadOp(info.stencilLoadOp);
		attachment.stencilStoreOp	= GetVkAttachmentStoreOp(info.stencilStoreOp);
		attachment.initialLayout	= GetVkImageLayout(info.initialLayout);
		attachment.finalLayout		= GetVkImageLayout(info.finalLayout);

		return new VulkanGFXRenderAttachment(attachment, info);
	}


	/********************************************************************************************/


	void VulkanGFXModule::DestroyRenderAttachment(GFXRenderAttachment* pRenderAttachment)
	{
		delete pRenderAttachment;
	}


	/********************************************************************************************/


	GFXRenderSubpass* VulkanGFXModule::CreateRenderSubpass(GFXRenderSubpassInfo info)
	{
		VkAttachmentReference* pInputAttachments		= NULL; 
		VkAttachmentReference* pColorAttachments		= NULL;
		VkAttachmentReference* pResolveAttachments		= NULL;
		VkAttachmentReference* pDepthStencilAttachment	= NULL;
		
		if (info.inputAttachmentCount > 0 && info.pInputAttachments)
		{
			pInputAttachments = new VkAttachmentReference[info.inputAttachmentCount];

			for (UInt32 i = 0; i < info.inputAttachmentCount; i++)
			{
				pInputAttachments[i].attachment = info.pInputAttachments[i].index;
				pInputAttachments[i].layout = GetVkImageLayout(info.pInputAttachments[i].layout);
			}
		}

		if (info.colorAttachmentCount > 0 && info.pColorAttachments)
		{
			pColorAttachments = new VkAttachmentReference[info.colorAttachmentCount];

			for (UInt32 i = 0; i < info.colorAttachmentCount; i++)
			{
				pColorAttachments[i].attachment = info.pColorAttachments[i].index;
				pColorAttachments[i].layout = GetVkImageLayout(info.pColorAttachments[i].layout);
			}
		}

		if (info.colorAttachmentCount > 0 && info.pResolveAttachments)
		{
			pResolveAttachments = new VkAttachmentReference[info.colorAttachmentCount];

			for (UInt32 i = 0; i < info.colorAttachmentCount; i++)
			{
				pResolveAttachments[i].attachment = info.pResolveAttachments[i].index;
				pResolveAttachments[i].layout = GetVkImageLayout(info.pResolveAttachments[i].layout);
			}
		}
		
		if (info.pDepthStencilAttachment)
		{
			pDepthStencilAttachment = new VkAttachmentReference;
			pDepthStencilAttachment->attachment = info.pDepthStencilAttachment->index;
			pDepthStencilAttachment->layout = GetVkImageLayout(info.pDepthStencilAttachment->layout);
		}

		VkSubpassDescription subpass	= {};
		subpass.pipelineBindPoint		= GetVkPipelineBindPoint(info.bindPoint);
		subpass.inputAttachmentCount	= info.inputAttachmentCount;
		subpass.pInputAttachments		= pInputAttachments;
		subpass.colorAttachmentCount	= info.colorAttachmentCount;
		subpass.pColorAttachments		= pColorAttachments;
		subpass.pResolveAttachments		= pResolveAttachments;
		subpass.pDepthStencilAttachment = pDepthStencilAttachment;
		subpass.preserveAttachmentCount = info.preservedAttachmentCount;
		subpass.pPreserveAttachments	= info.pPreservedAttachmentIndices;

		return new VulkanGFXRenderSubpass(subpass, pInputAttachments, pColorAttachments, 
			pResolveAttachments, pDepthStencilAttachment, info);
	}


	/********************************************************************************************/


	void VulkanGFXModule::DestroyRenderSubpass(GFXRenderSubpass* pRenderSubpass)
	{
		delete pRenderSubpass;
	}


	/********************************************************************************************/


	GFXRenderPass* VulkanGFXModule::CreateRenderPass(GFXDevice* pDevice, GFXRenderPassInfo info)
	{
		VkDevice device = static_cast<VulkanGFXDevice*>(pDevice)->GetVkDevice();

		VkAttachmentDescription*	pAttachments = NULL;
		VkSubpassDependency*		pDependencies = NULL;
		VkSubpassDescription*		pSubpasses = NULL;

		if (info.attachmentCount > 0 && info.ppAttachments)
		{
			pAttachments = new VkAttachmentDescription[info.attachmentCount]{};

			for (UInt32 i = 0; i < info.attachmentCount; i++)
			{
				VulkanGFXRenderAttachment* pAttachment = static_cast<VulkanGFXRenderAttachment*>(info.ppAttachments[i]);
				pAttachments[i] = pAttachment->GetVkAttachmentDescription();
			}
		}

		if (info.subpassCount > 0 && info.ppSubpasses)
		{
			pSubpasses = new VkSubpassDescription[info.subpassCount]{};

			for (UInt32 i = 0; i < info.subpassCount; i++)
			{
				VulkanGFXRenderSubpass* pSubpass = static_cast<VulkanGFXRenderSubpass*>(info.ppSubpasses[i]);
				pSubpasses[i] = pSubpass->GetVkSubpassDescription();
			}
		}

		if (info.dependencyCount > 0 && info.pDependencies)
		{
			pDependencies = new VkSubpassDependency[info.dependencyCount]{};

			for (UInt32 i = 0; i < info.dependencyCount; i++)
			{
				pDependencies[i].srcSubpass		= info.pDependencies[i].srcSubpassIndex;
				pDependencies[i].dstSubpass		= info.pDependencies[i].destSubpassIndex;
				pDependencies[i].srcStageMask	= GetVkPipelineStageFlags(info.pDependencies[i].srcStageMask);
				pDependencies[i].dstStageMask	= GetVkPipelineStageFlags(info.pDependencies[i].destStageMask);
				pDependencies[i].srcAccessMask	= GetVkAccessFlags(info.pDependencies[i].srcAccessMask);
				pDependencies[i].dstAccessMask	= GetVkAccessFlags(info.pDependencies[i].destAccessMask);
			}
		}

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType			= VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount	= info.attachmentCount;
		renderPassInfo.pAttachments		= pAttachments;
		renderPassInfo.subpassCount		= info.subpassCount;
		renderPassInfo.pSubpasses		= pSubpasses;
		renderPassInfo.dependencyCount	= info.dependencyCount;
		renderPassInfo.pDependencies	= pDependencies;

		VkRenderPass renderPass;

		if (vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS)
		{
			//Log.Error("Failed to create renderpass: vkCreateRenderPass failed!");

			delete[] pAttachments;
			delete[] pDependencies;
			delete[] pSubpasses;

			return NULL;
		}

		return new VulkanGFXRenderPass(pDevice, renderPass, pAttachments, pDependencies, pSubpasses, info);
	}


	/********************************************************************************************/


	void VulkanGFXModule::DestroyRenderPass(GFXRenderPass* pRenderPass)
	{
		delete pRenderPass;
	}


	/********************************************************************************************/


	GFXDescriptorSetLayout* VulkanGFXModule::CreateDescriptorSetLayout(GFXDevice* pDevice, GFXDescriptorSetLayoutInfo info)
	{
		VkDevice device = static_cast<VulkanGFXDevice*>(pDevice)->GetVkDevice();

		VkDescriptorSetLayoutBinding* pDescriptorSetLayoutBindings = new VkDescriptorSetLayoutBinding[info.bindingCount];

		for (UInt32 i = 0; i < info.bindingCount; i++)
		{
			pDescriptorSetLayoutBindings[i] = {};
			pDescriptorSetLayoutBindings[i].binding = info.pBindings[i].binding;
			pDescriptorSetLayoutBindings[i].descriptorType = GetVkDescriptorType(info.pBindings[i].type);
			pDescriptorSetLayoutBindings[i].descriptorCount = info.pBindings[i].count;
			pDescriptorSetLayoutBindings[i].stageFlags = GetVkShaderStageFlags(info.pBindings[i].stageFlags);
			pDescriptorSetLayoutBindings[i].pImmutableSamplers = NULL;
		}

		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = info.bindingCount;
		layoutInfo.pBindings = pDescriptorSetLayoutBindings;

		VkDescriptorSetLayout descriptorSetLayout;

		if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS)
		{
			//Log.Error("Failed to create descriptor set layout : vkCreateDescriptorSetLayout failed!");

			delete[] pDescriptorSetLayoutBindings;

			return NULL;
		}

		return new VulkanGFXDescriptorSetLayout(pDevice, descriptorSetLayout, pDescriptorSetLayoutBindings, info);
	}


	/********************************************************************************************/


	void VulkanGFXModule::DestroyDescriptorSetLayout(GFXDescriptorSetLayout* pLayout)
	{
		VkDevice device = static_cast<VulkanGFXDevice*>(pLayout->GetParentDevice())->GetVkDevice();
		VulkanGFXDescriptorSetLayout* pDescriptorSetLayout = static_cast<VulkanGFXDescriptorSetLayout*>(pLayout);

		delete[] pDescriptorSetLayout->GetDescriptorBindings();

		vkDestroyDescriptorSetLayout(device, pDescriptorSetLayout->GetVkDescriptorSetLayout(), NULL);

		delete pLayout;
	}


	/********************************************************************************************/


	GFXPipeline* VulkanGFXModule::CreatePipeline(GFXDevice* pDevice, GFXPipelineInfo info)
	{
		VkDevice device = static_cast<VulkanGFXDevice*>(pDevice)->GetVkDevice();

		VkPipelineShaderStageCreateInfo* pShaderStages = NULL;

		if (info.shaderCount > 0 && info.ppShaders)
		{
			pShaderStages = new VkPipelineShaderStageCreateInfo[info.shaderCount]{};

			for (UInt32 i = 0; i < info.shaderCount; i++)
			{
				VulkanGFXShader* pShader = static_cast<VulkanGFXShader*>(info.ppShaders[i]);
				pShaderStages[i] = pShader->GetVkPipelineShaderStageCreateInfo();
			}
		}

		VulkanGFXVertexInputState* pVertexInputState = static_cast<VulkanGFXVertexInputState*>(info.pVertexInputState);
		VulkanGFXInputAssemblyState* pInputAssembly = static_cast<VulkanGFXInputAssemblyState*>(info.pInputAssemblyState);
		VulkanGFXViewportState* pViewportState = static_cast<VulkanGFXViewportState*>(info.pViewportState);
		VulkanGFXRasterizationState* pRasterizationState = static_cast<VulkanGFXRasterizationState*>(info.pRasterizationState);
		VulkanGFXMultisampleState* pMultisampleState = static_cast<VulkanGFXMultisampleState*>(info.pMultisampleState);
		VulkanGFXColorBlendState* pColorBlendState = static_cast<VulkanGFXColorBlendState*>(info.pColorBlendState);
		VulkanGFXPipelineLayout* pPipelineLayout = static_cast<VulkanGFXPipelineLayout*>(info.pPipelineLayout);
		VulkanGFXRenderPass* pRenderPass = static_cast<VulkanGFXRenderPass*>(info.pRenderPass);

		// Why the do these need to exist? Drives me insane...
		VkPipelineVertexInputStateCreateInfo* pLocalVertexInputState = new VkPipelineVertexInputStateCreateInfo(pVertexInputState->GetVkVertexInputStateInfo());
		VkPipelineInputAssemblyStateCreateInfo* pLocalInputAssemblyState = new VkPipelineInputAssemblyStateCreateInfo(pInputAssembly->GetVkInputAssemblyStateInfo());
		VkPipelineViewportStateCreateInfo* pLocalViewportState = new VkPipelineViewportStateCreateInfo(pViewportState->GetVkViewportStateInfo());
		VkPipelineRasterizationStateCreateInfo* pLocalRaterizationState = new VkPipelineRasterizationStateCreateInfo(pRasterizationState->GetVkRasterizationStateInfo());
		VkPipelineMultisampleStateCreateInfo* pLocalMultisampleState = new VkPipelineMultisampleStateCreateInfo(pMultisampleState->GetVkMultisampleStateInfo());
		VkPipelineColorBlendStateCreateInfo* pLocalColorBlendState = new VkPipelineColorBlendStateCreateInfo(pColorBlendState->GetVkColorBlendStateInfo());

		/////////////////////////////////////////////////////////////////////////////////////

		VkDynamicState dynamicStates[] = {
			VK_DYNAMIC_STATE_VIEWPORT,
			VK_DYNAMIC_STATE_LINE_WIDTH
		};

		VkPipelineDynamicStateCreateInfo dynamicState = {};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = 2;
		dynamicState.pDynamicStates = dynamicStates;

		VkPipelineLayout pipelineLayout = static_cast<VulkanGFXPipelineLayout*>(info.pPipelineLayout)->GetVkPipelineLayout();

		/////////////////////////////////////////////////////////////////////////////////////

		VkGraphicsPipelineCreateInfo pipelineInfo = {};
		pipelineInfo.sType					= VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount				= info.shaderCount;
		pipelineInfo.pStages				= pShaderStages;
		pipelineInfo.pVertexInputState		= pLocalVertexInputState;
		pipelineInfo.pInputAssemblyState	= pLocalInputAssemblyState;
		pipelineInfo.pViewportState			= pLocalViewportState;
		pipelineInfo.pRasterizationState	= pLocalRaterizationState;
		pipelineInfo.pMultisampleState		= pLocalMultisampleState;
		pipelineInfo.pDepthStencilState		= nullptr; // Optional
		pipelineInfo.pColorBlendState		= pLocalColorBlendState;
		pipelineInfo.pDynamicState			= nullptr; // Optional
		pipelineInfo.layout					= pipelineLayout;
		pipelineInfo.renderPass				= pRenderPass->GetVkRenderPass();
		pipelineInfo.subpass				= info.subpassIndex;
		pipelineInfo.basePipelineHandle		= VK_NULL_HANDLE; // Optional
		pipelineInfo.basePipelineIndex		= -1; // Optional

		VkPipeline pipeline;

		if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS)
		{
			//Log.Error("Failed to create pipeline: vkCreateGraphicsPipelines failed!");

			delete[] pShaderStages;

			delete pLocalVertexInputState;
			delete pLocalInputAssemblyState;
			delete pLocalViewportState;
			delete pLocalRaterizationState;
			delete pLocalMultisampleState;
			delete pLocalColorBlendState;

			return NULL;
		}

		// Like.. why
		delete pLocalVertexInputState;
		delete pLocalInputAssemblyState;
		delete pLocalViewportState;
		delete pLocalRaterizationState;
		delete pLocalMultisampleState;
		delete pLocalColorBlendState;

		return new VulkanGFXPipeline(pDevice, pipeline, pShaderStages, info);
	}


	/********************************************************************************************/


	void VulkanGFXModule::DestroyPipleline(GFXPipeline* pPipeline)
	{
		//TODO: delete shader stages / etc?
		delete pPipeline;
	}


	/********************************************************************************************/


	GFXFramebuffer* VulkanGFXModule::CreateFramebuffer(GFXDevice* pDevice, GFXFramebufferInfo info)
	{
		VkDevice device = static_cast<VulkanGFXDevice*>(pDevice)->GetVkDevice();

		VkImageView pAttachments[16] = {};

		if (info.attachmentCount > 16)
		{
			//Log.Error("Failed to create framebuffer: cannot attach more than 16 image views!");
			return NULL;
		}

		for (UInt32 i = 0; i < info.attachmentCount; i++)
		{
			pAttachments[i] = static_cast<VulkanGFXImageView*>(info.ppAttachments[i])->GetVkImageView();
		}

		VkFramebufferCreateInfo framebufferInfo = {};
		framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferInfo.renderPass = static_cast<VulkanGFXRenderPass*>(info.pRenderPass)->GetVkRenderPass();
		framebufferInfo.attachmentCount = info.attachmentCount;
		framebufferInfo.pAttachments = pAttachments;
		framebufferInfo.width = info.width;
		framebufferInfo.height = info.height;
		framebufferInfo.layers = info.layers;

		VkFramebuffer framebuffer;

		if (vkCreateFramebuffer(device, &framebufferInfo, nullptr, &framebuffer) != VK_SUCCESS)
		{
			//Log.Error("Failed to create framebuffer: vkCreateFramebuffer failed!");
			return NULL;
		}

		return new VulkanGFXFramebuffer(pDevice, framebuffer, info);
	}


	/********************************************************************************************/


	void VulkanGFXModule::DestroyFramebuffer(GFXFramebuffer* pFramebuffer)
	{
		//TODO
	}


	/********************************************************************************************/


	GFXCommandPool* VulkanGFXModule::CreateCommandPool(GFXDevice* pDevice, GFXCommandPoolInfo info)
	{
		VkDevice device = static_cast<VulkanGFXDevice*>(pDevice)->GetVkDevice();

		VkCommandPool commandPool;

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = info.pFamily->GetFamilyIndex();
		poolInfo.flags = info.flags; //TODO: Make a wrapper for flags

		if (vkCreateCommandPool(device, &poolInfo, VK_NULL_HANDLE, &commandPool) != VK_SUCCESS)
		{
			//Log.Error("Failed to create command pool: vkCreateCommandPool failed!");
		}

		return new VulkanGFXCommandPool(pDevice, commandPool, info);
	}


	/********************************************************************************************/


	void VulkanGFXModule::DestroyCommandPool(GFXCommandPool* pCommandPool)
	{
		VkDevice device = static_cast<VulkanGFXDevice*>(pCommandPool->GetParentDevice())->GetVkDevice();
		VkCommandPool commandPool = static_cast<VulkanGFXCommandPool*>(pCommandPool)->GetVkCommandPool();

		vkDestroyCommandPool(device, commandPool, VK_NULL_HANDLE);

		delete pCommandPool;
	}


	/********************************************************************************************/


	Bool8 VulkanGFXModule::MapBufferMemory(GFXBuffer* pBuffer, USize offset, USize size, GFXBufferMapFlags flags, void** ppData)
	{
		VkDevice device = static_cast<VulkanGFXDevice*>(pBuffer->GetParentDevice())->GetVkDevice();
		VkBuffer buffer = static_cast<VulkanGFXBuffer*>(pBuffer)->GetVkBuffer();
		VkDeviceMemory memory = static_cast<VulkanGFXBuffer*>(pBuffer)->GetVkDeviceMemory();

		if (vkMapMemory(device, memory, offset, size, 0, ppData) != VK_SUCCESS)
		{
			//Log.Error("Failed map buffer memory: vkMapMemory failed!");
			return false;
		}

		return true;
	}


	/********************************************************************************************/


	Bool8 VulkanGFXModule::UnmapBufferMemory(GFXBuffer* pBuffer)
	{
		VkDevice device = static_cast<VulkanGFXDevice*>(pBuffer->GetParentDevice())->GetVkDevice();
		VkDeviceMemory memory = static_cast<VulkanGFXBuffer*>(pBuffer)->GetVkDeviceMemory();

		vkUnmapMemory(device, memory);

		return true;
	}


	/********************************************************************************************/


	const GFXPhysicalDeviceList& VulkanGFXModule::GetPhysicalDeviceList() const
	{
		return mPhysicalDeviceList;
	}


	/********************************************************************************************/


	GFXPhysicalDevice* VulkanGFXModule::GetPrimaryPhysicalDevice()
	{
		return mpPrimaryPhysicalDevice;
	}


	/********************************************************************************************/


	VulkanGFXQueueFamily::VulkanGFXQueueFamily(VkQueueFamilyProperties queueFamilyProperties, UInt32 index)
	{
		mQueueFamilyProperties = queueFamilyProperties;
		mFamilyIndex = index;
		mQueueCount = queueFamilyProperties.queueCount;

		mQueueFlags = 0;

		// TODO: move this:
		if (queueFamilyProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT)		mQueueFlags |= GFX_QUEUE_TYPE_GRAPHICS_BIT;
		if (queueFamilyProperties.queueFlags & VK_QUEUE_COMPUTE_BIT)		mQueueFlags |= GFX_QUEUE_TYPE_COMPUTE_BIT;
		if (queueFamilyProperties.queueFlags & VK_QUEUE_TRANSFER_BIT)		mQueueFlags |= GFX_QUEUE_TYPE_TRANSFER_BIT;
		if (queueFamilyProperties.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) mQueueFlags |= GFX_QUEUE_TYPE_BINDING_BIT;
	}


	/********************************************************************************************/


	//TODO: Move creating out of constuctor
	VulkanGFXPhysicalDevice::VulkanGFXPhysicalDevice(VkPhysicalDevice device)
	{
		mPhysicalDevice = device;

		vkGetPhysicalDeviceProperties(device, &mPhysicalDeviceProperties);
		vkGetPhysicalDeviceFeatures(device, &mPhysicalDeviceFeatures);
		vkGetPhysicalDeviceMemoryProperties(device, &mPhysicalDeviceMemoryProperties);

		UInt32 queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, VK_NULL_HANDLE);

		Array<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.Data());

		GFXQueueFamilyList queueFamilyList(queueFamilyCount);

		UInt32 index = 0;
		for (VkQueueFamilyProperties queueFamily : queueFamilies)
		{
			queueFamilyList[index++] = new VulkanGFXQueueFamily(queueFamily, index);
		}

		mDeviceName			= mPhysicalDeviceProperties.deviceName;
		mVendorName			= GFXDeviceVendorIDName(mPhysicalDeviceProperties.vendorID);
		mDeviceID			= mPhysicalDeviceProperties.deviceID;
		mVendorID			= mPhysicalDeviceProperties.vendorID;
		mApiVersion			= mPhysicalDeviceProperties.apiVersion;
		mDriverVersion		= mPhysicalDeviceProperties.driverVersion;
		mQueueFamilyList	= queueFamilyList;
	}


	/********************************************************************************************/


	VulkanGFXDevice::VulkanGFXDevice(VkDevice device, GFXPhysicalDevice* pPhysicalDevice, GFXQueueMatrix queueMatrix)
	{
		mVkDevice = device;
		mpPhysicalDevice = pPhysicalDevice;
		mQueueMatrix = queueMatrix;
	}


	/********************************************************************************************/


	VulkanGFXQueue::VulkanGFXQueue(VkQueue queue, GFXQueueInfo info)
	{
		mDebugName		= info.debugName;
		mVkQueue		= queue;
		mQueueFlags		= info.queueFlags;
		mFamilyIndex	= info.familyIndex;
		mPriority		= info.priority;
	}


	/********************************************************************************************/


	VulkanGFXBuffer::VulkanGFXBuffer(VkBuffer buffer, GFXDevice* pDevice, VkDeviceMemory memory, 
		VkMemoryRequirements memoryRequirements, GFXBufferInfo info)
	{
		mDebugName				= info.debugName;
		mVkBuffer				= buffer;
		mVkMemoryRequirements	= memoryRequirements;
		mVkDeviceMemory			= memory;
		mpParentDevice			= pDevice;
		mSize					= info.size;
		mUsageFlags				= info.usageFlags;
		mMemoryFlags			= info.memoryFlags;
		mSharingMode			= info.sharingMode;
	}


	/********************************************************************************************/


	VulkanGFXImageView::VulkanGFXImageView(GFXDevice* pDevice, GFXImage* pImage, VkImageView imageView, GFXImageViewInfo info)
	{
		mVkImageView	= imageView;
		mDebugName		= info.debugName;
		mpImage			= pImage;
		mImageType		= info.imageType;
		mImageFormat	= info.imageFormat;
		mBaseMipLevel	= info.baseMipLevel;
	}


	/********************************************************************************************/


	VulkanGFXImage::VulkanGFXImage(GFXDevice* pDevice, VkImage image, VkFormat format, VkDeviceMemory deviceMemory,
		VkMemoryRequirements memoryRequirements, GFXImageInfo info)
	{
		mVkImage				= image;
		mVkFormat				= format;
		mVkDeviceMemory			= deviceMemory;
		mVkMemoryRequirements	= memoryRequirements;
		mDebugName				= info.debugName;
		mpParentDevice			= pDevice;
		mWidth					= info.width;
		mHeight					= info.height;
		mImageFormat			= info.format;
		mImageType				= info.imageType;
		mMipLevels				= info.mipLevels;
		mMultisamples			= info.multisamples;
		mSharingMode			= info.sharingMode;
	}


	/********************************************************************************************/


	VulkanGFXShader::VulkanGFXShader(GFXDevice* pDevice, VkShaderModule module, 
		VkPipelineShaderStageCreateInfo pipelineStageInfo, GFXShaderInfo info)
	{
		mVkShaderModule			= module;
		mVkPipelineStageInfo	= pipelineStageInfo;
		mpParentDevice			= pDevice;
		mDebugName				= info.debugName;
		mShaderStage			= info.shaderStage;
	}


	/********************************************************************************************/


	VulkanGFXVertexInputState::VulkanGFXVertexInputState(VkPipelineVertexInputStateCreateInfo vertexInputStateInfo, 
		VkVertexInputBindingDescription* pVkVertexInputBindingDescriptions,
		VkVertexInputAttributeDescription* pVkVertexInputAttributeDescriptions,
		GFXVertexInputStateInfo info)
	{
		mVkVertexInputStateInfo = vertexInputStateInfo;
		mpVkVertexInputBindingDescriptions = pVkVertexInputBindingDescriptions;
		mpVkVertexInputAttributeDescriptions = pVkVertexInputAttributeDescriptions;
	}


	/********************************************************************************************/


	VulkanGFXInputAssemblyState::VulkanGFXInputAssemblyState(VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateInfo, GFXInputAssemblyStateInfo info)
	{
		mVkInputAssemblyStateInfo	= inputAssemblyStateInfo;
		mTopology					= info.topology;
	}


	/********************************************************************************************/


	VulkanGFXViewportState::VulkanGFXViewportState(VkPipelineViewportStateCreateInfo viewportStateInfo, VkViewport* pViewport,
		VkRect2D* pScissor, GFXViewportStateInfo info)
	{
		mVkViewportStateInfo	= viewportStateInfo;
		mpVkViewport			= pViewport;
		mpVkScissor				= pScissor;
		mViewport				= info.viewport;
		mScissor				= info.scissor;
	}

	VulkanGFXViewportState::~VulkanGFXViewportState()
	{
		delete mpVkViewport;
		delete mpVkScissor;
	}


	/********************************************************************************************/


	VulkanGFXRasterizationState::VulkanGFXRasterizationState(VkPipelineRasterizationStateCreateInfo rasterizationStateInfo, GFXRasterizationStateInfo info)
	{
		mVkRasterizationStateInfo	= rasterizationStateInfo;
		mFillMode					= info.fillMode;
		mLineWidth					= info.lineWidth;
		mCullFace					= info.cullFace;
		mFrontFace					= info.frontFace;
	}


	/********************************************************************************************/


	VulkanGFXMultisampleState::VulkanGFXMultisampleState(VkPipelineMultisampleStateCreateInfo multisampleStateInfo, GFXMultisampleStateInfo info)
	{
		mVkMultisampleStateInfo		= multisampleStateInfo;
		mMultisamplingEnabled		= info.enableMultisampling;
		mMultisamples				= info.multisamples;
	}


	/********************************************************************************************/


	VulkanGFXColorBlendState::VulkanGFXColorBlendState(VkPipelineColorBlendStateCreateInfo colorBlendState,
		VkPipelineColorBlendAttachmentState* colorBlendAttachments, GFXColorBlendStateInfo info)
	{
		mVkColorBlendStateInfo		= colorBlendState;
		mpVkColorBlendAttachments	= colorBlendAttachments;
		mLogicalOpEnabled			= info.logicalOpEnable;
		mLogicalOp					= info.logicalOp;
		mAttachmentCount			= info.attachmentCount;
		mpAttachments				= info.pAttachments;
		mBlendConstants[0]			= info.blendConstants[0];
		mBlendConstants[1]			= info.blendConstants[1];
		mBlendConstants[2]			= info.blendConstants[2];
		mBlendConstants[3]			= info.blendConstants[3];
	}

	VulkanGFXColorBlendState::~VulkanGFXColorBlendState()
	{
		delete[] mpVkColorBlendAttachments;
	}


	/********************************************************************************************/


	VulkanGFXPipelineLayout::VulkanGFXPipelineLayout(GFXDevice* pDevice, VkPipelineLayout pipelineLayout, GFXPipelineLayoutInfo info)
	{
		mpParentDevice = pDevice;
		mVkPipelineLayout = pipelineLayout;
	}


	/********************************************************************************************/


	VulkanGFXRenderAttachment::VulkanGFXRenderAttachment(VkAttachmentDescription description, GFXRenderAttachmentInfo info)
	{
		mVkAttachmentDescription	= description;
		mImageFormat				= info.imageFormat;
		mMultisamples				= info.multisamples;
		mLoadOp						= info.loadOp;
		mStoreOp					= info.storeOp;
		mStencilLoadOp				= info.stencilLoadOp;
		mStencilStoreOp				= info.stencilStoreOp;
		mInitialLayout				= info.initialLayout;
		mFinalLayout				= info.finalLayout;
	}


	/********************************************************************************************/


	VulkanGFXRenderSubpass::VulkanGFXRenderSubpass(VkSubpassDescription subpassDescription, VkAttachmentReference* pInputAttachments,
		VkAttachmentReference* pColorAttachments, VkAttachmentReference* pResolveAttachments,
		VkAttachmentReference* pDepthStencilAttachment, GFXRenderSubpassInfo info)
	{
		mVkSubpassDescription			= subpassDescription;
		mpVkInputAttachments			= pInputAttachments;
		mpVkColorAttachments			= pColorAttachments;
		mpVkResolveAttachments			= pResolveAttachments;
		mpVkDepthStencilAttachment		= pDepthStencilAttachment;
		mBindPoint						= info.bindPoint;
		mInputAttachmentCount			= info.inputAttachmentCount;
		mpInputAttachments				= info.pInputAttachments;
		mColorAttachmentCount			= info.colorAttachmentCount;
		mpColorAttachments				= info.pColorAttachments;
		mpResolveAttachments			= info.pResolveAttachments;
		mpDepthStencilAttachment		= info.pDepthStencilAttachment;
		mPreservedAttachmentCount		= info.preservedAttachmentCount;
		mpPreservedAttachmentIndices	= info.pPreservedAttachmentIndices;
	}

	VulkanGFXRenderSubpass::~VulkanGFXRenderSubpass()
	{
		delete[]	mpVkInputAttachments;
		delete[]	mpVkColorAttachments;
		delete[]	mpVkResolveAttachments;
		delete		mpVkDepthStencilAttachment;
	}


	/********************************************************************************************/


	VulkanGFXRenderPass::VulkanGFXRenderPass(GFXDevice* pDevice, VkRenderPass renderPass, VkAttachmentDescription* pAttachments, 
		VkSubpassDependency* pDependencies, VkSubpassDescription* pSubpasses, GFXRenderPassInfo info)
	{
		mVkRenderPass		= renderPass;
		mpVkAttachments		= pAttachments;
		mpVkDependencies	= pDependencies;
		mpVkSubpasses		= pSubpasses;
		mpParentDevice		= pDevice;
		mAttachmentCount	= info.attachmentCount;
		mppAttachments		= info.ppAttachments;
		mSubpassCount		= info.subpassCount;
		mppSubpasses		= info.ppSubpasses;
		mDependencyCount	= info.dependencyCount;
		mpDependencies		= info.pDependencies;
	}

	VulkanGFXRenderPass::~VulkanGFXRenderPass()
	{
		delete[] mpVkAttachments;
		delete[] mpVkDependencies;
		delete[] mpVkSubpasses;
	}


	/********************************************************************************************/


	VulkanGFXDescriptorSetLayout::VulkanGFXDescriptorSetLayout(GFXDevice* pDevice, VkDescriptorSetLayout descriptorSetLayout, 
		VkDescriptorSetLayoutBinding* pVkDescriptorSetLayoutBindings, GFXDescriptorSetLayoutInfo info)
	{
		mpParentDevice = pDevice;
		mBindingCount = info.bindingCount;
		mpBindings = info.pBindings;
		mVkDescriptorSetLayout = descriptorSetLayout;
		mpVkDescriptorSetLayoutBindings = pVkDescriptorSetLayoutBindings;
	}


	/********************************************************************************************/


	VulkanGFXPipeline::VulkanGFXPipeline(GFXDevice* pDevice, VkPipeline pipeline, 
		VkPipelineShaderStageCreateInfo* pShaderStages, GFXPipelineInfo info)
	{
		mVkPipeline				= pipeline;
		mpVkShaderStages		= pShaderStages;
		mpParentDevice			= pDevice;
		mShaderCount			= info.shaderCount;
		mppShaders				= info.ppShaders;
		mpVertexInputState		= info.pVertexInputState;
		mpInputAssemblyState	= info.pInputAssemblyState;
		mpViewportState			= info.pViewportState;
		mpRasterizationState	= info.pRasterizationState;
		mpMultisampleState		= info.pMultisampleState;
		mpPipelineLayout		= info.pPipelineLayout;
		mpRenderPass			= info.pRenderPass;
		mSubpassIndex			= info.subpassIndex;
		mpParentPipeline		= info.pParentPipeline;
		mParentPipelineIndex	= info.parentPipelineIndex;
	}

	VulkanGFXPipeline::~VulkanGFXPipeline()
	{
		delete[] mpVkShaderStages;
	}


	/********************************************************************************************/


	VulkanGFXFramebuffer::VulkanGFXFramebuffer(GFXDevice* pDevice, VkFramebuffer framebuffer, GFXFramebufferInfo info)
	{
		mVkFramebuffer = framebuffer;
	}

	VulkanGFXFramebuffer::~VulkanGFXFramebuffer()
	{

	}


	/********************************************************************************************/


	VulkanGFXCommandPool::VulkanGFXCommandPool(GFXDevice* pDevice, VkCommandPool pool, GFXCommandPoolInfo info)
	{
		mVkCommandPool = pool;
	}


	/********************************************************************************************/


	VulkanGFXCommandPool::~VulkanGFXCommandPool()
	{
	}


	/********************************************************************************************/
}