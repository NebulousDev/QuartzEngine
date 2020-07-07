#pragma once

#include "gfx\GFXContext.h"
#include "vulkan\vulkan.h"
#include "util\Array.h"

namespace Quartz
{
	/********************************************************************************************/

	#define OVERRIDE_USE_VALIDATION 0
	#define OVERRIDE_USE_DEBUG_NAMES 0

	#define USE_VALIDATION defined(QUARTZ_DEBUG) or OVERRIDE_USE_VALIDATION
	#define USE_DEBUG_NAMES (USE_VALIDATION) or OVERRIDE_USE_DEBUG_NAMES


	void QUARTZ_API SetDebugMarkerObjectName(VkDevice device, VkDebugReportObjectTypeEXT objectType, UInt64 object, const Char* name);
	void QUARTZ_API SetDebugMarkerObjectName(GFXDevice* pDevice, VkDebugReportObjectTypeEXT objectType, UInt64 object, const Char* name);


	/********************************************************************************************/


	class QUARTZ_API VulkanGFXQueue final : public GFXQueue
	{
	private:
		VkQueue mVkQueue;

	public:
		VulkanGFXQueue(VkQueue queue, GFXQueueInfo info);

		FORCE_INLINE VkQueue GetVkQueue() { return mVkQueue; }
	};


	/********************************************************************************************/


	class QUARTZ_API VulkanGFXQueueFamily final : public GFXQueueFamily
	{
	private:
		VkQueueFamilyProperties mQueueFamilyProperties;

	public:
		VulkanGFXQueueFamily(VkQueueFamilyProperties queueFamilyProperties, UInt32 index);

		FORCE_INLINE VkQueueFamilyProperties GetVkQueueFamilyProperties() { return mQueueFamilyProperties; }
	};


	/********************************************************************************************/


	class QUARTZ_API VulkanGFXPhysicalDevice final : public GFXPhysicalDevice
	{
	private:
		VkPhysicalDevice					mPhysicalDevice;
		VkPhysicalDeviceProperties			mPhysicalDeviceProperties;
		VkPhysicalDeviceFeatures			mPhysicalDeviceFeatures;
		VkPhysicalDeviceMemoryProperties	mPhysicalDeviceMemoryProperties;
		
	public:
		VulkanGFXPhysicalDevice(VkPhysicalDevice device);

		FORCE_INLINE VkPhysicalDevice					GetVkPhysicalDevice() { return mPhysicalDevice; }
		FORCE_INLINE VkPhysicalDeviceProperties			GetVkPhysicalDeviceProperties() { return mPhysicalDeviceProperties; }
		FORCE_INLINE VkPhysicalDeviceFeatures			GetVkPhysicalDeviceFeatures() { return mPhysicalDeviceFeatures; }
		FORCE_INLINE VkPhysicalDeviceMemoryProperties	GetVkPhysicalDeviceMemoryProperties() { return mPhysicalDeviceMemoryProperties; }
	};


	/********************************************************************************************/


	class QUARTZ_API VulkanGFXDevice final : public GFXDevice
	{
	private:
		VkDevice mVkDevice;

	public:
		VulkanGFXDevice(VkDevice device, GFXPhysicalDevice* pPhysicalDevice, GFXQueueMatrix queueMatrix);

		FORCE_INLINE VkDevice GetVkDevice() { return mVkDevice; }
	};


	/********************************************************************************************/


	class QUARTZ_API VulkanGFXBuffer final : public GFXBuffer
	{
	private:
		VkBuffer				mVkBuffer;
		VkDeviceMemory			mVkDeviceMemory;
		VkMemoryRequirements	mVkMemoryRequirements;

	public:
		VulkanGFXBuffer(VkBuffer buffer, GFXDevice* pDevice, VkDeviceMemory memory, 
			VkMemoryRequirements memoryRequirements, GFXBufferInfo info);

		FORCE_INLINE VkBuffer				GetVkBuffer() { return mVkBuffer; }
		FORCE_INLINE VkDeviceMemory			GetVkDeviceMemory() { return mVkDeviceMemory; }
		FORCE_INLINE VkMemoryRequirements	GetVkMemoryRequirements() { return mVkMemoryRequirements; }
	};


	/********************************************************************************************/


	class QUARTZ_API VulkanGFXImage final : public GFXImage
	{
	private:
		VkImage					mVkImage;
		VkFormat				mVkFormat;
		VkDeviceMemory			mVkDeviceMemory;	
		VkMemoryRequirements	mVkMemoryRequirements;

	public:
		VulkanGFXImage(GFXDevice* pDevice, VkImage image, VkFormat format, VkDeviceMemory deviceMemory,
			VkMemoryRequirements memoryRequirements, GFXImageInfo info);

		FORCE_INLINE VkImage				GetVkImage() { return mVkImage; }
		FORCE_INLINE VkFormat				GetVkFormat() { return mVkFormat; }
		FORCE_INLINE VkDeviceMemory			GetVkDeviceMemory() { return mVkDeviceMemory; }
		FORCE_INLINE VkMemoryRequirements	GetVkMemoryRequirements() { return mVkMemoryRequirements; }
	};

	class QUARTZ_API VulkanGFXImageView final : public GFXImageView
	{
	private:
		VkImageView	mVkImageView;

	public:
		VulkanGFXImageView(GFXDevice* pDevice, GFXImage* pImage, VkImageView imageView, GFXImageViewInfo info);

		FORCE_INLINE VkImageView GetVkImageView() { return mVkImageView; }
	};


	/********************************************************************************************/


	class QUARTZ_API VulkanGFXShader final : public GFXShader
	{
	private:
		VkShaderModule					mVkShaderModule;
		VkPipelineShaderStageCreateInfo mVkPipelineStageInfo;

	public:
		VulkanGFXShader(GFXDevice* pDevice, VkShaderModule module, VkPipelineShaderStageCreateInfo pipelineStageInfo, GFXShaderInfo info);

		FORCE_INLINE VkShaderModule						GetVkShaderModule() { return mVkShaderModule; }
		FORCE_INLINE VkPipelineShaderStageCreateInfo	GetVkPipelineShaderStageCreateInfo() { return mVkPipelineStageInfo; }
	};


	/********************************************************************************************/


	class QUARTZ_API VulkanGFXVertexInputState final : public GFXVertexInputState
	{
	private:
		VkPipelineVertexInputStateCreateInfo	mVkVertexInputStateInfo;
		VkVertexInputBindingDescription*		mpVkVertexInputBindingDescriptions;
		VkVertexInputAttributeDescription*		mpVkVertexInputAttributeDescriptions;

	public:
		VulkanGFXVertexInputState(VkPipelineVertexInputStateCreateInfo vertexInputStateInfo, 
			VkVertexInputBindingDescription* pVkVertexInputBindingDescriptions,
			VkVertexInputAttributeDescription* pVkVertexInputAttributeDescriptions, 
			GFXVertexInputStateInfo info);
		
		FORCE_INLINE VkPipelineVertexInputStateCreateInfo	GetVkVertexInputStateInfo() { return mVkVertexInputStateInfo; }
		FORCE_INLINE VkVertexInputBindingDescription*		GetVkVertexInputBindingDescriptions() { return mpVkVertexInputBindingDescriptions; }
		FORCE_INLINE VkVertexInputAttributeDescription*		GetVkVertexInputAttributeDescriptions() { return mpVkVertexInputAttributeDescriptions; }
	};


	/********************************************************************************************/


	class QUARTZ_API VulkanGFXInputAssemblyState final : public GFXInputAssemblyState
	{
	private:
		VkPipelineInputAssemblyStateCreateInfo mVkInputAssemblyStateInfo;

	public:
		VulkanGFXInputAssemblyState(VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateInfo, GFXInputAssemblyStateInfo info);

		FORCE_INLINE VkPipelineInputAssemblyStateCreateInfo GetVkInputAssemblyStateInfo() { return mVkInputAssemblyStateInfo; }
	};


	/********************************************************************************************/


	class QUARTZ_API VulkanGFXViewportState final : public GFXViewportState
	{
	private:
		VkPipelineViewportStateCreateInfo	mVkViewportStateInfo;
		VkViewport*							mpVkViewport;
		VkRect2D*							mpVkScissor;

	public:
		VulkanGFXViewportState(VkPipelineViewportStateCreateInfo viewportStateInfo, VkViewport* pViewport, 
			VkRect2D* pScissor, GFXViewportStateInfo info);

		~VulkanGFXViewportState();

		FORCE_INLINE VkPipelineViewportStateCreateInfo	GetVkViewportStateInfo() { return mVkViewportStateInfo; }
		FORCE_INLINE VkViewport*						GetVkViewport() { return mpVkViewport; }
		FORCE_INLINE VkRect2D*							GetVkScissor() { return mpVkScissor; }
	};


	/********************************************************************************************/


	class QUARTZ_API VulkanGFXRasterizationState final : public GFXRasterizationState
	{
	private:
		VkPipelineRasterizationStateCreateInfo mVkRasterizationStateInfo;

	public:
		VulkanGFXRasterizationState(VkPipelineRasterizationStateCreateInfo rasterizationStateInfo, GFXRasterizationStateInfo info);

		FORCE_INLINE VkPipelineRasterizationStateCreateInfo GetVkRasterizationStateInfo() { return mVkRasterizationStateInfo; }
	};


	/********************************************************************************************/


	class QUARTZ_API VulkanGFXMultisampleState final : public GFXMultisampleState
	{
	private:
		VkPipelineMultisampleStateCreateInfo mVkMultisampleStateInfo;

	public:
		VulkanGFXMultisampleState(VkPipelineMultisampleStateCreateInfo multisampleStateInfo, GFXMultisampleStateInfo info);

		FORCE_INLINE VkPipelineMultisampleStateCreateInfo GetVkMultisampleStateInfo() { return mVkMultisampleStateInfo; }
	};


	/********************************************************************************************/


	class QUARTZ_API VulkanGFXColorBlendState final : public GFXColorBlendState
	{
	private:
		VkPipelineColorBlendStateCreateInfo		mVkColorBlendStateInfo;
		VkPipelineColorBlendAttachmentState*	mpVkColorBlendAttachments;

	public:
		VulkanGFXColorBlendState(VkPipelineColorBlendStateCreateInfo colorBlendState, 
			VkPipelineColorBlendAttachmentState* colorBlendAttachments, GFXColorBlendStateInfo info);

		~VulkanGFXColorBlendState();

		FORCE_INLINE VkPipelineColorBlendStateCreateInfo GetVkColorBlendStateInfo() { return mVkColorBlendStateInfo; }
		FORCE_INLINE VkPipelineColorBlendAttachmentState* GetVkColorBlendAttachments() { return mpVkColorBlendAttachments; }
	};


	/********************************************************************************************/


	class QUARTZ_API VulkanGFXPipelineLayout final : public GFXPipelineLayout
	{
	private:
		VkPipelineLayout mVkPipelineLayout;

	public:
		VulkanGFXPipelineLayout(GFXDevice* pDevice, VkPipelineLayout pipelineLayout, GFXPipelineLayoutInfo info);

		FORCE_INLINE VkPipelineLayout GetVkPipelineLayout() { return mVkPipelineLayout; }
	};


	/********************************************************************************************/


	class QUARTZ_API VulkanGFXRenderAttachment final : public GFXRenderAttachment
	{
	private:
		VkAttachmentDescription mVkAttachmentDescription;

	public:
		VulkanGFXRenderAttachment(VkAttachmentDescription description, GFXRenderAttachmentInfo info);

		FORCE_INLINE VkAttachmentDescription GetVkAttachmentDescription() { return mVkAttachmentDescription; }
	};


	/********************************************************************************************/


	class QUARTZ_API VulkanGFXRenderSubpass final : public GFXRenderSubpass
	{
	private:
		VkSubpassDescription	mVkSubpassDescription;
		VkAttachmentReference*	mpVkInputAttachments;
		VkAttachmentReference*	mpVkColorAttachments;
		VkAttachmentReference*	mpVkResolveAttachments;
		VkAttachmentReference*	mpVkDepthStencilAttachment;

	public:
		VulkanGFXRenderSubpass(VkSubpassDescription subpassDescription, VkAttachmentReference* pInputAttachments, 
			VkAttachmentReference* pColorAttachments, VkAttachmentReference* pResolveAttachments, 
			VkAttachmentReference* pDepthStencilAttachment, GFXRenderSubpassInfo info);

		~VulkanGFXRenderSubpass();

		FORCE_INLINE VkSubpassDescription	GetVkSubpassDescription() const { return mVkSubpassDescription; }
		FORCE_INLINE VkAttachmentReference* GetVkInputAttachments() const { return mpVkInputAttachments; }
		FORCE_INLINE VkAttachmentReference* GetVkColorAttachments() const { return mpVkColorAttachments; }
		FORCE_INLINE VkAttachmentReference* GetVkResolveAttachments() const { return mpVkResolveAttachments; }
		FORCE_INLINE VkAttachmentReference* GetVkDepthStencilAttachment() const { return mpVkDepthStencilAttachment; }
	};


	/********************************************************************************************/


	class QUARTZ_API VulkanGFXRenderPass final : public GFXRenderPass
	{
	private:
		VkRenderPass				mVkRenderPass;
		VkAttachmentDescription*	mpVkAttachments;
		VkSubpassDependency*		mpVkDependencies;
		VkSubpassDescription*		mpVkSubpasses;

	public:
		VulkanGFXRenderPass(GFXDevice* pDevice, VkRenderPass renderPass, VkAttachmentDescription* pAttachments, 
			VkSubpassDependency* pDependencies, VkSubpassDescription* pSubpasses, GFXRenderPassInfo info);

		~VulkanGFXRenderPass();

		FORCE_INLINE VkRenderPass				GetVkRenderPass() const { return mVkRenderPass; }
		FORCE_INLINE VkAttachmentDescription*	GetVkAttachments() const { return mpVkAttachments; }
		FORCE_INLINE VkSubpassDependency*		GetVkDependencies() const { return mpVkDependencies; }
		FORCE_INLINE VkSubpassDescription*		GetVkSubpasses() const { return mpVkSubpasses; }
	};


	/********************************************************************************************/


	class QUARTZ_API VulkanGFXDescriptorSetLayout final : public GFXDescriptorSetLayout
	{
	private:
		VkDescriptorSetLayout			mVkDescriptorSetLayout;
		VkDescriptorSetLayoutBinding*	mpVkDescriptorSetLayoutBindings;

	public:
		VulkanGFXDescriptorSetLayout(GFXDevice* pDevice, VkDescriptorSetLayout descriptorSetLayout, 
			VkDescriptorSetLayoutBinding* pVkDescriptorSetLayoutBindings, GFXDescriptorSetLayoutInfo info);
	
		FORCE_INLINE VkDescriptorSetLayout GetVkDescriptorSetLayout() const { return mVkDescriptorSetLayout; }
	};


	/********************************************************************************************/


	class QUARTZ_API VulkanGFXPipeline final : public GFXPipeline
	{
	private:
		VkPipeline							mVkPipeline;
		VkPipelineShaderStageCreateInfo*	mpVkShaderStages;

	public:
		VulkanGFXPipeline(GFXDevice* pDevice, VkPipeline pipeline, 
			VkPipelineShaderStageCreateInfo* pShaderStages, GFXPipelineInfo info);

		~VulkanGFXPipeline();

		FORCE_INLINE VkPipeline GetVkPipeline() const { return mVkPipeline; }

	};


	/********************************************************************************************/


	class QUARTZ_API VulkanGFXFramebuffer final : public GFXFramebuffer
	{
	private:
		VkFramebuffer mVkFramebuffer;

	public:
		VulkanGFXFramebuffer(GFXDevice* pDevice, VkFramebuffer framebuffer, GFXFramebufferInfo info);
		~VulkanGFXFramebuffer();

		FORCE_INLINE VkFramebuffer GetVkFramebuffer() const { return mVkFramebuffer; }
	};


	/********************************************************************************************/


	class QUARTZ_API VulkanGFXCommandPool final : public GFXCommandPool
	{
	private:
		VkCommandPool mVkCommandPool;
	public:
		VulkanGFXCommandPool(GFXDevice* pDevice, VkCommandPool pool, GFXCommandPoolInfo info);
		~VulkanGFXCommandPool();

		FORCE_INLINE VkCommandPool GetVkCommandPool() const { return mVkCommandPool; }
	};


	/********************************************************************************************/


	class QUARTZ_API VulkanGFXModule final : public GFXModule
	{
	private:
		VkInstance mVkInstance;

		Bool8 mVulkanInitialized;

		// TODO: replace with a Set for faster lookups
		Array<VkLayerProperties>		mAvailableLayers; 
		Array<const Char*>				mValidationLayers;
		UInt32							mAvaliableLayerCount;
		UInt32							mEnabledLayerCount;

		// TODO: replace with a Set for faster lookups
		Array<VkExtensionProperties>	mAvailableExtensions;
		Array<const Char*>				mExtensions;
		UInt32							mAvailableExtensionCount;
		UInt32							mEnabledExtensionCount;

		GFXPhysicalDeviceList			mPhysicalDeviceList;
		GFXPhysicalDevice*				mpPrimaryPhysicalDevice;

		// TODO: replace with a Set for faster lookups
		GFXDeviceList					mDeviceList;

		VkDebugUtilsMessengerEXT		mDebugMessenger;


		void	EnumerateValidationLayers();
		Bool8	CheckValidationLayer(const Char* layerName);

		void	EnumerateExtensions();
		Bool8	CheckExtension(const Char* extensionName);

		void				EnumerateDevices(VkInstance instance);
		Bool8				ValidateDevice(GFXPhysicalDevice* pDevice);
		UInt32				CalcDeviceScore(GFXPhysicalDevice* pDevice);
		GFXPhysicalDevice*	FindPrimaryPhysicalDevice();

		Bool8		InitModuleImpl(const Char** pFailMessage) final override;
		Bool8		DestroyModuleImpl(const Char** pFailMessage) final override;
		const Char*	GetModuleNameImpl() const final override;

	public:
		Bool8 InitGraphicsContext() final override;
		Bool8 DestroyGraphicsContext() final override;

		GFXDevice*				CreateDevice(GFXPhysicalDevice* pPhysicalDevice, GFXDeviceInfo info) final override;
		void					DestroyDevice(GFXDevice* pDevice) final override;
		GFXBuffer*				CreateBuffer(GFXDevice* pDevice, GFXBufferInfo info) final override;
		void					DestroyBuffer(GFXBuffer* pBuffer) final override;
		GFXImageView*			CreateImageView(GFXImage* pImage, GFXImageViewInfo info);
		void					DestroyImageView(GFXImageView* pImageView) final override;
		GFXShader*				CreateShader(GFXDevice* pDevice, GFXShaderInfo info) final override;
		void					DestroyShader(GFXShader* pShader) final override;

		GFXVertexInputState*	CreateVertexInputState(GFXVertexInputStateInfo info) final override;
		void					DestroyVertexInputState(GFXVertexInputState* pVertexInputState) final override;
		GFXInputAssemblyState*	CreateInputAssemblyState(GFXInputAssemblyStateInfo info) final override;
		void					DestroyInputAssemblyState(GFXInputAssemblyState* pInputAssemblyState) final override;
		GFXViewportState*		CreateViewportState(GFXViewportStateInfo info) final override;
		void					DestroyViewportState(GFXViewportState* pViewportState) final override;
		GFXRasterizationState*	CreateRasterizationState(GFXRasterizationStateInfo info) final override;
		void					DestroyRasterizationState(GFXRasterizationState* pRasterizationState) final override;
		GFXMultisampleState*	CreateMultisampleState(GFXMultisampleStateInfo info) final override;
		void					DestroyMultisampleState(GFXMultisampleState* pMultisampleState) final override;
		GFXColorBlendState*		CreateColorBlendState(GFXColorBlendStateInfo info) final override;
		void					DestroyColorBlendState(GFXColorBlendState* pColorBlendState) final override;
		GFXPipelineLayout*		CreatePipelineLayout(GFXDevice* pDevice, GFXPipelineLayoutInfo info) final override;
		void					DestroyPipelineLatout(GFXPipelineLayout* pPipelineLayout) final override;
		GFXRenderAttachment*	CreateRenderAttachment(GFXRenderAttachmentInfo info) final override;
		void					DestroyRenderAttachment(GFXRenderAttachment* pRenderAttachment) final override;
		GFXRenderSubpass*		CreateRenderSubpass(GFXRenderSubpassInfo info) final override;
		void					DestroyRenderSubpass(GFXRenderSubpass* pRenderSubpass) final override;
		GFXRenderPass*			CreateRenderPass(GFXDevice* pDevice, GFXRenderPassInfo info) final override;
		void					DestroyRenderPass(GFXRenderPass* pRenderPass) final override;
		GFXDescriptorSetLayout* CreateDescriptorSetLayout(GFXDevice* pDevice, GFXDescriptorSetLayoutInfo info) final override;
		void					DestroyDescriptorSetLayout(GFXDescriptorSetLayout* pLayout) final override;
		GFXPipeline*			CreatePipeline(GFXDevice* pDevice, GFXPipelineInfo info) final override;
		void					DestroyPipleline(GFXPipeline* pPipeline) final override;
		GFXFramebuffer*			CreateFramebuffer(GFXDevice* pDevice, GFXFramebufferInfo info) final override;
		void					DestroyFramebuffer(GFXFramebuffer* pFramebuffer) final override;
		GFXCommandPool*			CreateCommandPool(GFXDevice* pDevice, GFXCommandPoolInfo info) final override;
		void					DestroyCommandPool(GFXCommandPool* pCommandPool) final override;

		Bool8 MapBufferMemory(GFXBuffer* pBuffer, USize offset, USize size, GFXBufferMapFlags flags, void** ppData) final override;
		Bool8 UnmapBufferMemory(GFXBuffer* pBuffer) final override;

		const GFXPhysicalDeviceList&	GetPhysicalDeviceList() const final override;
		GFXPhysicalDevice*				GetPrimaryPhysicalDevice() final override;

		FORCE_INLINE VkInstance GetVkInstance() const { return mVkInstance; }
	};


	/********************************************************************************************/
}