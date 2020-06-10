#include "GFXContext.h"

//#include "io/Log.h"

#include "gfx/GFXPlatform.h"

namespace Quartz
{
	/********************************************************************************************/


	Bool8 GFXContext::SetGFXModule(GFXModule* pModule, Bool8 destroyOnReplace)
	{
		if (!pModule->IsModuleInitialized())
		{
			if (!pModule->InitModule())
			{
				//Log.Error("Failed to set graphics module to '%s'!", pModule->GetModuleName());
				return false;
			}
		}

		if (mpGFX && destroyOnReplace)
		{
			if (!mpGFX->DestroyModule())
			{
				//Log.Error("Failed to set graphics module to '%s'!", pModule->GetModuleName());
				return false;
			}
		}

		mpGFX = pModule;

		//Log.Info("GFXContext graphics module set to '%s'", pModule->GetModuleName());

		return true;
	}


	/********************************************************************************************/


	Bool8 GFXContext::SetGFXPlatformModule(GFXPlatformModule* pPlatformModule, Bool8 destroyOnReplace)
	{
		if (!pPlatformModule->IsModuleInitialized())
		{
			if (!pPlatformModule->InitModule())
			{
				//Log.Error("Failed to set graphics platform module to '%s'!", pPlatformModule->GetModuleName());
				return false;
			}
		}

		if (mpPlatformGFX && destroyOnReplace)
		{
			if (!mpPlatformGFX->DestroyModule())
			{
				//Log.Error("Failed to set graphics platform module to '%s'!", pPlatformModule->GetModuleName());
				return false;
			}
		}

		mpPlatformGFX = pPlatformModule;

		//Log.Info("GFXContext graphics platform module set to '%s'", pPlatformModule->GetModuleName());

		return true;
	}


	/********************************************************************************************/


	Bool8 GFXContext::InitGraphicsContext()
	{
		#ifdef QUARTZ_DEBUG

			if (!mpGFX || !mpGFX->IsModuleInitialized() || !mpPlatformGFX || !mpGFX->IsModuleInitialized())
			{
				//Log.Error("Failed to initialize graphics context! GFXModule is not loaded!");
				return false;
			}

			if (!mpGFX->InitGraphicsContext())
			{
				//Log.Error("Failed to initialize graphics context with module: '%s'!", mpGFX->GetModuleName());
				return false;
			}

			return true;

		#else

			return mpGFX->InitGraphicsContext();

		#endif
	}


	/********************************************************************************************/


	Bool8 GFXContext::DestroyGraphicsContext()
	{
		#ifdef QUARTZ_DEBUG

			if (!mpGFX || !mpGFX->IsModuleInitialized() || mpGFX->IsModuleDestroyed())
			{
				//Log.Error("Failed to destroy graphics context! GFXModule is not loaded!");
				return false;
			}

			if (!mpGFX->DestroyGraphicsContext())
			{
				//Log.Error("Failed to destroy graphics context with module: '%s'!", mpGFX->GetModuleName());
				return false;
			}

			return true;

		#else
	
			return mpGFX->DestroyGraphicsContext();

		#endif
	}


	/********************************************************************************************/


	GFXDevice* GFXContext::CreateDevice(GFXPhysicalDevice* pPhysicalDevice, GFXDeviceInfo info)
	{
		return mpGFX->CreateDevice(pPhysicalDevice, info);
	}


	/********************************************************************************************/


	void GFXContext::DestroyDevice(GFXDevice* pDevice)
	{
		mpGFX->DestroyDevice(pDevice);
	}


	/********************************************************************************************/


	GFXSurface* GFXContext::CreateSurface(GFXDevice* pDevice, GFXSurfaceInfo info)
	{
		return mpPlatformGFX->CreateSurface(mpGFX, pDevice, info);
	}


	/********************************************************************************************/


	void GFXContext::DestroySurface(GFXSurface* pSurface)
	{
		mpPlatformGFX->DestroySurface(mpGFX, pSurface);
	}


	/********************************************************************************************/


	GFXSwapchain* GFXContext::CreateSwapchain(GFXDevice* pDevice, GFXSurface* pSurface, GFXSwapchainInfo info)
	{
		return mpPlatformGFX->CreateSwapchain(pDevice, pSurface, info);
	}


	/********************************************************************************************/


	void GFXContext::DestroySwapchain(GFXSwapchain* pSwapchain)
	{
		mpPlatformGFX->DestroySwapchain(pSwapchain);
	}


	/********************************************************************************************/


	GFXBuffer* GFXContext::CreateBuffer(GFXDevice* pDevice, GFXBufferInfo info)
	{
		return mpGFX->CreateBuffer(pDevice, info);
	}


	/********************************************************************************************/


	void GFXContext::DestroyBuffer(GFXBuffer* pBuffer)
	{
		mpGFX->DestroyBuffer(pBuffer);
	}


	/********************************************************************************************/


	GFXImageView* GFXContext::CreateImageView(GFXImage* pImage, GFXImageViewInfo info)
	{
		return mpGFX->CreateImageView(pImage, info);
	}


	/********************************************************************************************/


	void GFXContext::DestroyImageView(GFXImageView* pImageView)
	{
		mpGFX->DestroyImageView(pImageView);
	}


	/********************************************************************************************/


	GFXShader* GFXContext::CreateShader(GFXDevice* pDevice, GFXShaderInfo info)
	{
		return mpGFX->CreateShader(pDevice, info);
	}


	/********************************************************************************************/


	void GFXContext::DestroyShader(GFXShader* pShader)
	{
		return mpGFX->DestroyShader(pShader);
	}


	/********************************************************************************************/


	GFXVertexInputState* GFXContext::CreateVertexInputState(GFXVertexInputStateInfo info)
	{
		return mpGFX->CreateVertexInputState(info);
	}


	/********************************************************************************************/


	void GFXContext::DestroyVertexInputState(GFXVertexInputState* info)
	{
		mpGFX->DestroyVertexInputState(info);
	}


	/********************************************************************************************/


	GFXInputAssemblyState* GFXContext::CreateInputAssemblyState(GFXInputAssemblyStateInfo info)
	{
		return mpGFX->CreateInputAssemblyState(info);
	}


	/********************************************************************************************/


	void GFXContext::DestroyInputAssemblyState(GFXInputAssemblyState* pInputAssemblyState)
	{
		mpGFX->DestroyInputAssemblyState(pInputAssemblyState);
	}


	/********************************************************************************************/


	GFXViewportState* GFXContext::CreateViewportState(GFXViewportStateInfo info)
	{
		return mpGFX->CreateViewportState(info);
	}


	/********************************************************************************************/


	void GFXContext::DestroyViewportState(GFXViewportState* pViewportState)
	{
		mpGFX->DestroyViewportState(pViewportState);
	}


	/********************************************************************************************/


	GFXRasterizationState* GFXContext::CreateRasterizationState(GFXRasterizationStateInfo info)
	{
		return mpGFX->CreateRasterizationState(info);
	}


	/********************************************************************************************/


	void GFXContext::DestroyRasterizationState(GFXRasterizationState* pRasterizationState)
	{
		mpGFX->DestroyRasterizationState(pRasterizationState);
	}


	/********************************************************************************************/


	GFXMultisampleState* GFXContext::CreateMultisampleState(GFXMultisampleStateInfo info)
	{
		return mpGFX->CreateMultisampleState(info);
	}


	/********************************************************************************************/


	void GFXContext::DestroyMultisampleState(GFXMultisampleState* pMultisampleState)
	{
		mpGFX->DestroyMultisampleState(pMultisampleState);
	}


	/********************************************************************************************/


	GFXColorBlendState* GFXContext::CreateColorBlendState(GFXColorBlendStateInfo info)
	{
		return mpGFX->CreateColorBlendState(info);
	}


	/********************************************************************************************/


	void GFXContext::DestroyColorBlendState(GFXColorBlendState* pColorBlendState)
	{
		mpGFX->DestroyColorBlendState(pColorBlendState);
	}


	/********************************************************************************************/


	GFXPipelineLayout* GFXContext::CreatePipelineLayout(GFXDevice* pDevice, GFXPipelineLayoutInfo info)
	{
		return mpGFX->CreatePipelineLayout(pDevice, info);
	}


	/********************************************************************************************/


	void GFXContext::DestroyPipelineLatout(GFXPipelineLayout* pPipelineLayout)
	{
		mpGFX->DestroyPipelineLatout(pPipelineLayout);
	}


	/********************************************************************************************/


	GFXRenderAttachment* GFXContext::CreateRenderAttachment(GFXRenderAttachmentInfo info)
	{
		return mpGFX->CreateRenderAttachment(info);
	}


	/********************************************************************************************/


	void GFXContext::DestroyRenderAttachment(GFXRenderAttachment* pRenderAttachment)
	{
		mpGFX->DestroyRenderAttachment(pRenderAttachment);
	}


	/********************************************************************************************/


	GFXRenderSubpass* GFXContext::CreateRenderSubpass(GFXRenderSubpassInfo info)
	{
		return mpGFX->CreateRenderSubpass(info);
	}


	/********************************************************************************************/


	void GFXContext::DestroyRenderSubpass(GFXRenderSubpass* pRenderSubpass)
	{
		mpGFX->DestroyRenderSubpass(pRenderSubpass);
	}


	/********************************************************************************************/


	GFXRenderPass* GFXContext::CreateRenderPass(GFXDevice* pDevice, GFXRenderPassInfo info)
	{
		return mpGFX->CreateRenderPass(pDevice, info);
	}


	/********************************************************************************************/


	void GFXContext::DestroyRenderPass(GFXRenderPass* pRenderPass)
	{
		mpGFX->DestroyRenderPass(pRenderPass);
	}


	/********************************************************************************************/


	GFXDescriptorSetLayout* GFXContext::CreateDescriptorSetLayout(GFXDevice* pDevice, GFXDescriptorSetLayoutInfo info)
	{
		return mpGFX->CreateDescriptorSetLayout(pDevice, info);
	}


	/********************************************************************************************/


	void GFXContext::DestroyDescriptorSetLayout(GFXDescriptorSetLayout* pLayout)
	{
		mpGFX->DestroyDescriptorSetLayout(pLayout);
	}


	/********************************************************************************************/


	GFXPipeline* GFXContext::CreatePipeline(GFXDevice* pDevice, GFXPipelineInfo info)
	{
		return mpGFX->CreatePipeline(pDevice, info);
	}


	/********************************************************************************************/


	void GFXContext::DestroyPipleline(GFXPipeline* pPipeline)
	{
		return mpGFX->DestroyPipleline(pPipeline);
	}


	/********************************************************************************************/


	GFXFramebuffer* GFXContext::CreateFramebuffer(GFXDevice* pDevice, GFXFramebufferInfo info)
	{
		return mpGFX->CreateFramebuffer(pDevice, info);
	}
	
	
	/********************************************************************************************/


	void GFXContext::DestroyFramebuffer(GFXFramebuffer* pFramebuffer)
	{
		return mpGFX->DestroyFramebuffer(pFramebuffer);
	}


	/********************************************************************************************/


	GFXCommandPool* GFXContext::CreateCommandPool(GFXDevice* pDevice, GFXCommandPoolInfo info)
	{
		return mpGFX->CreateCommandPool(pDevice, info);
	}


	/********************************************************************************************/


	void GFXContext::DestroyCommandPool(GFXCommandPool* pCommandPool)
	{
		mpGFX->DestroyCommandPool(pCommandPool);
	}


	/********************************************************************************************/


	GFXImageArray GFXContext::GetSwapChainImages(GFXSwapchain* pSwapchain)
	{
		return mpPlatformGFX->GetSwapChainImages(pSwapchain);
	}


	/********************************************************************************************/


	Bool8 GFXContext::MapBufferMemory(GFXBuffer* pBuffer, USize offset, USize size, GFXBufferMapFlags flags, void** ppData)
	{
		return mpGFX->MapBufferMemory(pBuffer, offset, size, flags, ppData);
	}


	/********************************************************************************************/


	Bool8 GFXContext::UnmapBufferMemory(GFXBuffer* pBuffer)
	{
		return mpGFX->UnmapBufferMemory(pBuffer);
	}


	/********************************************************************************************/


	GFXPhysicalDevice* GFXContext::GetPrimaryPhysicalDevice()
	{
		return mpGFX->GetPrimaryPhysicalDevice();
	}


	/********************************************************************************************/


	const GFXPhysicalDeviceList& GFXContext::GetPhysicalDeviceList() const
	{
		return mpGFX->GetPhysicalDeviceList();
	}


	/********************************************************************************************/


	GFXQueue* GFXDevice::GetQueue(UInt32 familyIndex, UInt32 queueIndex)
	{
		return mQueueMatrix[familyIndex][queueIndex];
	}


	/********************************************************************************************/


	const GFXQueueList& GFXDevice::GetQueueList(UInt32 familyIndex)
	{
		return mQueueMatrix[familyIndex];
	}


	/********************************************************************************************/
}

