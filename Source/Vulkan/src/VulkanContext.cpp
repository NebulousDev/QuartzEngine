#include "VulkanContext.h"

#include "log/log.h"

#include "VulkanSurface.h"
#include "VulkanDevice.h"
#include "VulkanShader.h"
#include "VulkanPipeline.h"
#include "VulkanBuffer.h"
#include "VulkanFrameBuffer.h"
#include "VulkanCommandBuffer.h"

namespace Quartz
{
	VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT		messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT				messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void*										pUserData)
	{
		/*
		if (messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
		{
			Log.Error(pCallbackData->pMessage);
		}
		else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT)
		{
			Log.Warning(pCallbackData->pMessage);
		}
		else
		{
			Log.Debug(pCallbackData->pMessage);
		}
		*/

		Log.Error(pCallbackData->pMessage);

		return VK_FALSE;
	}

	void VulkanContext::SetDebugNameImpl(GFXResource* pResource, const String& debugName)
	{
		VulkanDevice& vulkanDevice = GetDefaultDevice().CastAs<VulkanDevice&>();
		//TODO: Make this work
		//vulkanDevice.SetDebugMarkerObjectName(0, VK_DEBUG_REPORT_OBJECT_TYPE_UNKNOWN_EXT, debugName); 
	}

	Bool8 EnumerateVkLayerProperties(Array<VkLayerProperties>& layerProperties)
	{
		VkResult result;
		UInt32 layerCount = 0;

		result = vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		layerProperties.Resize(layerCount);
		result = vkEnumerateInstanceLayerProperties(&layerCount, layerProperties.Data());

		return result == VK_SUCCESS;
	}

	Bool8 EnumerateVkExtensionProperties(Array<VkExtensionProperties>& extensionProperties)
	{
		VkResult result;
		UInt32 extensionCount = 0;

		result = vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);
		extensionProperties.Resize(extensionCount);
		result = vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, extensionProperties.Data());
	
		return result == VK_SUCCESS;
	}

	Bool8 EnumerateVkPhysicalDevices(VkInstance instance, Array<VkPhysicalDevice>& physicalDevices)
	{
		VkResult result;
		UInt32 physicalDeviceCount;

		result = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
		physicalDevices.Resize(physicalDeviceCount);
		result = vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, physicalDevices.Data());

		return result == VK_SUCCESS;
	}

	Bool8 VulkanContext::InitInstance(const String& appName, const String& engineName,
		const Array<String>& extensions, const Array<String>& validationLayers)
	{
		if (!EnumerateVkLayerProperties(mAvailableLayers))
		{
			Log.Warning("Failed to create a vulkan instance: Unable to enumerate validation layers!");
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

			Log.Warning("Attempted to enable unsupported vulkan instance layer [\'%s\']!", layerName.Str());

			layerFound:;
		}

		if (!EnumerateVkExtensionProperties(mAvailableExtensions))
		{
			Log.Warning("Failed to create a vulkan instance: Unable to enumerate extensions!");
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

			Log.Warning("Attempted to enable unsupported vulkan instance extension [\'%s\']!", extName.Str());

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

		VkApplicationInfo appInfo	= {};
		appInfo.sType				= VK_STRUCTURE_TYPE_APPLICATION_INFO;
		appInfo.pApplicationName	= appName.Str();
		appInfo.applicationVersion	= VK_MAKE_VERSION(1, 0, 0);
		appInfo.pEngineName			= engineName.Str();
		appInfo.engineVersion		= VK_MAKE_VERSION(1, 0, 0);
		appInfo.apiVersion			= VK_API_VERSION_1_1; // NOTE: 1_2 is not yet needed

		VkInstanceCreateInfo createInfo		= {};
		createInfo.sType					= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		createInfo.pApplicationInfo			= &appInfo;
		createInfo.enabledLayerCount		= mEnabledLayerNames.Size();
		createInfo.ppEnabledLayerNames		= mEnabledLayerNames.Data();
		createInfo.enabledExtensionCount	= mEnabledExtensionNames.Size();
		createInfo.ppEnabledExtensionNames	= mEnabledExtensionNames.Data();
		createInfo.pNext					= (VkDebugUtilsMessengerCreateInfoEXT*)&debugMessengerInfo;

		if (vkCreateInstance(&createInfo, NULL, &mVkInstance) != VK_SUCCESS)
		{
			Log.Critical("Failed to create a vulkan instance: vkCreateInstance failed!");
			return false;
		}

		VkDebugUtilsMessengerEXT mDebugMessenger;
		PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>
			(vkGetInstanceProcAddr(mVkInstance, "vkCreateDebugUtilsMessengerEXT"));

		if (!vkCreateDebugUtilsMessengerEXT)
		{
			Log.Warning("Failed to address for \"PFN_vkCreateDebugUtilsMessengerEXT\"! No validation messages will be displayed!");
		}
		else if (vkCreateDebugUtilsMessengerEXT(mVkInstance, &debugMessengerInfo, NULL, &mDebugMessenger) != VK_SUCCESS)
		{
			Log.Warning("Failed to create validation debug messenger! No validation messages will be displayed!");
		}

		return true;
	}

	Bool8 VulkanContext::InitDevices()
	{
		if (!EnumerateVkPhysicalDevices(mVkInstance, mAvailablePhysicalDevices))
		{
			Log.Error("Failed to initialize graphics device: vkEnumeratePhysicalDevices failed!");
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
			Log.Critical("Failed to initialize graphics device: No suitable graphics adapter was detected!");
			return false;
		}

		Array<String> deviceExtensions;
		deviceExtensions.PushBack(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
		deviceExtensions.PushBack("VK_EXT_full_screen_exclusive");

		mpDevice = new VulkanDevice(pPhyiscalDeviceCandidate, deviceExtensions);

		if (!mpDevice->IsValidDevice())
		{
			Log.Critical("Failed to initialize graphics device: No valid device was created!");
			return false;
		}

		return true;
	}

	VulkanContext::VulkanContext(const String& appName, const String& engineName,
		const Array<String>& extensions, const Array<String>& validationLayers)
	{
		mAppName = appName;
		mEngineName = engineName;

		if (
			InitInstance(appName, engineName, extensions, validationLayers) &&
			InitDevices()
			)
		{
			mValidContext = true;
		}
	}
	
	GFXSurface* VulkanContext::CreateSurface(Window& window, UInt32 width, UInt32 height, Bool8 vSync, Bool8 fullscreen)
	{
		return new VulkanSurface(mVkInstance, window, *mpDevice, width, height, vSync, fullscreen);
	}

	GFXGraphicsPipeline* VulkanContext::CreateGraphicsPipeline(
		GFXGraphicsPipelineShaderState& shaderState,
		GFXRenderPass& renderPass,
		GFXSurface& surface)
	{
		const UInt32 tempWidth = 640;
		const UInt32 tempHeight = 480;

		VulkanDevice& vulkanDevice = GetDefaultDevice().CastAs<VulkanDevice&>();

		struct Vertex
		{
			float pos[3];
		};

		VulkanGraphicsPipelineState graphicsPipelineState = {};

		VulkanVertexBinding vertexBinding;
		vertexBinding.binding = 0;
		vertexBinding.stride = sizeof(Vertex);
		vertexBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

		VulkanVertexAttribute vertexAttrib;
		vertexAttrib.binding = 0;
		vertexAttrib.location = 0;
		vertexAttrib.format = VK_FORMAT_R32G32B32_SFLOAT;
		vertexAttrib.offset = offsetof(Vertex, pos);

		VulkanViewport viewport;
		viewport.x = 0;
		viewport.y = 0;
		viewport.width = tempWidth;
		viewport.height = tempHeight;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VulkanScissor scissor;
		scissor.x = 0;
		scissor.y = 0;
		scissor.width = tempWidth;
		scissor.height = tempHeight;

		VulkanBlendAttachment colorBlendAttachment;
		colorBlendAttachment.enableBlend = true;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

		///////////////////////////////////////////////////////////////////////////////////

		VulkanDescriptorSetLayoutBinding binding;
		binding.binding = 0;
		binding.descriptorCount = 1;
		binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		binding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;

		Array<VulkanDescriptorSetLayoutBinding> descriptorBindings;
		descriptorBindings.PushBack(binding);
		VulkanDescriptorSetLayout descriptorLayout(vulkanDevice, descriptorBindings);

		///////////////////////////////////////////////////////////////////////////////////

		Array<VulkanDescriptorSetLayout> descriptorSets;
		descriptorSets.PushBack(descriptorLayout);
		VulkanPipelineLayout pipelineLayout(vulkanDevice, descriptorSets);

		///////////////////////////////////////////////////////////////////////////////////

		/*
		VulkanAttachment colorAttachment;
		colorAttachment.flags = 0;
		colorAttachment.format = surface.CastAs<const VulkanSurface&>().GetFormat();
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VulkanAttachmentReference colorReference;
		colorReference.attachment = 0;
		colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VulkanSubpass subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachments.PushBack(colorReference);

		VulkanSubpassDependency subpassDependency = {};
		subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		subpassDependency.dependencyFlags = 0;
		subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		subpassDependency.srcAccessMask = 0;
		subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		Array<VulkanAttachment> attachments;
		attachments.PushBack(colorAttachment);
		Array<VulkanSubpass> subpasses;
		subpasses.PushBack(subpass);
		Array<VulkanSubpassDependency> subpassDependencies;
		subpassDependencies.PushBack(subpassDependency);
		VulkanRenderPass renderPass(vulkanDevice, attachments, subpasses, subpassDependencies);
		*/

		///////////////////////////////////////////////////////////////////////////////////

		graphicsPipelineState.flags = 0;
		graphicsPipelineState.pVertexShader = &shaderState.pVertexShader->CastAs<VulkanVertexShader&>();
		graphicsPipelineState.pPixelShader = &shaderState.pPixelShader->CastAs<VulkanPixelShader&>();

		graphicsPipelineState.vertexBindings.PushBack(vertexBinding);
		graphicsPipelineState.vertexAttributes.PushBack(vertexAttrib);

		graphicsPipelineState.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

		graphicsPipelineState.viewports.PushBack(viewport);
		graphicsPipelineState.scissors.PushBack(scissor);

		graphicsPipelineState.polygonMode = VK_POLYGON_MODE_FILL;
		graphicsPipelineState.cullMode = VK_CULL_MODE_BACK_BIT;
		graphicsPipelineState.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
		graphicsPipelineState.lineWidth = 1.0f;

		graphicsPipelineState.multisamples = 1;

		graphicsPipelineState.enableDepthTest = true;
		graphicsPipelineState.enableDepthWrite = true;
		graphicsPipelineState.enableDepthBoundsTest = false;
		graphicsPipelineState.enableStencilTest = false;
		graphicsPipelineState.depthCompareOp = VK_COMPARE_OP_LESS;
		graphicsPipelineState.minDepth = 0.0f;
		graphicsPipelineState.maxDepth = 1.0f;

		graphicsPipelineState.enableColorBlendLogicOp;
		graphicsPipelineState.colorBlendLogicOp = VK_LOGIC_OP_COPY;

		graphicsPipelineState.colorBlendAttachments.PushBack(colorBlendAttachment);

		graphicsPipelineState.pipelineLayout = pipelineLayout;

		graphicsPipelineState.renderPass = renderPass.CastAs<VulkanRenderPass&>();
		graphicsPipelineState.subpassIndex = 0;

		return new VulkanGraphicsPipeline(vulkanDevice, graphicsPipelineState, nullptr);
	}

	GFXVertexShader* VulkanContext::CreateVertexShader(const Array<Byte>& shaderCode)
	{
		VulkanDevice& vulkanDevice = GetDefaultDevice().CastAs<VulkanDevice&>();
		return new VulkanVertexShader(vulkanDevice, shaderCode, "main");
	}

	GFXPixelShader* VulkanContext::CreatePixelShader(const Array<Byte>& shaderCode)
	{
		VulkanDevice& vulkanDevice = GetDefaultDevice().CastAs<VulkanDevice&>();
		return new VulkanPixelShader(vulkanDevice, shaderCode, "main");
	}

	GFXVertexBuffer* VulkanContext::CreateVertexBuffer(UInt64 sizeBytes, Bool8 hostVisable)
	{
		VulkanDevice& vulkanDevice = GetDefaultDevice().CastAs<VulkanDevice&>();

		VkMemoryPropertyFlags memoryProperties = 0;
		if (hostVisable)
		{
			memoryProperties |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
			memoryProperties |= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		}

		return new VulkanVertexBuffer(vulkanDevice, sizeBytes, memoryProperties, hostVisable);
	}

	GFXIndexBuffer* VulkanContext::CreateIndexBuffer(UInt32 stride, UInt64 size, Bool8 hostVisable)
	{
		VulkanDevice& vulkanDevice = GetDefaultDevice().CastAs<VulkanDevice&>();

		VkMemoryPropertyFlags memoryProperties = 0;
		if (hostVisable)
		{
			memoryProperties |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
			memoryProperties |= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		}

		return new VulkanIndexBuffer(vulkanDevice, stride, size, memoryProperties, hostVisable);
	}

	GFXUniformBuffer* VulkanContext::CreateUniformBuffer(UInt32 sizeBytes, Bool8 hostVisable)
	{
		VulkanDevice& vulkanDevice = GetDefaultDevice().CastAs<VulkanDevice&>();

		VkMemoryPropertyFlags memoryProperties = 0;
		if (hostVisable)
		{
			memoryProperties |= VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT;
			memoryProperties |= VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		}

		return new VulkanUniformBuffer(vulkanDevice, sizeBytes, memoryProperties, hostVisable);
	}

	GFXFramebuffer* VulkanContext::CreateFramebuffer(GFXGraphicsPipeline* pGrapicsPipeline, UInt32 width, UInt32 height, const Array<GFXImageView*>& images)
	{
		VulkanDevice& vulkanDevice = GetDefaultDevice().CastAs<VulkanDevice&>();

		Array<VkImageView> attachments;
		for (GFXImageView* pImageView : images)
		{
			VulkanImageView& vulkanImageView = pImageView->CastAs<VulkanImageView&>();
			attachments.PushBack(vulkanImageView.GetImageViewHandle());
		}

		if (pGrapicsPipeline == nullptr)
		{
			//TODO: Error
			return nullptr;
		}

		VulkanGraphicsPipeline& vulkanPipeline = pGrapicsPipeline->CastAs<VulkanGraphicsPipeline&>();
		const VulkanRenderPass& renderPass = vulkanPipeline.GetPipelineState().renderPass;

		return new VulkanFrameBuffer(vulkanDevice, renderPass.GetRenderPassHandle(), width, height, attachments);
	}

	GFXFramebuffer* VulkanContext::CreateFramebuffer(GFXGraphicsPipeline* pGrapicsPipeline, GFXSurface* pSurface, UInt32 bufferIndex)
	{
		VulkanSurface& vulkanSurface = pSurface->CastAs<VulkanSurface&>();

		Array<GFXImageView*> imageViews;
		imageViews.PushBack(&vulkanSurface.GetImageViews()[bufferIndex]);

		return CreateFramebuffer(pGrapicsPipeline, vulkanSurface.GetWidth(), vulkanSurface.GetHeight(), imageViews);
	}

	GFXCommandBuffer* VulkanContext::CreateGraphicsCommandBuffer()
	{
		VulkanDevice& vulkanDevice = GetDefaultDevice().CastAs<VulkanDevice&>();
		return new VulkanCommandBuffer(vulkanDevice, vulkanDevice.GetGraphicsCommandPoolHandle());
	}

	GFXCommandBuffer* VulkanContext::CreateComputeCommandBuffer()
	{
		VulkanDevice& vulkanDevice = GetDefaultDevice().CastAs<VulkanDevice&>();
		return new VulkanCommandBuffer(vulkanDevice, vulkanDevice.GetComputeCommandPoolHandle());
	}

	GFXCommandBuffer* VulkanContext::CreateTransferCommandBuffer()
	{
		VulkanDevice& vulkanDevice = GetDefaultDevice().CastAs<VulkanDevice&>();
		return new VulkanCommandBuffer(vulkanDevice, vulkanDevice.GetTransferCommandPoolHandle());
	}

	VkAttachmentLoadOp LoadOpToAttachmentLoadOp(GFXLoadOp loadOp)
	{
		static VkAttachmentLoadOp sLoadOpTable[] = 
		{ 
			VK_ATTACHMENT_LOAD_OP_DONT_CARE, 
			VK_ATTACHMENT_LOAD_OP_LOAD, 
			VK_ATTACHMENT_LOAD_OP_CLEAR
		};

		return sLoadOpTable[(UInt32)loadOp];
	}

	VkAttachmentStoreOp StoreOpToAttachmentStoreOp(GFXStoreOp storeOp)
	{
		static VkAttachmentStoreOp sStoreOpTable[] =
		{
			VK_ATTACHMENT_STORE_OP_DONT_CARE,
			VK_ATTACHMENT_STORE_OP_STORE
		};

		return sStoreOpTable[(UInt32)storeOp];
	}

	VkFormat FormatToVkFormat(GFXFormat format)
	{
		static VkFormat sFormatTable[] =
		{
			VK_FORMAT_R8_UNORM,
			VK_FORMAT_R8G8_UNORM,
			VK_FORMAT_R8G8B8_UNORM,
			VK_FORMAT_R8G8B8A8_UNORM,
			VK_FORMAT_B8G8R8A8_UNORM,
			VK_FORMAT_D24_UNORM_S8_UINT
		};

		return sFormatTable[(UInt32)format];
	}

	GFXRenderPass* VulkanContext::CreateRenderPass(GFXRenderPassInfo& renderPassInfo)
	{
		VulkanRenderPassLayout renderPassLayout = {};

		for (GFXColorAttachment& color : renderPassInfo.colorTargets)
		{
			VulkanAttachment attachment = {};
			attachment.flags = 0;
			attachment.format = FormatToVkFormat(color.format);
			attachment.samples = VK_SAMPLE_COUNT_1_BIT;
			attachment.loadOp = LoadOpToAttachmentLoadOp(color.loadOp);
			attachment.storeOp = StoreOpToAttachmentStoreOp(color.storeOp);
			attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;//VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
			attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;//VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			renderPassLayout.attachments.PushBack(attachment);
		}

		for (GFXDepthStencilAttachment& depthStencil : renderPassInfo.depthStencilTargets)
		{
			VulkanAttachment attachment = {};
			attachment.flags = 0;
			attachment.format = FormatToVkFormat(depthStencil.format);
			attachment.samples = VK_SAMPLE_COUNT_1_BIT;
			attachment.loadOp = LoadOpToAttachmentLoadOp(depthStencil.depthLoadOp);
			attachment.storeOp = StoreOpToAttachmentStoreOp(depthStencil.depthStoreOp);
			attachment.stencilLoadOp = LoadOpToAttachmentLoadOp(depthStencil.stencilLoadOp);
			attachment.stencilStoreOp = StoreOpToAttachmentStoreOp(depthStencil.stencilStoreOp);
			attachment.initialLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

			renderPassLayout.attachments.PushBack(attachment);
		}

		VulkanSubpass subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

		for (UInt32 i = 0; i < renderPassInfo.colorTargets.Size(); ++i)
		{
			VulkanAttachmentReference colorReference;
			colorReference.attachment = i;
			colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			subpass.colorAttachments.PushBack(colorReference);
		}

		for (UInt32 i = 0; i < renderPassInfo.depthStencilTargets.Size(); ++i)
		{
			VulkanAttachmentReference colorReference;
			colorReference.attachment = i;
			colorReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

			subpass.depthStencilAttachments.PushBack(colorReference);
		}

		renderPassLayout.subpasses.PushBack(subpass);

		VulkanDevice& vulkanDevice = GetDefaultDevice().CastAs<VulkanDevice&>();
		return new VulkanRenderPass(vulkanDevice, renderPassLayout);
	}

	void VulkanContext::BeginRenderPass(GFXCommandBuffer& commandBuffer, GFXRenderPass& renderPass, GFXFramebuffer& frameBuffer)
	{
		VulkanCommandBuffer& vulkanCommandBuffer = commandBuffer.CastAs<VulkanCommandBuffer&>();
		VulkanRenderPass& vulkanRenderPass = renderPass.CastAs<VulkanRenderPass&>();
		VulkanFrameBuffer& vulkanFrameBuffer = frameBuffer.CastAs<VulkanFrameBuffer&>();

		VkClearValue clearColor = { 1.0f, 0.0f, 0.0f, 1.0f };

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = vulkanRenderPass.GetRenderPassHandle();
		renderPassInfo.framebuffer = vulkanFrameBuffer.GetFrameBufferHandle();
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = { vulkanFrameBuffer.GetWidth(), vulkanFrameBuffer.GetHeight() };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(vulkanCommandBuffer.GetCommandBufferHandle(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	}

	void VulkanContext::EndRenderPass(GFXCommandBuffer& commandBuffer)
	{
		VulkanCommandBuffer& vulkanCommandBuffer = commandBuffer.CastAs<VulkanCommandBuffer&>();
		vkCmdEndRenderPass(vulkanCommandBuffer.GetCommandBufferHandle());
	}

	void VulkanContext::Submit(GFXCommandBuffer& commandBuffer, GFXSurface& surface)
	{
		VulkanDevice& vulkanDevice = GetDefaultDevice().CastAs<VulkanDevice&>();
		VulkanCommandBuffer& vulkanCommandBuffer = commandBuffer.CastAs<VulkanCommandBuffer&>();
		VulkanSurface& vulkanSurface = surface.CastAs<VulkanSurface&>();

		VkCommandBuffer commandBuffers[] = { vulkanCommandBuffer.GetCommandBufferHandle() };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		VkSemaphore waitSemaphores[] = { vulkanSurface.GetImageAqcuiredSemaphore(vulkanSurface.GetImageIndex()) };
		VkSemaphore signalSemaphores[] = { vulkanSurface.GetImageCompleteSemaphore(vulkanSurface.GetImageIndex()) };
		VkFence fence = vulkanSurface.GetImageFence(vulkanSurface.GetImageIndex());

		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = commandBuffers;

		VulkanQueue& graphicsQueue = vulkanDevice.GetGrahpicsQueue();

		//vkWaitForFences(vulkanDevice.GetDeviceHandle(), 1, &fence, VK_TRUE, UINT64_MAX);
		vkResetFences(vulkanDevice.GetDeviceHandle(), 1, &fence);

		if (vkQueueSubmit(graphicsQueue.GetQueueHandle(), 1, &submitInfo, fence) != VK_SUCCESS)
		{
			Log.Critical("Failed to submit queue: vkQueueSubmit failed!");
		}
	}

	void VulkanContext::Present(GFXSurface* pSurface)
	{
		VulkanDevice& vulkanDevice = GetDefaultDevice().CastAs<VulkanDevice&>();
		VulkanSurface& vulkanSurface = pSurface->CastAs<VulkanSurface&>();

		VkSemaphore signalSemaphores[] = { vulkanSurface.GetImageCompleteSemaphore(vulkanSurface.GetImageIndex()) };
		VkSwapchainKHR swapChains[] = { vulkanSurface.GetSwapChainHandle() };
		UInt32 imageIndices[] = { vulkanSurface.GetImageIndex() };

		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = imageIndices;
		presentInfo.pResults = nullptr;

		VulkanQueue& presentQueue = vulkanDevice.GetPresentQueue();

		vkQueuePresentKHR(presentQueue.GetQueueHandle(), &presentInfo);
	}

	void VulkanContext::DrawIndexed(GFXCommandBuffer& commandBuffer, UInt32 indexCount, UInt32 indexOffset)
	{
		VulkanCommandBuffer& vulkanCommandBuffer = commandBuffer.CastAs<VulkanCommandBuffer&>();
		vkCmdDrawIndexed(vulkanCommandBuffer.GetCommandBufferHandle(), indexCount, 1, indexOffset, 0, 0);
	}

	void VulkanContext::WaitSurfaceReady(GFXSurface& surface)
	{
		VulkanDevice& vulkanDevice = GetDefaultDevice().CastAs<VulkanDevice&>();
		VulkanSurface& vulkanSurface = surface.CastAs<VulkanSurface&>();

		// Not supported
	}

	void VulkanContext::WaitForPresent()
	{
		VulkanDevice& vulkanDevice = GetDefaultDevice().CastAs<VulkanDevice&>();
		VulkanQueue& presentQueue = vulkanDevice.GetPresentQueue();
		vkQueueWaitIdle(presentQueue.GetQueueHandle());
	}

	void VulkanContext::BindGraphicsPipeline(GFXCommandBuffer& commandBuffer, GFXGraphicsPipeline& pipeline)
	{
		VulkanCommandBuffer& vulkanCommandBuffer = commandBuffer.CastAs<VulkanCommandBuffer&>();
		VulkanGraphicsPipeline& vulkanPipeline = pipeline.CastAs<VulkanGraphicsPipeline&>();

		vkCmdBindPipeline(vulkanCommandBuffer.GetCommandBufferHandle(), VK_PIPELINE_BIND_POINT_GRAPHICS, vulkanPipeline.GetPipelineHandle());
	}

	void VulkanContext::UnbindGraphicsPipeline(GFXCommandBuffer& commandBuffer)
	{
		// Not supported
	}

	void* VulkanContext::MapVertexBuffer(GFXVertexBuffer* pVertexBuffer)
	{
		if (pVertexBuffer != nullptr && pVertexBuffer->IsHostVisible())
		{
			VulkanVertexBuffer& buffer = pVertexBuffer->CastAs<VulkanVertexBuffer&>();
			void* pMapData = buffer.GetDeviceMemoryAllocation()->MapMemory(0, buffer.GetSizeBytes());
			return pMapData;
		}

		return nullptr;
	}

	void* VulkanContext::MapIndexBuffer(GFXIndexBuffer* pIndexBuffer)
	{
		if (pIndexBuffer != nullptr && pIndexBuffer->IsHostVisible())
		{
			VulkanIndexBuffer& buffer = pIndexBuffer->CastAs<VulkanIndexBuffer&>();
			void* pMapData = buffer.GetDeviceMemoryAllocation()->MapMemory(0, buffer.GetSizeBytes());
			return pMapData;
		}

		return nullptr;
	}

	void* VulkanContext::MapUniformBuffer(GFXUniformBuffer* pUniformBuffer)
	{
		if (pUniformBuffer != nullptr && pUniformBuffer->IsHostVisible())
		{
			VulkanUniformBuffer& buffer = pUniformBuffer->CastAs<VulkanUniformBuffer&>();
			void* pMapData = buffer.GetDeviceMemoryAllocation()->MapMemory(0, buffer.GetSizeBytes());
			return pMapData;
		}

		return nullptr;
	}

	void VulkanContext::UnmapVertexBuffer(GFXVertexBuffer* pVertexBuffer)
	{
		if (pVertexBuffer != nullptr && pVertexBuffer->IsHostVisible())
		{
			VulkanVertexBuffer& buffer = pVertexBuffer->CastAs<VulkanVertexBuffer&>();
			buffer.GetDeviceMemoryAllocation()->UnmapMemory();
		}
	}

	void VulkanContext::UnmapIndexBuffer(GFXIndexBuffer* pIndexBuffer)
	{
		if (pIndexBuffer != nullptr && pIndexBuffer->IsHostVisible())
		{
			VulkanIndexBuffer& buffer = pIndexBuffer->CastAs<VulkanIndexBuffer&>();
			buffer.GetDeviceMemoryAllocation()->UnmapMemory();
		}
	}

	void VulkanContext::UnmapUniformBuffer(GFXUniformBuffer* pUniformBuffer)
	{
		if (pUniformBuffer != nullptr && pUniformBuffer->IsHostVisible())
		{
			VulkanUniformBuffer& buffer = pUniformBuffer->CastAs<VulkanUniformBuffer&>();
			buffer.GetDeviceMemoryAllocation()->UnmapMemory();
		}
	}

	void VulkanContext::BindVertexBuffer(GFXCommandBuffer& commandBuffer, GFXVertexBuffer* pVertexBuffer)
	{
		VulkanDevice& vulkanDevice = GetDefaultDevice().CastAs<VulkanDevice&>();
		VulkanCommandBuffer& vulkanCommandBuffer = commandBuffer.CastAs<VulkanCommandBuffer&>();
		VulkanVertexBuffer& vulkanVertexBuffer = pVertexBuffer->CastAs<VulkanVertexBuffer&>();
			
		VkBuffer vertexBuffers[] = { vulkanVertexBuffer.GetBufferHandle() };
		VkDeviceSize offsets[] = { 0 };

		vkCmdBindVertexBuffers(vulkanCommandBuffer.GetCommandBufferHandle(), 0, 1, vertexBuffers, offsets);
	}

	void VulkanContext::BindIndexBuffer(GFXCommandBuffer& commandBuffer, GFXIndexBuffer* pIndexBuffer)
	{
		VulkanDevice& vulkanDevice = GetDefaultDevice().CastAs<VulkanDevice&>();
		VulkanCommandBuffer& vulkanCommandBuffer = commandBuffer.CastAs<VulkanCommandBuffer&>();
		VulkanIndexBuffer& vulkanIndexBuffer = pIndexBuffer->CastAs<VulkanIndexBuffer&>();

		vkCmdBindIndexBuffer(vulkanCommandBuffer.GetCommandBufferHandle(), vulkanIndexBuffer.GetBufferHandle(), 0, VK_INDEX_TYPE_UINT16);
	}

	void VulkanContext::BindUniformBuffer(GFXGraphicsPipeline& pipeline, GFXCommandBuffer& commandBuffer, GFXUniformBuffer* pUniformBuffer)
	{
		VulkanDevice& vulkanDevice = GetDefaultDevice().CastAs<VulkanDevice&>();
		VulkanCommandBuffer& vulkanCommandBuffer = commandBuffer.CastAs<VulkanCommandBuffer&>();
		VulkanUniformBuffer& vulkanUnifromBuffer = pUniformBuffer->CastAs<VulkanUniformBuffer&>();
		VulkanGraphicsPipeline& vulkanPipeline = pipeline.CastAs<VulkanGraphicsPipeline&>();

		VkDescriptorSet descriptorSets[] = { vulkanUnifromBuffer.GetDescriptorSet() };
		
		vkCmdBindDescriptorSets(vulkanCommandBuffer.GetCommandBufferHandle(), VK_PIPELINE_BIND_POINT_GRAPHICS, 
			vulkanPipeline.GetPipelineState().pipelineLayout.GetPipelineLayoutHandle(), 0, 1, descriptorSets, 0, nullptr);
	}

	void VulkanContext::UnbindVertexBuffer(GFXCommandBuffer& commandBuffer, GFXVertexBuffer* vertexBuffer)
	{

	}

	void VulkanContext::UnbindIndexBuffer(GFXCommandBuffer& commandBuffer, GFXIndexBuffer* pIndexBuffer)
	{

	}

	void VulkanContext::UnbindUniformBuffer(GFXCommandBuffer& commandBuffer, GFXUniformBuffer* pUniformBuffer)
	{

	}

	GFXDevice& VulkanContext::GetDefaultDevice()
	{
		return (*mpDevice).CastAs<GFXDevice&>();
	}
}

