#pragma once

#include "graphics2/GraphicsModule.h"

#include "VulkanSwapchain.h"
#include "VulkanSurface.h"
#include "VulkanDevice.h"
#include "VulkanPipeline.h"
#include "VulkanCommandBuffer.h"

namespace Quartz
{
	class QUARTZ_API VulkanGraphics : public Graphics
	{
	protected:
		VkInstance						mvkInstance;
		Array<VulkanPhysicalDevice>		mPhysicalDevices;
		VulkanDevice*					mpDevice;
		Array<VkLayerProperties>		mAvailableLayers;
		Array<VkExtensionProperties>	mAvailableExtensions;
		Array<VkPhysicalDevice>			mAvailablePhysicalDevices;
		Array<const char*>				mEnabledValidationLayerNames;
		Array<const char*>				mEnabledExtensionNames;

	private:
		VkResult CreateVkImageView(VkImageView* pVkImageView, VkImage vkImage, 
			VkImageViewType vkImageViewType,VkImageAspectFlags vkAspectFlags, 
			VkFormat vkFormat, UInt32 mipStart, UInt32 mipLevels, UInt32 layerStart, UInt32 layers);

		VulkanSwapchain* CreateSwapchain(VulkanSurface* pSurface, UInt32 bufferCount);

		void TransitionImage(VulkanImage* pImage, VkImageLayout oldLayout, VkImageLayout newLayout);

	public:
		VulkanGraphics();

		Viewport*		CreateViewport(Surface* pSurface, Scene* pScene, Renderer* pRenderer) override;
		void			DestroyViewport(Viewport* pViewport) override;

		virtual Image*	CreateImage(ImageType type, UInt32 width, UInt32 height, UInt32 depth,
							UInt32 layers, UInt32 mips, ImageFormat format, ImageUsages usages) override;
		virtual void	DestroyImage(Image* pImage) override;

		ImageView*		CreateImageView(Image* pImage, ImageViewType type,
							UInt32 width, UInt32 height, UInt32 depth,
							UInt32 layers, UInt32 layerStart,
							UInt32 mips, UInt32 mipStart, 
							ImageFormat format, ImageViewUsage usage) override;
		void			DestroyImageView(ImageView* pImageView) override;

		Buffer*			CreateBuffer(UInt32 sizeBytes, BufferUsages usages, BufferAccess access) override;
		void			DestroyBuffer(Buffer* pBuffer) override;

		Renderpass*		CreateRenderpass(const String& name,
							const Array<Attachment>& attachments, const Array<Subpass>& subpasses) override;
		void			DestroyRenderpass(Renderpass* pRenderpass) override;

		Framebuffer*	CreateFramebuffer(Renderpass* pRenderpass, Viewport* pViewport, UInt32 width, UInt32 height) override;
		void			DestroyFramebuffer(Framebuffer* pFramebuffer) override;

		Shader*			CreateShader(const String& name, const Array<Byte>& binary) override;
		void			DestroyShader(Shader* pShader) override;

		GraphicsPipeline*	CreateGraphicsPipeline(const GraphicsPipelineInfo& info, UInt32 subpass) override;
		void				DestroyGraphicsPipeline(GraphicsPipeline* pPipeline) override;

		CommandBuffer*	CreateCommandBuffer() override;
		void			DestroyCommandBuffer(CommandBuffer* pCommandBuffer) override;

		void			CopyBuffer(Buffer* pSource, Buffer* pDest) override;

		void			BindUniformBuffer(Pipeline* pPipeline, UInt32 set, UInt32 buffer, Buffer* pBuffer) override;

		void			Submit(Viewport* pViewport, CommandBuffer* pCommandBuffer) override;

		Bool8 Init() override;

		void Tick() override;
	};
}