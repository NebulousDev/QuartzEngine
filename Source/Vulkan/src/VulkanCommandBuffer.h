#pragma once

#include "graphics/CommandBuffer.h"

#include "VulkanRenderpass.h"
#include "VulkanFramebuffer.h"
#include "VulkanDevice.h"
#include "VulkanPipeline.h"

#include "util/Map.h"
#include "util/Set.h"

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
			VULKAN_COMMAND_BEGIN_RENDERPASS,
			VULKAN_COMMAND_END_RENDERPASS,
			VULKAN_COMMAND_SET_GRAPHICS_PIPELINE,
			VULKAN_COMMAND_SET_COMPUTE_PIPELINE,
			VULKAN_COMMAND_SET_VERTEX_BUFFERS,
			VULKAN_COMMAND_SET_INTEX_BUFFER,

			VULKAN_COMMAND_BIND_UNIFORM_BUFFER,
			VULKAN_COMMAND_BIND_UNIFORM_TEXTURE_SAMPLER,

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
			VulkanUniform*	pUniform;
			UInt32			element;
		};

		struct VulkanCommandBindUniformTextureSampler
			: public VulkanCommandBase<VULKAN_COMMAND_BIND_UNIFORM_TEXTURE_SAMPLER>
		{
			UInt32							set;
			UInt32							binding;
			VulkanUniformTextureSampler		uniformTextureSampler; // TODO: should maybe not stay a copy?
		};

		struct VulkanCommandDrawIndexed
			: public VulkanCommandBase<VULKAN_COMMAND_DRAW_INDEXED>
		{
			UInt32 count;
			UInt32 start;
		};

		struct DeferredUniformBind
		{
			UniformState*	pUniformState;
			UInt32			dynamicOffset;
		};

		struct VulkanCommandBufferState
		{
			VulkanGraphicsPipeline*		pGraphicsPipeline;
			Array<UniformState*>		dirtyUniforms;
			Array<DeferredUniformBind>	deferredUniformBinds;
		};

	private:
		VulkanDevice*				mpDevice;
		Array<VulkanCommand*>		mCommandList;
		VkCommandPool				mvkCommandPool;
		Array<VkCommandBuffer>		mCommandBuffers;
		CommandBufferUsages			mUsages;
		Bool8						mBuilt;

		VulkanCommandBufferState	mState;

	private:
		void UpdateAndBindDescriptorSets(UInt32 frameIndex);

	public:
		VulkanCommandBuffer(VulkanDevice* pDevice, CommandBufferType type);

		void BeginRecording() override;
		void EndRecording() override;

		void BeginRenderpass(Renderpass* pRenderpass, Framebuffer* pFramebuffer) override;
		void EndRenderpass() override;

		void SetPipeline(Pipeline* pPipeline) override;

		void SetVertexBuffers(const Array<Buffer*>& buffers) override;
		void SetIndexBuffer(Buffer* pBuffer) override;

		void BindUniform(UInt32 set, UInt32 binding, Uniform* pUniform, UInt32 element) override;
		void BindUniformTexture(UInt32 set, UInt32 binding, UniformTextureSampler* pUniformTextureSampler) override;

		void DrawIndexed(UInt32 count, UInt32 start) override;

		void BuildBuffers(UInt32 bufferCount);

		void RecordStatic();
		void RecordDynamic(UInt32 frameIndex);

		FORCE_INLINE Array<VkCommandBuffer>&	GetCommandbuffers() { return mCommandBuffers; }
		FORCE_INLINE UInt32						GetCommandBufferCount() { return mCommandBuffers.Size(); }
		FORCE_INLINE VkCommandPool				GetVkCommandPool() { return mvkCommandPool; }
		FORCE_INLINE Bool8						IsDynamic() const { return mType == COMMAND_BUFFER_DYNAMIC; }
		FORCE_INLINE Bool8						IsBuilt() const { return mBuilt; }
	};
}