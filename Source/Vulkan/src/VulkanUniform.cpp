#include "VulkanUniform.h"

#include "Engine.h"
#include "VulkanViewport.h"

namespace Quartz
{
	VulkanUniform::VulkanUniform(VulkanDevice* pDevice, UniformType type, UInt32 elementSize, UInt32 elementCount, UniformFlags flags)
		: Uniform(type, elementSize, elementCount, flags),
		mpDevice(pDevice)
	{
		UInt32 minUboAlignment	= mpDevice->GetPhysicalDevice().GetPhysicalDeviceProperties().limits.minUniformBufferOffsetAlignment;
		UInt32 uboAlignedSize	= (mElementSize + minUboAlignment - 1) & ~(minUboAlignment - 1);

		mAlignedSize = uboAlignedSize;
	}

	void VulkanUniform::Set(void* pData)
	{
		// Dont use for now
	}

	void VulkanUniform::SetElement(Context* pContext, UInt32 element, void* pData)
	{
		// TODO: This is awful.
		VulkanContext*		pVulkanContext		= static_cast<VulkanContext*>(pContext);
		VulkanSwapchain*	pVulkanSwapchain	= pVulkanContext->GetSwapchain();
		UInt32				frameIndex			= pVulkanSwapchain->GetFrameIndex();

		memcpy_s(mpMappedBuffers[frameIndex] + (mAlignedSize * element), mAlignedSize, pData, mAlignedSize);
	}

	// Warning, data must be properly padded!!!
	void VulkanUniform::SetElementArray(Context* pContext, UInt32 startIndex, UInt32 endIndex, void* pData)
	{
		// TODO: This is awful.
		VulkanContext*		pVulkanContext		= static_cast<VulkanContext*>(pContext);
		VulkanSwapchain*	pVulkanSwapchain	= pVulkanContext->GetSwapchain();
		UInt32				frameIndex			= pVulkanSwapchain->GetFrameIndex();

		memcpy_s(mpMappedBuffers[frameIndex] + (mAlignedSize * startIndex), mAlignedSize * mElementCount, pData, mAlignedSize * (endIndex - startIndex));
	}

	void VulkanUniform::BuildBuffers(UInt32 bufferCount)
	{
		Graphics* pGraphics = Engine::GetInstance()->GetGraphics();

		if (IsUsingStaging())
		{
			for (VulkanBuffer* pOldBuffer : mpStagingBuffers)
			{
				pGraphics->DestroyBuffer(pOldBuffer);
			}

			for (VulkanBuffer* pOldBuffer : mpUniformBuffers)
			{
				pGraphics->DestroyBuffer(pOldBuffer);
			}

			mpStagingBuffers.Clear();
			mpUniformBuffers.Clear();
			mpMappedBuffers.Clear();

			mpStagingBuffers.Resize(bufferCount);
			mpUniformBuffers.Resize(bufferCount);
			mpMappedBuffers.Resize(bufferCount);

			for (UInt32 i = 0; i < bufferCount; i++)
			{
				mpStagingBuffers[i] = static_cast<VulkanBuffer*>
				(
					pGraphics->CreateBuffer(mAlignedSize * mElementCount,
						BUFFER_USAGE_UNIFORM_BUFFER_BIT | BUFFER_USAGE_TRANSFER_SRC_BIT, 
						BUFFER_ACCESS_HOST_VISIBLE_BIT | BUFFER_ACCESS_HOST_COHERENT_BIT)
				);

				mpUniformBuffers[i] = static_cast<VulkanBuffer*>
				(
					pGraphics->CreateBuffer(mAlignedSize * mElementCount,
						BUFFER_USAGE_UNIFORM_BUFFER_BIT | BUFFER_USAGE_TRANSFER_DEST_BIT, 
						BUFFER_ACCESS_DEVICE_LOCAL_BIT)
				);

				mpMappedBuffers[i] = (MappedData)mpStagingBuffers[i]->MapBuffer(mAlignedSize * mElementCount, 0);
			}
		}
		else
		{
			for (VulkanBuffer* pOldBuffer : mpUniformBuffers)
			{
				pGraphics->DestroyBuffer(pOldBuffer);
			}

			mpUniformBuffers.Clear();
			mpMappedBuffers.Clear();

			mpUniformBuffers.Resize(bufferCount);
			mpMappedBuffers.Resize(bufferCount);

			for (UInt32 i = 0; i < bufferCount; i++)
			{
				mpUniformBuffers[i] = static_cast<VulkanBuffer*>
				(
					pGraphics->CreateBuffer(mAlignedSize * mElementCount,
						BUFFER_USAGE_UNIFORM_BUFFER_BIT, 
						BUFFER_ACCESS_HOST_VISIBLE_BIT | BUFFER_ACCESS_HOST_COHERENT_BIT)
				);

				mpMappedBuffers[i] = (MappedData)mpUniformBuffers[i]->MapBuffer(mAlignedSize * mElementCount, 0);
			}
		}
		
		mBuilt = true;
	}

	void VulkanUniformTextureSampler::Set(ImageView* pImageView)
	{
		mpImageView = static_cast<VulkanImageView*>(pImageView);
	}
}
