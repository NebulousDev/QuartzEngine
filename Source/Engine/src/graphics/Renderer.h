#pragma once

#include "../entity/World.h"
#include "Components.h"

#include "../object/Lights.h"

namespace Quartz
{
#define MAX_RENDER_BUFFER_SIZE		3
#define MAX_RENDER_PASS_COUNT		8
#define MAX_FRAMEBUFFER_IMAGE_COUNT	16

	struct RenderBuffer
	{
		HGFXCommandBuffer commandBuffers[MAX_RENDER_BUFFER_SIZE];
		HGFXBuffer mvpUniformBuffers[MAX_RENDER_BUFFER_SIZE];
		HGFXBuffer lightsUniformBuffers[MAX_RENDER_BUFFER_SIZE];
		HGFXRenderPass renderPasses[MAX_RENDER_PASS_COUNT];
		HGFXFramebuffer frameBuffers[MAX_RENDER_PASS_COUNT][MAX_RENDER_BUFFER_SIZE];
		HGFXImage framebufferImages[MAX_FRAMEBUFFER_IMAGE_COUNT];
		HGFXImageView framebufferImageViews[MAX_FRAMEBUFFER_IMAGE_COUNT];
		HGFXSwapchain swapchain;
		UInt32 frameBufferCount;
		UInt32 bufferCount;
	};

	struct MVP
	{
		Matrix4 model;
		Matrix4 view;
		Matrix4 proj;
	};

	struct Lights
	{
		Vector4 cameraPosition;
		DirectionalLight directional[4];
		PointLight point[4];
		UInt32 directionalCount;
		UInt32 pointCount;
		UInt32 buffer[2];
	};

	struct RenderPassInfo
	{
		UInt32 width;
		UInt32 height;
		GFXRenderPassInfo renderPassInfo;
	};

	struct FrameInfo
	{
		UInt32 bufferCount;
		HGFXSwapchain swapchain;
		Array<RenderPassInfo> renderPasses;
	};

	class QUARTZ_API ClassicRenderer
	{
	private:
		VGFXContext* pGfx;
		RenderBuffer buffers;
		HGFXRenderPass renderPass;

		void BuildFramebufferImages(FrameInfo& info);

	public:
		void SetupRenderer(VGFXContext* pGfx, FrameInfo& info);
		void BuildPipelines(EntityWorld& world);
		void BuildCommandBuffers(EntityWorld& world);
		void RenderScene(EntityWorld& world);
	};
}