#pragma once

#include "graphics2/CommandBuffer.h"

#include "VulkanRenderpass.h"
#include "VulkanFramebuffer.h"
#include "VulkanDevice.h"
#include "VulkanPipeline.h"

#include "util/Map.h"

#include <vulkan/vulkan.h>

namespace Quartz
{
	class VulkanCommandBuffer : public CommandBuffer
	{
	public:
		enum CommandBufferUsageBits
		{
			COMMAND_BUFFER_USAGE_GRAPHICS_BIT	= 0x1,
			COMMAND_BUFFER_USAGE_COMPUTE_BIT	= 0x2,
			COMMAND_BUFFER_USAGE_TRANSFER_BIT	= 0x4
		};

		typedef Flags32 CommandBufferUsages;

		enum VulkanCommandType
		{
			VULKAN_COMMAND_INVALID,
			VULKAN_COMMAND_BEGIN_RECORDING,
			VULKAN_COMMAND_END_RECORDING,
			VULKAN_COMMAND_BEGIN_RENDERPASS,
			VULKAN_COMMAND_END_RENDERPASS,
			VULKAN_COMMAND_SET_GRAPHICS_PIPELINE,
			VULKAN_COMMAND_SET_COMPUTE_PIPELINE,
			VULKAN_COMMAND_SET_VERTEX_BUFFERS,
			VULKAN_COMMAND_SET_INTEX_BUFFER,

			VULKAN_COMMAND_BIND_UNIFORM_BUFFER,

			VULKAN_COMMAND_DRAW_INDEXED
		};

		struct VulkanCommand
		{
			VulkanCommandType type;
		};

		template <VulkanCommandType Type>
		struct VulkanCommandBase : public VulkanCommand
		{
			VulkanCommandBase()
			{
				VulkanCommand::type = Type;
			};
		};

		struct VulkanCommandBeginRecording 
			: public VulkanCommandBase<VULKAN_COMMAND_BEGIN_RECORDING> { };

		struct VulkanCommandEndRecording 
			: public VulkanCommandBase<VULKAN_COMMAND_END_RECORDING> { };

		struct VulkanCommandBeginRenderpass 
			: public VulkanCommandBase<VULKAN_COMMAND_BEGIN_RENDERPASS>
		{
			VulkanRenderpass* pRenderpass;
			VulkanFramebuffer* pFramebuffer;
		};

		struct VulkanCommandEndRenderpass 
			: public VulkanCommandBase<VULKAN_COMMAND_END_RENDERPASS> {};

		struct VulkanCommandSetGraphicsPipeline 
			: public VulkanCommandBase<VULKAN_COMMAND_SET_GRAPHICS_PIPELINE>
		{
			VulkanGraphicsPipeline* pPipeline;
		};

		struct VulkanCommandSetVertexBuffers 
			: public VulkanCommandBase<VULKAN_COMMAND_SET_VERTEX_BUFFERS>
		{
			Array<VulkanBuffer*> buffers;
		};

		struct VulkanCommandSetIndexBuffer 
			: public VulkanCommandBase<VULKAN_COMMAND_SET_INTEX_BUFFER>
		{
			VulkanBuffer* pBuffer;
		};

		struct VulkanCommandBindUniformBuffer 
			: public VulkanCommandBase<VULKAN_COMMAND_BIND_UNIFORM_BUFFER>
		{
			UInt32			set;
			UInt32			binding;
			VulkanBuffer*	pBuffer;
		};

		struct VulkanCommandDrawIndexed
			: public VulkanCommandBase<VULKAN_COMMAND_DRAW_INDEXED>
		{
			UInt32 count;
			UInt32 start;
		};

		struct VulkanCommandBufferState
		{
			VulkanGraphicsPipeline* pGraphicsPipeline;
		};

	private:
		VulkanDevice*				mpDevice;
		Array<VulkanCommand*>		mCommandList;
		VkCommandPool				mvkCommandPool;
		Array<VkCommandBuffer>		mCommandBuffers;
		CommandBufferUsages			mUsages;
		Bool8						mIsDynamic;
		Bool8						mBuilt;

		VulkanCommandBufferState	mState;

	public:
		VulkanCommandBuffer(VulkanDevice* pDevice);

		void BeginRecording() override;
		void EndRecording() override;

		void BeginRenderpass(Renderpass* pRenderpass, Framebuffer* pFramebuffer) override;
		void EndRenderpass() override;

		void SetPipeline(Pipeline* pPipeline) override;

		void SetVertexBuffers(const Array<Buffer*>& buffers) override;
		void SetIndexBuffer(Buffer* pBuffer) override;

		void BindUniform(UInt32 set, UInt32 binding, Buffer* pBuffer) override;

		void DrawIndexed(UInt32 count, UInt32 start) override;

		void Build(UInt32 bufferCount);

		void Reset();

		FORCE_INLINE Array<VkCommandBuffer>&	GetCommandbuffers() { return mCommandBuffers; }
		FORCE_INLINE UInt32						GetCommandBufferCount() { return mCommandBuffers.Size(); }
		FORCE_INLINE VkCommandPool				GetVkCommandPool() { return mvkCommandPool; }
		FORCE_INLINE Bool8						IsBuilt() const { return mBuilt; }
	};
}