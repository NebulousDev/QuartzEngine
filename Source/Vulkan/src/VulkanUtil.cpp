#include "VulkanUtil.h"

#include "VulkanImage.h"

#include "log/Log.h"

namespace VulkanUtil
{
	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT		messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT				messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData)
	{
		if (messageType == VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT)
		{
			Log::Debug("%s\n", pCallbackData->pMessage);
		}
		else if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
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

	String VendorNameFromID(UInt32 vendorID)
	{
		switch (vendorID)
		{
			case 0x1002: return "AMD";
			case 0x1010: return "ImgTec";
			case 0x10DE: return "NVIDIA";
			case 0x13B5: return "ARM";
			case 0x5143: return "Qualcomm";
			case 0x8086: return "INTEL";

			default: return "Unknown";
		}
	}

	ImageFormat VkFormatToImageFormat(VkFormat vkFormat)
	{
		switch (vkFormat)
		{
			case VK_FORMAT_R8_UNORM:			return IMAGE_FORMAT_R;
			case VK_FORMAT_R8G8_UNORM:			return IMAGE_FORMAT_RG;
			case VK_FORMAT_R8G8B8_UNORM:		return IMAGE_FORMAT_RGB;
			case VK_FORMAT_R8G8B8A8_UNORM:		return IMAGE_FORMAT_RGBA;

			case VK_FORMAT_B8G8R8A8_UNORM:		return IMAGE_FORMAT_BGRA;

			case VK_FORMAT_R32_SFLOAT:			return IMAGE_FORMAT_R_FLOAT;
			case VK_FORMAT_R32G32_SFLOAT:		return IMAGE_FORMAT_RG_FLOAT;
			case VK_FORMAT_R32G32B32_SFLOAT:	return IMAGE_FORMAT_RGB_FLOAT;
			case VK_FORMAT_R32G32B32A32_SFLOAT: return IMAGE_FORMAT_RGBA_FLOAT;

			case VK_FORMAT_D32_SFLOAT:			return IMAGE_FORMAT_DEPTH_32;
			case VK_FORMAT_D24_UNORM_S8_UINT:	return IMAGE_FORMAT_DEPTH_24_STENCIL_8;

			default: return IMAGE_FORMAT_UNKNOWN;
		}
	}

	VkFormat ImageFormatToVkFormat(ImageFormat format)
	{
		static VkFormat sFormatTable[] =
		{
			VK_FORMAT_UNDEFINED,

			VK_FORMAT_R8_UNORM,
			VK_FORMAT_R8G8_UNORM,
			VK_FORMAT_R8G8B8_UNORM,
			VK_FORMAT_R8G8B8A8_UNORM,

			VK_FORMAT_B8G8R8A8_UNORM,

			VK_FORMAT_R32_SFLOAT,
			VK_FORMAT_R32G32_SFLOAT,
			VK_FORMAT_R32G32B32_SFLOAT,
			VK_FORMAT_R32G32B32A32_SFLOAT,

			VK_FORMAT_D32_SFLOAT,
			VK_FORMAT_D24_UNORM_S8_UINT
		};

		return sFormatTable[(UInt32)format];
	}

	VkImageType ImageTypeToVkImageType(ImageType type)
	{
		static VkImageType sTypeTable[] =
		{
			VK_IMAGE_TYPE_1D,
			VK_IMAGE_TYPE_2D,
			VK_IMAGE_TYPE_3D
		};

		return sTypeTable[(UInt32)type];
	}

	VkImageViewType ImageViewTypeToVkImageViewType(ImageViewType type)
	{
		switch (type)
		{
			case IMAGE_VIEW_TYPE_1D:		return VK_IMAGE_VIEW_TYPE_1D;
			case IMAGE_VIEW_TYPE_2D:		return VK_IMAGE_VIEW_TYPE_2D;
			case IMAGE_VIEW_TYPE_3D:		return VK_IMAGE_VIEW_TYPE_3D;
			case IMAGE_VIEW_TYPE_CUBEMAP:	return VK_IMAGE_VIEW_TYPE_CUBE;
			case IMAGE_VIEW_TYPE_ARRAY_1D:	return VK_IMAGE_VIEW_TYPE_1D_ARRAY;
			case IMAGE_VIEW_TYPE_ARRAY_2D:	return VK_IMAGE_VIEW_TYPE_2D_ARRAY;
			default:						return VK_IMAGE_VIEW_TYPE_2D;
		}
	}

	VkImageAspectFlags ImageUsageToVkImageAspects(ImageViewUsage usage)
	{
		static VkImageAspectFlags sAspectTable[] =
		{
			0,
			VK_IMAGE_ASPECT_COLOR_BIT,
			VK_IMAGE_ASPECT_COLOR_BIT,
			VK_IMAGE_ASPECT_DEPTH_BIT,
			VK_IMAGE_ASPECT_STENCIL_BIT,
			VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT
		};

		return sAspectTable[(UInt32)usage];
	}

	VkBufferUsageFlags BufferUsagesToVkBufferUsageFlags(BufferUsages bufferUsages)
	{
		VkBufferUsageFlags vkBufferUsageFlags = 0;

		if (bufferUsages & BUFFER_USAGE_TRANSFER_SRC_BIT)
			vkBufferUsageFlags |= VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
		if (bufferUsages & BUFFER_USAGE_TRANSFER_DEST_BIT)
			vkBufferUsageFlags |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		if (bufferUsages & BUFFER_USAGE_VERTEX_BUFFER_BIT)
			vkBufferUsageFlags |= VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		if (bufferUsages & BUFFER_USAGE_INDEX_BUFFER_BIT)
			vkBufferUsageFlags |= VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
		if (bufferUsages & BUFFER_USAGE_UNIFORM_BUFFER_BIT)
			vkBufferUsageFlags |= VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
		if (bufferUsages & BUFFER_USAGE_STORAGE_BUFFER_BIT)
			vkBufferUsageFlags |= VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;

		return vkBufferUsageFlags;
	}

	VkMemoryPropertyFlags BufferAccessToVkMemoryPropertyFlags(BufferAccess bufferAccess)
	{
		VkMemoryPropertyFlags vkMemoryPropertyFlags = 0;

		if (bufferAccess & BUFFER_ACCESS_DEVICE_LOCAL_BIT)
			vkMemoryPropertyFlags |= VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
		if (bufferAccess & BUFFER_ACCESS_HOST_VISIBLE_BIT)
			vkMemoryPropertyFlags |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
		if (bufferAccess & BUFFER_ACCESS_HOST_COHERENT_BIT)
			vkMemoryPropertyFlags |= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		if (bufferAccess & BUFFER_ACCESS_HOST_CACHED_BIT)
			vkMemoryPropertyFlags |= VK_MEMORY_PROPERTY_HOST_CACHED_BIT;

		return vkMemoryPropertyFlags;
	}

	VkPrimitiveTopology PrimitiveTopologyToVkPrimitiveTopology(PrimitiveTopology topology)
	{
		static VkPrimitiveTopology topologyTable[] =
		{
			VK_PRIMITIVE_TOPOLOGY_POINT_LIST,
			VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
			VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
		};

		return topologyTable[(UInt32)topology];
	}

	VkPolygonMode PolygonModeToVkPolygonMode(PolygonMode mode)
	{
		static VkPolygonMode modeTable[] =
		{
			VK_POLYGON_MODE_FILL,
			VK_POLYGON_MODE_LINE,
			VK_POLYGON_MODE_POINT
		};

		return modeTable[(UInt32)mode];
	}

	VkCullModeFlags CullModeToVkCullMode(CullMode mode)
	{
		static VkCullModeFlags modeTable[] =
		{
			VK_CULL_MODE_NONE,
			VK_CULL_MODE_FRONT_BIT,
			VK_CULL_MODE_BACK_BIT,
			VK_CULL_MODE_FRONT_AND_BACK
		};

		return modeTable[(UInt32)mode];
	}

	VkFrontFace FaceWindToVkFrontFace(FaceWind wind)
	{
		static VkFrontFace windTable[] =
		{
			VK_FRONT_FACE_COUNTER_CLOCKWISE,
			VK_FRONT_FACE_CLOCKWISE
		};

		return windTable[(UInt32)wind];
	}

	UInt32 MultisampleToVkMultisample(Multisample multisample)
	{
		// @TODO: Add check for support

		static UInt32 multisampleTable[] =
		{
			1, 2, 4, 8
		};

		return multisampleTable[(UInt32)multisample];
	}

	VkFormat AttributeTypeToVkFormat(AttributeType type)
	{
		static VkFormat typeTable[] =
		{
			VK_FORMAT_R32_SFLOAT,
			VK_FORMAT_R32G32_SFLOAT,
			VK_FORMAT_R32G32B32_SFLOAT,
			VK_FORMAT_R32G32B32A32_SFLOAT,
			VK_FORMAT_R32_SINT,
			VK_FORMAT_R32G32_SINT,
			VK_FORMAT_R32G32B32_SINT,
			VK_FORMAT_R32G32B32A32_SINT,
			VK_FORMAT_R32_UINT,
			VK_FORMAT_R32G32_UINT,
			VK_FORMAT_R32G32B32_UINT,
			VK_FORMAT_R32G32B32A32_UINT,
			VK_FORMAT_A2B10G10R10_SINT_PACK32,
			VK_FORMAT_A2B10G10R10_UINT_PACK32
		};

		return typeTable[(UInt32)type];
	}

	UInt32 AttributeTypeToSize(AttributeType type)
	{
		switch (type)
		{
			case ATTRIBUTE_TYPE_FLOAT:				return 1 * sizeof(Float32);
			case ATTRIBUTE_TYPE_FLOAT2:				return 2 * sizeof(Float32);
			case ATTRIBUTE_TYPE_FLOAT3:				return 3 * sizeof(Float32);
			case ATTRIBUTE_TYPE_FLOAT4:				return 4 * sizeof(Float32);
			case ATTRIBUTE_TYPE_INT:				return 1 * sizeof(Int32);
			case ATTRIBUTE_TYPE_INT2:				return 2 * sizeof(Int32);
			case ATTRIBUTE_TYPE_INT3:				return 3 * sizeof(Int32);
			case ATTRIBUTE_TYPE_INT4:				return 4 * sizeof(Int32);
			case ATTRIBUTE_TYPE_UINT:				return 1 * sizeof(UInt32);
			case ATTRIBUTE_TYPE_UINT2:				return 2 * sizeof(UInt32);
			case ATTRIBUTE_TYPE_UINT3:				return 3 * sizeof(UInt32);
			case ATTRIBUTE_TYPE_UINT4:				return 4 * sizeof(UInt32);
			case ATTRIBUTE_TYPE_INT_2_10_10_10:		return 1 * sizeof(Int32);
			case ATTRIBUTE_TYPE_UINT_2_10_10_10:	return 1 * sizeof(UInt32);
			default: return 0;
		}
	}

	VkImageUsageFlags ImageUsagesToVkImageUsageFlags(ImageUsages usages)
	{
		VkImageUsageFlags flags = 0;

		if ((usages & IMAGE_USAGE_SAMPLED_TEXTURE_BIT) == IMAGE_USAGE_SAMPLED_TEXTURE_BIT)
		{
			flags |= VK_IMAGE_USAGE_SAMPLED_BIT;
		}

		if ((usages & IMAGE_USAGE_COLOR_ATTACHMENT_BIT) == IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
		{
			flags |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		}

		if ((usages & IMAGE_USAGE_DEPTH_ATTACHMENT_BIT) == IMAGE_USAGE_DEPTH_ATTACHMENT_BIT ||
			(usages & IMAGE_USAGE_STENCIL_ATTACHMENT_BIT) == IMAGE_USAGE_STENCIL_ATTACHMENT_BIT)
		{
			flags |= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
		}

		if ((usages & IMAGE_USAGE_INPUT_ATTACHMENT_BIT) == IMAGE_USAGE_INPUT_ATTACHMENT_BIT)
		{
			flags |= VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT;
		}

		if ((usages & IMAGE_USAGE_TRANSFER_SRC_BIT) == IMAGE_USAGE_TRANSFER_SRC_BIT)
		{
			flags |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		}

		if ((usages & IMAGE_USAGE_TRANSFER_DST_BIT) == IMAGE_USAGE_TRANSFER_DST_BIT)
		{
			flags |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		}

		return flags;
	}

	VkResult EnumerateVkLayerProperties(Array<VkLayerProperties>& layerProperties)
	{
		VkResult result;
		UInt32 layerCount = 0;

		result = vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		layerProperties.Resize(layerCount);
		result = vkEnumerateInstanceLayerProperties(&layerCount, layerProperties.Data());

		if (result != VK_SUCCESS)
		{
			Log::Critical("Failed to emumerate vulkan instance layer properties!");
		}

		return result;
	}

	VkResult EnumerateVkExtensionProperties(Array<VkExtensionProperties>& extensionProperties)
	{
		VkResult result;
		UInt32 extensionCount = 0;

		result = vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);
		extensionProperties.Resize(extensionCount);
		result = vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, extensionProperties.Data());

		if (result != VK_SUCCESS)
		{
			Log::Critical("Failed to emumerate vulkan extention properties!");
		}

		return result;
	}

	VkResult EnumerateVkPhysicalDevices(VkInstance instance, Array<VkPhysicalDevice>& physicalDevices)
	{
		VkResult result;
		UInt32 physicalDeviceCount;

		result = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
		physicalDevices.Resize(physicalDeviceCount);
		result = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.Data());

		if (result != VK_SUCCESS)
		{
			Log::Critical("Failed to emumerate vulkan physical devices!");
		}

		return result;
	}

	UInt32 FindCompatableMemoryType(VulkanDevice* pDevice, Flags32 memoryTypeBits, VkMemoryPropertyFlags memoryProperties)
	{
		VkPhysicalDeviceMemoryProperties deviceMemoryProperties = pDevice->GetPhysicalDevice().GetPhysicalDeviceMemoryProperties();

		for (UInt32 i = 0; i < deviceMemoryProperties.memoryTypeCount; i++)
		{
			if ((memoryTypeBits & (1 << i)) && (deviceMemoryProperties.memoryTypes[i].propertyFlags & memoryProperties) == memoryProperties)
			{
				return i;
			}
		}

		return (UInt32)-1;
	}

	VkResult CreateVkInstance(VkInstance* pvkInstance, const char* appName, Array<const char*> validationLayers, Array<const char*> extentions)
	{
		VkApplicationInfo appInfo = {};
		appInfo.sType				= VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName	= appName;
		appInfo.applicationVersion	= VK_MAKE_VERSION(0, 0, 0);
		appInfo.pEngineName			= "Quartz Engine";
		appInfo.engineVersion		= VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion			= VK_API_VERSION_1_2;

		VkInstanceCreateInfo vkInstanceInfo = {};
		vkInstanceInfo.sType					= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		vkInstanceInfo.pApplicationInfo			= &appInfo;
		vkInstanceInfo.enabledLayerCount		= validationLayers.Size();
		vkInstanceInfo.ppEnabledLayerNames		= validationLayers.Data();
		vkInstanceInfo.enabledExtensionCount	= extentions.Size();
		vkInstanceInfo.ppEnabledExtensionNames	= extentions.Data();

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
		debugMessengerInfo.pNext = NULL;

		vkInstanceInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugMessengerInfo;

		VkResult result = vkCreateInstance(&vkInstanceInfo, NULL, pvkInstance);

		if (result != VK_SUCCESS)
		{
			Log::Critical("Failed to create a vulkan instance: vkCreateInstance failed!");
		}

		VkDebugUtilsMessengerEXT mDebugMessenger;
		PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>
			(vkGetInstanceProcAddr(*pvkInstance, "vkCreateDebugUtilsMessengerEXT"));

		if (!vkCreateDebugUtilsMessengerEXT)
		{
			Log::Warning("Failed to address for \"PFN_vkCreateDebugUtilsMessengerEXT\"! No validation messages will be displayed!");
		}
		else if (vkCreateDebugUtilsMessengerEXT(*pvkInstance, &debugMessengerInfo, NULL, &mDebugMessenger) != VK_SUCCESS)
		{
			Log::Warning("Failed to create validation debug messenger! No validation messages will be displayed!");
		}

		return result;
	}
}

