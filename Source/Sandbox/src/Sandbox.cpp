
#include "Common.h"

#include "util/Array.h"
#include "util/String.h"

#include "math/Math.h"

#include "Win32WindowManager.h"
#include "Win32Vulkan.h"

#include "object/OBJLoader.h"

#include <iostream>
#include <fstream>

#include "log/Log.h"

_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;

#define MODEL_PATH "models/testScene.obj"
#define DIFFUSE_PATH "textures/default.png"

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

#define DEFAULT_WIDTH 1280
#define DEFAULT_HEIGHT 720

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
	Window& window = manager2.CreateWindow(StringW(L"Quartz Engine"), 100, 100, DEFAULT_WIDTH, DEFAULT_HEIGHT, false, true);
	window.Show();

	HGFXSurface surface = pVulkanContext->CreateSurface(window, window.ClientWidth(), window.ClientWidth(), false, false);
	HGFXSwapchain swapchain = pVulkanContext->CreateSwapchain(surface, window, 3, window.ClientWidth(), window.ClientWidth(), false, false);

	GFXRenderAttachment colorAttachment;
	colorAttachment.format = GFX_IMAGE_FORMAT_BGRA8_UNORM;
	colorAttachment.initalLayout = GFX_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout = GFX_IMAGE_LAYOUT_PRESENT;
	colorAttachment.stencilLoadOp = GFX_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp = GFX_STORE_OP_DONT_CARE;
	colorAttachment.loadOp = GFX_LOAD_OP_CLEAR;
	colorAttachment.storeOp = GFX_STORE_OP_STORE;

	GFXRenderAttachment depthAttachment;
	depthAttachment.format = GFX_IMAGE_FORMAT_DEPTH24_UNORM_STENCIL8_UINT;
	depthAttachment.initalLayout = GFX_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = GFX_IMAGE_LAYOUT_DEPTH_STENCIL_OUTPUT;
	depthAttachment.stencilLoadOp = GFX_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp = GFX_STORE_OP_DONT_CARE;
	depthAttachment.loadOp = GFX_LOAD_OP_CLEAR;
	depthAttachment.storeOp = GFX_STORE_OP_DONT_CARE;

	GFXRenderPassInfo renderPassInfo =
	{
		// Subpasses
		{
			{
				"Color Subpass",
				{
					{ &colorAttachment, GFX_IMAGE_LAYOUT_COLOR_OUTPUT },
					{ &depthAttachment, GFX_IMAGE_LAYOUT_DEPTH_STENCIL_OUTPUT }
				}
			}
		},

		// Dependancies
		{
			{
				// This is the dependancy
				GFX_SUBPASS_EXTERNAL,
				GFX_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | GFX_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
				0, //GFX_ACCESS_COLOR_ATTACHMENT_READ_BIT | GFX_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT,

				// For this subpass stage
				GFX_SUBPASS_INDEX_0,
				GFX_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | GFX_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
				GFX_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | GFX_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
			}
		}
	};

	HGFXRenderPass renderPass = pVulkanContext->CreateRenderPass(renderPassInfo);

	GFXGraphicsPipelineInfo pipelineInfo;
	{
		Bounds2f renderBounds(0, 0, window.ClientWidth(), window.ClientHeight());

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

		GFXBlendAttachment colorOutputBlendAttachment;
		colorOutputBlendAttachment.blend = GFXColorBlend();
		pipelineInfo.blendAttachments.PushBack(colorOutputBlendAttachment);
	}

	HGFXGraphicsPipeline pipeline = pVulkanContext->CreateGraphicsPipeline(pipelineInfo, renderPass, 0);

	HGFXImage depthStencilImage = pVulkanContext->CreateImage(GFX_IMAGE_TYPE_2D, 
		GFX_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, 
		GFX_IMAGE_FORMAT_DEPTH24_UNORM_STENCIL8_UINT, DEFAULT_WIDTH, DEFAULT_HEIGHT, 1, 1, 1);

	HGFXImageView depthStencilView = pVulkanContext->CreateImageView(depthStencilImage, 
		GFX_IMAGE_VIEW_TYPE_2D, GFX_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT, 0, 1, 0, 1);


	Array<Byte> modelFileData = ReadFile(MODEL_PATH);
	String modelDataString = String((char*)modelFileData.Data(), modelFileData.Size());
	Model mModel = LoadOBJ(modelDataString);

	struct UBO
	{
		Matrix4 model;
		Matrix4 view;
		Matrix4 proj;
	}
	ubo{};

	ubo.model.SetTranslation({ 0.0f, 0.0f, 0.0f });
	ubo.view.SetTranslation({ 0.0f, 0.0f, 5.0f });
	ubo.proj.SetPerspective(90.0f, DEFAULT_WIDTH / DEFAULT_HEIGHT, 0.0001f, 1000.0f);

	HGFXBuffer vertexBuffer = pVulkanContext->CreateBuffer(GFX_BUFFER_USAGE_VERTEX_BUFFER_BIT, 
		GFX_BUFFER_ACCESS_HOST_VISIBLE_BIT | GFX_BUFFER_ACCESS_HOST_COHERENT_BIT, mModel.vertexData.buffer.Size());
	void* pVertexData = pVulkanContext->MapBuffer(vertexBuffer);
	memcpy(pVertexData, mModel.vertexData.buffer.Data(), mModel.vertexData.buffer.Size());
	pVulkanContext->UnmapBuffer(vertexBuffer);

	HGFXBuffer indexBuffer = pVulkanContext->CreateBuffer(GFX_BUFFER_USAGE_INDEX_BUFFER_BIT,
		GFX_BUFFER_ACCESS_HOST_VISIBLE_BIT | GFX_BUFFER_ACCESS_HOST_COHERENT_BIT, mModel.indexData.buffer.Size());
	void* pIndexData = pVulkanContext->MapBuffer(indexBuffer);
	memcpy(pIndexData, mModel.indexData.buffer.Data(), mModel.indexData.buffer.Size());
	pVulkanContext->UnmapBuffer(indexBuffer);

	constexpr UInt32 BUFFER_COUNT = 3;

	HGFXCommandBuffer commandBuffers[BUFFER_COUNT];
	HGFXFramebuffer frameBuffers[BUFFER_COUNT];
	HGFXBuffer uniformBuffers[BUFFER_COUNT];

	for (UInt32 i = 0; i < BUFFER_COUNT; i++)
	{
		uniformBuffers[i] = pVulkanContext->CreateBuffer(GFX_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			GFX_BUFFER_ACCESS_HOST_VISIBLE_BIT | GFX_BUFFER_ACCESS_HOST_COHERENT_BIT, sizeof(UBO));
		void* pUniformData = pVulkanContext->MapBuffer(uniformBuffers[i]);
		memcpy(pUniformData, &ubo, sizeof(UBO));
		pVulkanContext->UnmapBuffer(uniformBuffers[i]);

		commandBuffers[i] = pVulkanContext->CreateCommandBuffer(GFX_COMMAND_BUFFER_USAGE_GRAPHICS_BIT);
		
		HGFXImageView swapChainImageView = pVulkanContext->GetSwapchainImageView(swapchain, i);

		Array<HGFXImageView> frameBufferImages =
		{
			swapChainImageView,
			depthStencilView
		};

		frameBuffers[i] = pVulkanContext->CreateFramebuffer(renderPass, frameBufferImages, DEFAULT_WIDTH, DEFAULT_HEIGHT);
	
		// I dont like this
		pVulkanContext->SetUniformBuffer(pipeline, 0, 0, uniformBuffers[i], i);

		pVulkanContext->BeginCommandBuffer(commandBuffers[i]);
		pVulkanContext->BeginRenderPass(commandBuffers[i], renderPass, frameBuffers[i]);

		pVulkanContext->BindGraphicsPipeline(commandBuffers[i], pipeline, i);
		pVulkanContext->BindVertexBuffers(commandBuffers[i], { vertexBuffer });
		pVulkanContext->BindIndexBuffer(commandBuffers[i], indexBuffer);

		pVulkanContext->DrawIndexed(commandBuffers[i], mModel.indexData.buffer.Size() / 4, 0);

		pVulkanContext->EndRenderPass(commandBuffers[i]);
		pVulkanContext->EndCommandBuffer(commandBuffers[i]);
	}

	while (true)
	{
		UInt32 frameIndex = pVulkanContext->AcquireSwapchainImageIndex(swapchain);

		void* pUniformData = pVulkanContext->MapBuffer(uniformBuffers[frameIndex]);
		memcpy(pUniformData, &ubo, sizeof(UBO));
		pVulkanContext->UnmapBuffer(uniformBuffers[frameIndex]);

		pVulkanContext->Submit(commandBuffers[frameIndex], swapchain);
		pVulkanContext->Present(swapchain);

		manager2.Update();
	}

	return 0;
}