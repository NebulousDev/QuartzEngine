#include "Renderer.h"

#include "../Engine.h"

namespace Quartz
{

	GFXImageUsages FindAllUsages()
	{

	}

	void ClassicRenderer::BuildFramebufferImages(FrameInfo& info)
	{
		//HGFXImageView swapChainImageView = pVulkanContext->GetSwapchainImageView(pWindow->GetSwapchain(), i);

		for (UInt32 i = 0; info.renderPasses.Size(); i++)
		{
			GFXRenderPassInfo& passInfo = info.renderPasses[i].renderPassInfo;

			for (UInt32 j = 0; j < passInfo.subpasses.Size(); j++)
			{
				GFXRenderSubpassInfo& subpassInfo = passInfo.subpasses[j];
				
				for (UInt32 k = 0; k < subpassInfo.attachments.Size(); k++)
				{
					GFXRenderAttachment* pAttachment = subpassInfo.attachments[k].pAttachment;

					GFXImageUsages usages;

					switch (subpassInfo.attachments[k].layout)
					{
						case GFX_IMAGE_LAYOUT_COLOR_INPUT:
						case GFX_IMAGE_LAYOUT_COLOR_OUTPUT:
						{
							usages = GFX_IMAGE_USAGE_COLOR_ATTACHMENT;
							break;
						}

						case GFX_IMAGE_LAYOUT_DEPTH_INPUT:
						case GFX_IMAGE_LAYOUT_DEPTH_OUTPUT:
						{
							usages = GFX_IMAGE_USAGE_DEPTH_ATTACHMENT;
							break;
						}

						case GFX_IMAGE_LAYOUT_DEPTH_STENCIL_INPUT:
						case GFX_IMAGE_LAYOUT_DEPTH_STENCIL_OUTPUT:
						{
							usages = GFX_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT;
							break;
						}

						case GFX_IMAGE_LAYOUT_STENCIL_INPUT:
						case GFX_IMAGE_LAYOUT_STENCIL_OUTPUT:
						{
							usages = GFX_IMAGE_USAGE_STENCIL_ATTACHMENT;
							break;
						}

						default:
						{
							usages = GFX_IMAGE_USAGE_INVALID;
							// @TODO: Error
						}
					}

					HGFXImage image = pGfx->CreateImage(GFX_IMAGE_TYPE_2D, usages, 
						pAttachment->format, info.renderPasses[i].width, info.renderPasses[i].height, 1, 1, 1);
				}
			}
		}
	}

	void ClassicRenderer::SetupRenderer(VGFXContext* pGfx, FrameInfo& info)
	{
		this->pGfx = pGfx;
		buffers.bufferCount = info.bufferCount;
		buffers.swapchain = info.swapchain;

		BuildFramebufferImages(info);

		if (info.bufferCount > MAX_RENDER_BUFFER_SIZE)
		{
			// error
		}

		if (info.renderPasses.Size() > MAX_RENDER_PASS_COUNT)
		{
			// error
		}

		for (UInt32 i = 0; info.renderPasses.Size(); i++)
		{
			buffers.renderPasses[i] = pGfx->CreateRenderPass(info.renderPasses[i].renderPassInfo);
		}

		for (UInt32 i = 0; i < buffers.bufferCount; i++)
		{
			buffers.mvpUniformBuffers[i] = pGfx->CreateBuffer(GFX_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				GFX_BUFFER_ACCESS_HOST_VISIBLE_BIT | GFX_BUFFER_ACCESS_HOST_COHERENT_BIT, sizeof(MVP));

			buffers.lightsUniformBuffers[i] = pGfx->CreateBuffer(GFX_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				GFX_BUFFER_ACCESS_HOST_VISIBLE_BIT | GFX_BUFFER_ACCESS_HOST_COHERENT_BIT, sizeof(Lights));

			buffers.commandBuffers[i] = pGfx->CreateCommandBuffer(GFX_COMMAND_BUFFER_USAGE_GRAPHICS_BIT);

			for (UInt32 j = 0; info.renderPasses.Size(); j++)
			{
				//buffers.frameBuffers[j][i] = pGfx->CreateFramebuffer(renderPass, , info.renderPasses[j].width, info.renderPasses[j].height);
			}
		}
	}

	void ClassicRenderer::BuildPipelines(EntityWorld& world)
	{

	}

	void ClassicRenderer::BuildCommandBuffers(EntityWorld& world)
	{
		VGFXContext& gfx = Engine::GetInstance().GetGraphics();

		for (Entity e : world.CreateView<MeshComponent, TransformComponent>())
		{
			TransformComponent& transform = world.GetComponent<TransformComponent>(e);
			MeshComponent& mesh = world.GetComponent<MeshComponent>(e);

		}
	}

	void ClassicRenderer::RenderScene(EntityWorld& world)
	{
		
	}
}

