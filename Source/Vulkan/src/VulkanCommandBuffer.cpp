#include "VulkanCommandBuffer.h"

#include "VulkanRenderpass.h"
#include "VulkanFramebuffer.h"
#include "VulkanPipeline.h"
#include "VulkanBuffer.h"

#include "log/Log.h"

namespace Quartz
{
	VulkanCommandBuffer::VulkanCommandBuffer(VulkanDevice* pDevice, CommandBufferType type)
		: CommandBuffer(type),
		mpDevice(pDevice),
		mvkCommandPool(VK_NULL_HANDLE),
		mUsages(0),
		mBuilt(false),
		mState{}
	{
		// Nothing
	}

	void VulkanCommandBuffer::BeginRecording()
	{
		// TODO: all commands should be allocated from a pool to avoid this
		for (VulkanCommand* pCommand : mCommandList)
		{
			delete pCommand;
		}

		mCommandList.Clear();
	}

	void VulkanCommandBuffer::EndRecording()
	{
		
	}

	void VulkanCommandBuffer::BeginRenderpass(Renderpass* pRenderpass, Framebuffer* pFramebuffer)
	{
		VulkanCommandBeginRenderpass* pCommand = new VulkanCommandBeginRenderpass();
		pCommand->pRenderpass	= static_cast<VulkanRenderpass*>(pRenderpass);
		pCommand->pFramebuffer	= static_cast<VulkanFramebuffer*>(pFramebuffer);

		mCommandList.PushBack(pCommand);
	}

	void VulkanCommandBuffer::EndRenderpass()
	{
		mCommandList.PushBack(new VulkanCommandEndRenderpass());
	}

	void VulkanCommandBuffer::SetPipeline(Pipeline* pPipeline)
	{
		if (pPipeline->GetPipelineType() == PIPELINE_TYPE_GRAPHICS)
		{
			VulkanCommandSetGraphicsPipeline* pCommand = new VulkanCommandSetGraphicsPipeline();
			pCommand->pPipeline = static_cast<VulkanGraphicsPipeline*>(pPipeline);

			mCommandList.PushBack(pCommand);

			mUsages |= COMMAND_BUFFER_USAGE_GRAPHICS_BIT;
		}
		else
		{
			// Compute
		}
	}

	void VulkanCommandBuffer::SetVertexBuffers(const Array<Buffer*>& buffers)
	{
		VulkanCommandSetVertexBuffers* pCommand = new VulkanCommandSetVertexBuffers();
		pCommand->buffers = reinterpret_cast<const Array<VulkanBuffer*>&>(buffers); // @NOTE: possibly dangerous conversion?

		mCommandList.PushBack(pCommand);
	}

	void VulkanCommandBuffer::SetIndexBuffer(Buffer* pBuffer)
	{
		VulkanCommandSetIndexBuffer* pCommand = new VulkanCommandSetIndexBuffer();
		pCommand->pBuffer = static_cast<VulkanBuffer*>(pBuffer);

		mCommandList.PushBack(pCommand);
	}

	void VulkanCommandBuffer::BindUniform(UInt32 set, UInt32 binding, Uniform* pUniform, UInt32 element)
	{
		VulkanCommandBindUniformBuffer* pCommand = new VulkanCommandBindUniformBuffer();
		pCommand->set		= set;
		pCommand->binding	= binding;
		pCommand->pUniform	= static_cast<VulkanUniform*>(pUniform);
		pCommand->element	= element;

		mCommandList.PushBack(pCommand);
	}

	void VulkanCommandBuffer::BindUniformTexture(UInt32 set, UInt32 binding, UniformTextureSampler* pUniformTextureSampler)
	{
		VulkanCommandBindUniformTextureSampler* pCommand = new VulkanCommandBindUniformTextureSampler();
		pCommand->set						= set;
		pCommand->binding					= binding;
		pCommand->pUniformTextureSampler	= static_cast<VulkanUniformTextureSampler*>(pUniformTextureSampler);

		mCommandList.PushBack(pCommand);
	}

	void VulkanCommandBuffer::DrawIndexed(UInt32 count, UInt32 start)
	{
		VulkanCommandDrawIndexed* pCommand = new VulkanCommandDrawIndexed();
		pCommand->count = count;
		pCommand->start = start;

		mCommandList.PushBack(pCommand);
	}

	void VulkanCommandBuffer::BuildBuffers(UInt32 bufferCount)
	{
		if (mBuilt)
		{
			vkFreeCommandBuffers(mpDevice->GetDeviceHandle(), mvkCommandPool, mCommandBuffers.Size(), mCommandBuffers.Data());
		}

		if (mUsages == COMMAND_BUFFER_USAGE_GRAPHICS_BIT)
		{
			mvkCommandPool = mpDevice->GetGraphicsCommandPoolHandle();
		}
		else if (mUsages == COMMAND_BUFFER_USAGE_COMPUTE_BIT)
		{
			mvkCommandPool = mpDevice->GetComputeCommandPoolHandle();
		}
		else if (mUsages == (COMMAND_BUFFER_USAGE_GRAPHICS_BIT & COMMAND_BUFFER_USAGE_COMPUTE_BIT))
		{
			mvkCommandPool = mpDevice->GetGraphicsCommandPoolHandle();
		}
		else if (mUsages == (COMMAND_BUFFER_USAGE_GRAPHICS_BIT & COMMAND_BUFFER_USAGE_TRANSFER_BIT))
		{
			mvkCommandPool = mpDevice->GetGraphicsCommandPoolHandle();
		}
		else if (mUsages == (COMMAND_BUFFER_USAGE_COMPUTE_BIT & COMMAND_BUFFER_USAGE_TRANSFER_BIT))
		{
			mvkCommandPool = mpDevice->GetComputeCommandPoolHandle();
		}
		else //COMMAND_BUFFER_USAGE_TRANSFER_BIT
		{
			mvkCommandPool = mpDevice->GetTransferCommandPoolHandle();
		}

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType					= VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool			= mvkCommandPool;
		allocInfo.level					= VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount	= bufferCount;

		if (mCommandBuffers.Size() != bufferCount)
		{
			mCommandBuffers.Clear();
			mCommandBuffers.Resize(bufferCount);
		}

		vkAllocateCommandBuffers(mpDevice->GetDeviceHandle(), &allocInfo, mCommandBuffers.Data());

		mBuilt = true;
	}

	void VulkanCommandBuffer::RecordStatic()
	{
		// TODO: should be a debug assert
		if (mType != COMMAND_BUFFER_STATIC)
		{
			Log::Critical("Called BuildStatic() on a dynamic commandbuffer!!!");
			return;
		}

		for (UInt32 i = 0; i < mCommandBuffers.Size(); i++)
		{
			RecordDynamic(i);
		}

		mBuilt = true;
	}

	void VulkanCommandBuffer::RecordDynamic(UInt32 frameIndex)
	{
		// TODO: check index
		VkCommandBuffer vkCommandBuffer = mCommandBuffers[frameIndex];

		// buffer is implicitly reset with vkBeginCommandBuffer
		//vkResetCommandBuffer(vkCommandBuffer, VK_NULL_HANDLE);

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

		if (vkBeginCommandBuffer(vkCommandBuffer, &beginInfo) != VK_SUCCESS)
		{
			Log::Error("Failed to begin command buffer recording: vkBeginCommandBuffer failed!");
			return;
		}

		for (VulkanCommand* pCommand : mCommandList)
		{
			switch (pCommand->type)
			{
				case VULKAN_COMMAND_BEGIN_RENDERPASS:
				{
					VulkanCommandBeginRenderpass* pBeginRenderpass = static_cast<VulkanCommandBeginRenderpass*>(pCommand);

					VulkanRenderpass*	pVulkanRenderpass	= pBeginRenderpass->pRenderpass;
					VulkanFramebuffer*	pVulkanFramebuffer	= pBeginRenderpass->pFramebuffer;

					// @TODO: this will need to be set automatically per attachment
					VkClearValue clearColor = { 0.02f, 0.05f, 0.05f, 1.0f };
					VkClearValue clearDepth = { 1.0f, 0 };

					VkClearValue clearValues[] = { clearColor, clearDepth };

					VkRenderPassBeginInfo renderPassInfo = {};
					renderPassInfo.sType				= VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
					renderPassInfo.renderPass			= pVulkanRenderpass->GetVkRenderPass();
					renderPassInfo.framebuffer			= pVulkanFramebuffer->GetVkFramebuffers()[frameIndex];
					renderPassInfo.renderArea.offset	= { 0, 0 };
					renderPassInfo.renderArea.extent	= { pVulkanFramebuffer->GetWidth(), pVulkanFramebuffer->GetHeight() };
					renderPassInfo.clearValueCount		= 2;
					renderPassInfo.pClearValues			= clearValues;

					vkCmdBeginRenderPass(vkCommandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

					break;
				}

				case VULKAN_COMMAND_END_RENDERPASS:
				{
					vkCmdEndRenderPass(vkCommandBuffer);

					break;
				}

				case VULKAN_COMMAND_SET_GRAPHICS_PIPELINE:
				{
					VulkanCommandSetGraphicsPipeline* pSetGraphicsPipeline = static_cast<VulkanCommandSetGraphicsPipeline*>(pCommand);

					VulkanGraphicsPipeline* pGraphicsPipeline = static_cast<VulkanGraphicsPipeline*>(pSetGraphicsPipeline->pPipeline);
					vkCmdBindPipeline(vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pGraphicsPipeline->GetVkPipeline());
					
					/*
					const Array<VkDescriptorSet>& descriptorSets = pGraphicsPipeline->GetUniformStates()[frameIndex].descriptorSets;
					VkPipelineLayout vkPipelineLayout = pGraphicsPipeline->GetVkPipelineInfo().layout;
					vkCmdBindDescriptorSets(vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
						vkPipelineLayout, 0, descriptorSets.Size(), descriptorSets.Data(), 0, VK_NULL_HANDLE);
					*/

					mState.pGraphicsPipeline = pSetGraphicsPipeline->pPipeline;

					break;
				}

				case VULKAN_COMMAND_SET_COMPUTE_PIPELINE:
				{
					break;
				}

				case VULKAN_COMMAND_SET_VERTEX_BUFFERS:
				{
					VulkanCommandSetVertexBuffers* pSetVertexBuffers = static_cast<VulkanCommandSetVertexBuffers*>(pCommand);

					Array<VkBuffer>		vkBuffers(pSetVertexBuffers->buffers.Size());
					Array<VkDeviceSize> vkOffsetSizes(pSetVertexBuffers->buffers.Size());

					for (UInt32 i = 0; i < pSetVertexBuffers->buffers.Size(); i++)
					{
						vkBuffers[i]		= static_cast<VulkanBuffer*>(pSetVertexBuffers->buffers[i])->GetVkBuffer();
						vkOffsetSizes[i]	= 0;
					}

					vkCmdBindVertexBuffers(vkCommandBuffer, 0, pSetVertexBuffers->buffers.Size(), vkBuffers.Data(), vkOffsetSizes.Data());

					break;
				}

				case VULKAN_COMMAND_SET_INTEX_BUFFER:
				{
					VulkanCommandSetIndexBuffer* pSetIndexBuffer = static_cast<VulkanCommandSetIndexBuffer*>(pCommand);

					VulkanBuffer* pIndexBuffer = static_cast<VulkanBuffer*>(pSetIndexBuffer->pBuffer);

					vkCmdBindIndexBuffer(vkCommandBuffer, pIndexBuffer->GetVkBuffer(), 0, VK_INDEX_TYPE_UINT32);

					break;
				}

				case VULKAN_COMMAND_BIND_UNIFORM_BUFFER:
				{
					VulkanCommandBindUniformBuffer* pBindUniformBuffer = static_cast<VulkanCommandBindUniformBuffer*>(pCommand);

					UniformState&	uniformState	= mState.pGraphicsPipeline->GetUniformState(pBindUniformBuffer->set, frameIndex);
					VulkanUniform*	pUniform		= pBindUniformBuffer->pUniform;
					UInt32			alignedSize		= pUniform->GetAlignedElementSize();
					UInt32			set				= pBindUniformBuffer->set;
					UInt32			binding			= pBindUniformBuffer->binding;

					UInt32* pBindIndex = uniformState.bindIndexMap.Get(binding);

					if (pBindIndex)
					{
						if (uniformState.bufferInfos[*pBindIndex].buffer != pUniform->GetUniformBuffers()[frameIndex]->GetVkBuffer())
						{
							// This is kinda janky as it only updates one per frame (not the whole backbuffer array)
							// Could result in jittery uniforms
							uniformState.SetBuffer(mpDevice, pUniform->GetUniformBuffers()[frameIndex], binding, 0, alignedSize);

							if (!mState.dirtyUniforms.Contains(&uniformState)) // TODO: REPLACE WITH SET ASPAP!
								mState.dirtyUniforms.PushBack(&uniformState);
						}

						UInt32 offset = alignedSize * pBindUniformBuffer->element;
						mState.deferredUniformBinds.PushBack({ &uniformState, offset });
					}
					else
					{
						// TODO: Error
					}

					break;
				}

				case VULKAN_COMMAND_BIND_UNIFORM_TEXTURE_SAMPLER:
				{
					VulkanCommandBindUniformTextureSampler* pBindUniformTextureSampler = static_cast<VulkanCommandBindUniformTextureSampler*>(pCommand);

					UniformState&					uniformState	= mState.pGraphicsPipeline->GetUniformState(pBindUniformTextureSampler->set, frameIndex);
					VulkanUniformTextureSampler*	pUniform		= pBindUniformTextureSampler->pUniformTextureSampler;
					UInt32							set				= pBindUniformTextureSampler->set;
					UInt32							binding			= pBindUniformTextureSampler->binding;

					UInt32* pBindIndex = uniformState.bindIndexMap.Get(binding);

					if (pBindIndex)
					{
						if (uniformState.imageInfos[*pBindIndex].imageView != pUniform->GetVulkanImageView()->GetVkImageView())
						{
							// This is kinda janky as it only updates one per frame (not the whole backbuffer array)
							// Could result in jittery uniforms
							uniformState.SetImageSampler(mpDevice, pUniform->GetVulkanImageView(), binding);

							if (!mState.dirtyUniforms.Contains(&uniformState)) // TODO: REPLACE WITH SET ASPAP!
								mState.dirtyUniforms.PushBack(&uniformState);
						}

						mState.deferredUniformBinds.PushBack({ &uniformState, 0 });
					}
					else
					{
						// TODO: Error
					}

					break;
				}

				case VULKAN_COMMAND_DRAW_INDEXED:
				{
					VulkanCommandDrawIndexed* pDrawIndexed = static_cast<VulkanCommandDrawIndexed*>(pCommand);

					// TODO: I dont really like updating descriptor sets in the draw call, but
					//		 it might be the only logical place to do it.
					UpdateAndBindDescriptorSets(frameIndex);

					vkCmdDrawIndexed(vkCommandBuffer, pDrawIndexed->count, 1, pDrawIndexed->start, 0, 0);

					break;
				}

				default:
				{
					// Invalid command
					break;
				}
			}
		}

		if (vkEndCommandBuffer(vkCommandBuffer) != VK_SUCCESS)
		{
			Log::Error("Failed to end command buffer recording: vkBeginCommandBuffer failed!");
		}
	}

	void VulkanCommandBuffer::UpdateAndBindDescriptorSets(UInt32 frameIndex)
	{
		VkCommandBuffer vkCommandBuffer = mCommandBuffers[frameIndex];

		for (UniformState* pUniformState : mState.dirtyUniforms)
		{
			pUniformState->UpdateDescriptorSets(mpDevice);
		}

		for (DeferredUniformBind& bind : mState.deferredUniformBinds)
		{
			vkCmdBindDescriptorSets(vkCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
				mState.pGraphicsPipeline->GetVkPipelineInfo().layout,
				bind.pUniformState->set, 1, &bind.pUniformState->descriptorSet, 1, &bind.dynamicOffset);
		}

		mState.dirtyUniforms.Clear();
		mState.deferredUniformBinds.Clear();
	}
}