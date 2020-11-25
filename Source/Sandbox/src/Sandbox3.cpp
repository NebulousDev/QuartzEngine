
#include "Common.h"

#include "util/Array.h"
#include "util/String.h"

#include "Win32WindowManager.h"
#include "Win32Vulkan.h"

#include <iostream>
#include <fstream>

#include "log/Log.h"

namespace Quartz
{
	static Array<Byte> ReadFile(const String& filename)
	{
		std::ifstream file(filename.Str(), std::ios::ate | std::ios::binary);

		if (!file.is_open())
		{
			Log.Error("Cannot open file %s", filename.Str());
			throw std::runtime_error("failed to open file!");
		}

		size_t fileSize = (size_t)file.tellg();
		Array<Byte> buffer(fileSize);

		file.seekg(0);
		file.read((char*)buffer.Data(), fileSize);

		file.close();

		return buffer;
	}
}

int main()
{
	using namespace Quartz;

	Array<String> extensions;
	extensions.PushBack(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	extensions.PushBack(VK_KHR_SURFACE_EXTENSION_NAME);
	extensions.PushBack("VK_KHR_win32_surface");

	Array<String> validationLayers;
	validationLayers.PushBack("VK_LAYER_KHRONOS_validation");

	Win32VulkanContext* pVulkanContext = new Win32VulkanContext();
	pVulkanContext->InitInstanceAndDevices("Sandbox", extensions, validationLayers);

	Win32WindowManager manager2;
	Window& window = manager2.CreateWindow(StringW(L"Quartz Engine"), 100, 100, 1280, 720, false, true);
	window.Show();

	HGFXSurface surface = pVulkanContext->CreateSurface(window, window.Width(), window.Height(), false, false);
	HGFXSwapchain swapchain = pVulkanContext->CreateSwapchain(surface, window, 3, window.Width(), window.Height(), false, false);


	GFXRenderAttachment colorAttachment;
	GFXRenderAttachment albetoAttachment;
	GFXRenderAttachment normalAttachment;
	GFXRenderAttachment positionAttachment;
	GFXRenderAttachment depthAttachment;

	Array<int> temp = { 1, 2 ,3 };

	GFXRenderPassInfo renderPassInfo =
	{
		// Subpasses
		{
			{
				"G-Buffer Subpass",
				{
					{ &albetoAttachment, GFX_IMAGE_LAYOUT_COLOR_OUTPUT },
					{ &normalAttachment, GFX_IMAGE_LAYOUT_COLOR_OUTPUT },
					{ &positionAttachment, GFX_IMAGE_LAYOUT_COLOR_OUTPUT },
					{ &depthAttachment, GFX_IMAGE_LAYOUT_DEPTH_STENCIL_OUTPUT }	
				},
			},

			{
				"Color Subpass",
				{
					{ &albetoAttachment, GFX_IMAGE_LAYOUT_COLOR_INPUT },
					{ &normalAttachment, GFX_IMAGE_LAYOUT_COLOR_INPUT },
					{ &positionAttachment, GFX_IMAGE_LAYOUT_COLOR_INPUT },
					{ &depthAttachment, GFX_IMAGE_LAYOUT_DEPTH_STENCIL_INPUT },
					{ &colorAttachment, GFX_IMAGE_LAYOUT_COLOR_OUTPUT }
				}
			}
		},

		// Dependancies
		{
			{
				// This is the dependancy
				GFX_SUBPASS_EXTERNAL,

				// For this subpass stage
				GFX_SUBPASS_INDEX_0,

				// When this pipeline stage is complete V
				GFX_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,

				// This pipeline stage may start
				GFX_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
			},

			{
				GFX_SUBPASS_INDEX_0,
				GFX_SUBPASS_INDEX_1,
				GFX_PIPELINE_STAGE_PIXEL_SHADER_BIT,
				GFX_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
			}
		}

	};

	HGFXCommandBuffer buffers[3];
	HGFXGraphicsPipeline pipelines[3];

	GFXGraphicsPipelineInfo pipelineInfo;

	{
		Bounds2f renderBounds(0, 0, window.Width(), window.Height());

		pipelineInfo.vertexShader = pVulkanContext->CreateShader(GFX_SHADER_STAGE_VERTEX, ReadFile("shaders/vert.spv"), "main");
		pipelineInfo.pixelShader = pVulkanContext->CreateShader(GFX_SHADER_STAGE_PIXEL, ReadFile("shaders/frag.spv"), "main");
		
		pipelineInfo.viewport.bounds = renderBounds;
		pipelineInfo.viewport.minDepth = 0.0f;
		pipelineInfo.viewport.maxDepth = 1.0f;
		
		pipelineInfo.scissor.bounds = renderBounds;
		
		pipelineInfo.topology = GFX_PRIMITIVE_TOPOLOGY_TRIANGLES;
		pipelineInfo.polygonMode = GFX_POLYGON_MODE_FILL;
		pipelineInfo.cullMode = GFX_CULL_MODE_NONE;
		pipelineInfo.faceWind = GFX_FACE_WIND_COUNTER_CLOCKWISE;
		pipelineInfo.lineWidth = 1.0f;
		
		pipelineInfo.multisample = GFX_MULTISAMPLE_DISABLED;
		
		pipelineInfo.depthTesting = true;
		pipelineInfo.depthOperation = GFX_COMPARE_OP_LESS_OR_EQUAL;

		GFXBufferAttachent vertexBufferAttachment;
		vertexBufferAttachment.binding = 0;
		vertexBufferAttachment.stride = 8 * sizeof(Float32);
		pipelineInfo.bufferAttachemnts.PushBack(vertexBufferAttachment);

		GFXVertexAttribute positionAttrib;
		positionAttrib.binding = 0;
		positionAttrib.location = 0;
		positionAttrib.type = GFX_ATTRIBUTE_TYPE_FLOAT3;
		pipelineInfo.vertexAttributes.PushBack(positionAttrib);

		GFXVertexAttribute normalAttrib;
		normalAttrib.binding = 0;
		normalAttrib.location = 1;
		normalAttrib.type = GFX_ATTRIBUTE_TYPE_FLOAT3;
		pipelineInfo.vertexAttributes.PushBack(normalAttrib);

		GFXVertexAttribute texCoordAttrib;
		texCoordAttrib.binding = 0;
		texCoordAttrib.location = 2;
		texCoordAttrib.type = GFX_ATTRIBUTE_TYPE_FLOAT2;
		pipelineInfo.vertexAttributes.PushBack(texCoordAttrib);

		GFXDescriptorAttachment uniformAttachment;
		uniformAttachment.set = 0;
		uniformAttachment.binding = 0;
		uniformAttachment.arraySize = 1;
		uniformAttachment.type = GFX_DESCRIPTOR_TYPE_UNIFORM;
		uniformAttachment.stages = GFX_SHADER_STAGE_VERTEX_BIT;

		GFXDescriptorAttachment diffuseTextureAttachment;
		diffuseTextureAttachment.set = 0;
		diffuseTextureAttachment.binding = 1;
		diffuseTextureAttachment.arraySize = 1;
		diffuseTextureAttachment.type = GFX_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		diffuseTextureAttachment.stages = GFX_SHADER_STAGE_PIXEL_BIT;

		pipelineInfo.descriptorAttachents.PushBack(uniformAttachment);
		pipelineInfo.descriptorAttachents.PushBack(diffuseTextureAttachment);

		GFXBlendAttachment colorOutputBlendAttachment;
		colorOutputBlendAttachment.blend = GFXColorBlend();
		pipelineInfo.blendAttachments.PushBack(colorOutputBlendAttachment);
	}

	for (UInt32 i = 0; i < 3; i++)
	{
		buffers[i] = pVulkanContext->CreateCommandBuffer(GFX_COMMAND_BUFFER_USAGE_GRAPHICS_BIT);
		pipelines[i] = pVulkanContext->CreateGraphicsPipeline(pipelineInfo);
	}

	while (true)
	{

	}

	return 0;
}