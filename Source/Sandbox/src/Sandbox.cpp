#pragma once

#include "Common.h"
#include "util\Array.h"
#include "util\String.h"

#include "math\Math.h"

#include "io\Log.h"
#include "platform\Window.h"
#include "platform\System.h"

#include "Win32Platform.h"

#include "io\Win32Console.h"
#include "platform\Win32Window.h"
#include "platform\Win32System.h"

#include "Engine.h"

#include "json\JSON.h"

#include "gfx\GFXContext.h"
#include "Vulkan.h"

#include "VulkanWin32.h"
#include "VulkanUtil.h"

#include "input\Input.h"
#include "input\Win32Input.h"

#include <vector>
#include <iostream>
#include <string>
#include <fstream>

#include "event\EventSystem.h"

#include "EventTesting.h"

using namespace Quartz;

static std::vector<char> readFile(const std::string& filename)
{
	std::ifstream file(filename, std::ios::ate | std::ios::binary);

	if (!file.is_open())
	{
		Log.Error("Cannot open file %s", filename.c_str());
		throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();
	std::vector<char> buffer(fileSize);

	file.seekg(0);
	file.read(buffer.data(), fileSize);

	file.close();

	return buffer;
}

int main2()
{
	/* Initialize System */

	PlatformSystem::Init(new Win32System());
	Log.Info("System Initialized Successfully.");

	Platform* pPlatform = new Win32Platform();
	pPlatform->Initialize();

	PlatformInput& input = pPlatform->GetPlatformInput();

	/* Initialize Console */

	SystemConsole* pConsole = PlatformSystem::CreateConsole();
	pConsole->Init();
	Log.SetOutputConsole(pConsole);
	Log.Info("Log Initialized Successfully.");

	/* Initialize Input */
	PlatformInput* pInput = &pPlatform->GetPlatformInput();
	//pInput->InitializeInput();
	//pInput->PollDeviceConnections();
	Log.Info("Input Initialized Successfully.");

	InputDevice xBoxController;
	pInput->CreateDevice(pInput->GetConnectedDeviceList()[0], 0, &xBoxController);

	//InputDevice keyboard;
	//pInput->CreateDevice(pInput->GetConnectedDeviceList()[12], 0, &keyboard);

	//InputDevice mouse;
	//pInput->CreateDevice(pInput->GetConnectedDeviceList()[16], 0, &mouse);

	TestEvents();

	/////////////////////////////////////////////////////////////////////////////////////

	/*
	JSONValue& json = *ParseJSON("   {  \"num\": 12,\n\"text\" : \"\\\"Hello World\\\"\" } ");

	Log.Info("num : %llf", json["num"].AsNumber());
	Log.Info("text : %s", json["text"].AsString().Str());

	//system("PAUSE");
	*/

	/////////////////////////////////////////////////////////////////////////////////////


		// TODO IDEAS:
		// COMBINE ALL GRAPHICS ALLOCATIONS INTO ONE CONTIGUOUS ARRAY
		// DEVICES[], SWAPCHAINS[], PIPELINES[] etc...



	/////////////////////////////////////////////////////////////////////////////////////

	/* Create Window */

	Window* pWindow = PlatformSystem::CreateWindow(0, 0, 1280, 720, "Quartz Engine - Sandbox");
	pWindow->Show();
	Log.Info("Window Created Successfully.");

	/* Create Graphics Context */

	VulkanGFXModule vkGFXmodule = {};
	VulkanWin32GFXModule vkWin32GFXModule = {};

	GFXContext ctx = {};
	ctx.SetGFXModule(&vkGFXmodule);
	ctx.SetGFXPlatformModule(&vkWin32GFXModule);

	ctx.InitGraphicsContext();

	GFXPhysicalDeviceList deviceList = ctx.GetPhysicalDeviceList();
	for (GFXPhysicalDevice* pDevice : deviceList)
	{
		Log.Info("Found Graphics Device: \n  NAME:%s\n  VENDOR:%s\n  DRIVER:%d",
			pDevice->GetDeviceName(), pDevice->GetVendorName(), pDevice->GetDriverVersion());
	}

	GFXPhysicalDevice* pPhysicalDevice = ctx.GetPrimaryPhysicalDevice();
	GFXQueueFamily* pFamily = pPhysicalDevice->GetQueueFamilies()[0];

	GFXQueueInfo graphicsQueueInfo	= {};
	graphicsQueueInfo.debugName		= "Graphics Queue";
	graphicsQueueInfo.queueFlags	= GFX_QUEUE_TYPE_GRAPHICS_BIT;
	graphicsQueueInfo.familyIndex	= pFamily->GetFamilyIndex();
	graphicsQueueInfo.queueIndex	= 0;
	graphicsQueueInfo.priority		= 1.0f;

	GFXQueueInfo presentQueueInfo	= {};
	presentQueueInfo.debugName		= "Present Queue";
	presentQueueInfo.queueFlags		= GFX_QUEUE_TYPE_GRAPHICS_BIT;
	presentQueueInfo.familyIndex	= pFamily->GetFamilyIndex();
	presentQueueInfo.queueIndex		= 1;
	presentQueueInfo.priority		= 1.0f;

	GFXQueueInfoList queueList = { graphicsQueueInfo, presentQueueInfo };

	GFXDeviceInfo deviceInfo = {};
	deviceInfo.debugName = pPhysicalDevice->GetDeviceName();
	deviceInfo.pQueueList = &queueList;

	GFXDevice* pDevice = ctx.CreateDevice(pPhysicalDevice, deviceInfo);

	GFXSurfaceInfo surfaceInfo = {};
	surfaceInfo.debugName = "Default Surface";
	surfaceInfo.pWindow = pWindow;

	GFXSurface* pSurface = ctx.CreateSurface(pDevice, surfaceInfo);

	GFXQueue* pGraphicsQueue = pDevice->GetQueueHandle(0, 0);
	GFXQueue* pPresentQueue = pDevice->GetQueueHandle(0, 1);

	/////////////////

	GFXSwapchainInfo swapchainInfo	= {};
	swapchainInfo.debugName			= "Default Swapchain";
	swapchainInfo.sharingMode		= GFX_SHARING_MODE_EXCLUSIVE;
	swapchainInfo.presentMode		= GFX_PRESENT_MODE_TRIPPLE_BUFFERED;
	swapchainInfo.imageLayers		= 1;
	swapchainInfo.clipped			= false;

	GFXSwapchain* pSwapchain = ctx.CreateSwapchain(pDevice, pSurface, swapchainInfo);

	GFXImageArray swapchainImages = ctx.GetSwapChainImages(pSwapchain);
	GFXImageViewArray swapChainImageViews(swapchainImages.Size());

	GFXImageViewInfo viewInfo	= {};
	viewInfo.baseMipLevel		= 0;
	viewInfo.imageFormat		= swapchainImages[0]->GetImageFormat();
	viewInfo.imageType			= swapchainImages[0]->GetImageType();

	for (UInt32 i = 0; i < swapchainImages.Size(); i++)
	{
		viewInfo.debugName = "Swapchain Image View #";
		swapChainImageViews[i] = ctx.CreateImageView(swapchainImages[i], viewInfo);
	}

	/////////////////

	auto vertShaderCode = readFile("C:\\Development\\Quartz\\Quartz-Engine\\Source\\Sandbox\\src\\vert.spv");
	auto fragShaderCode = readFile("C:\\Development\\Quartz\\Quartz-Engine\\Source\\Sandbox\\src\\frag.spv");

	//auto vertShaderCode = readFile("C:\\DevStuffs\\Quartz-Engine\\Source\\Sandbox\\src\\vert.spv");
	//auto fragShaderCode = readFile("C:\\DevStuffs\\Quartz-Engine\\Source\\Sandbox\\src\\frag.spv");

	GFXShaderInfo vertexShaderInfo	= {};
	vertexShaderInfo.debugName		= "Default Vertex";
	vertexShaderInfo.shaderLang		= GFX_SHADER_LANG_SPIRV;
	vertexShaderInfo.shaderStage	= GFX_SHADER_STAGE_VERTEX;
	vertexShaderInfo.entryName		= "main";
	vertexShaderInfo.codeSize		= vertShaderCode.size();
	vertexShaderInfo.pCode			= vertShaderCode.data();

	GFXShaderInfo fragmentShaderInfo = {};
	fragmentShaderInfo.debugName	= "Default Fragment";
	fragmentShaderInfo.shaderLang	= GFX_SHADER_LANG_SPIRV;
	fragmentShaderInfo.shaderStage	= GFX_SHADER_STAGE_FRAGMENT;
	fragmentShaderInfo.entryName	= "main";
	fragmentShaderInfo.codeSize		= fragShaderCode.size();
	fragmentShaderInfo.pCode		= fragShaderCode.data();

	GFXShader* pVertexShader = ctx.CreateShader(pDevice, vertexShaderInfo);
	GFXShader* pFragmentShader = ctx.CreateShader(pDevice, fragmentShaderInfo);

	GFXShader* pShaderStages[2] = { pVertexShader, pFragmentShader };

	/////////////////

	struct Vertex
	{
		float position[3];
	};

	GFXVertexInputBindingInfo vertexBindingInfo = {};
	vertexBindingInfo.binding = 0;
	vertexBindingInfo.stride = sizeof(Vertex);
	vertexBindingInfo.inputRate = GFX_VERTEX_INPUT_RATE_VERTEX;

	GFXVertexInputAttributeInfo vertexAttribInfo = {};
	vertexAttribInfo.binding = 0;
	vertexAttribInfo.location = 0;
	vertexAttribInfo.format = GFX_FORMAT_R32G32B32_SFLOAT;
	vertexAttribInfo.offset = offsetof(Vertex, position);

	/////////////////

	GFXVertexInputStateInfo vertexStateInfo = {};
	vertexStateInfo.vertexBindingCount = 1;
	vertexStateInfo.pVertexBindingInfos = &vertexBindingInfo;
	vertexStateInfo.vertexAttributeCount = 1;
	vertexStateInfo.pVertexAttributeInfos = &vertexAttribInfo;

	GFXVertexInputState* pVertexInputState = ctx.CreateVertexInputState(vertexStateInfo);

	/////////////////

	GFXInputAssemblyStateInfo inputAssemblyInfo = {};
	inputAssemblyInfo.topology = GFX_PRIMITIVE_TOPOLOGY_TRIANGLES;

	GFXInputAssemblyState* pInputAssemblyState = ctx.CreateInputAssemblyState(inputAssemblyInfo);

	/////////////////

	GFXViewport viewport = {};
	viewport.x			= 0.0f;
	viewport.y			= 0.0f;
	viewport.width		= static_cast<Float32>(pSurface->GetWidth());
	viewport.height		= static_cast<Float32>(pSurface->GetHeight());
	viewport.minDepth	= 0.0f;
	viewport.maxDepth	= 1.0f;

	GFXScissor scissor = {};
	scissor.x			= 0;
	scissor.y			= 0;
	scissor.width		= static_cast<UInt32>(pSurface->GetWidth());
	scissor.height		= static_cast<UInt32>(pSurface->GetHeight());

	GFXViewportStateInfo viewportInfo = {};
	viewportInfo.viewport	= viewport;
	viewportInfo.scissor	= scissor;

	GFXViewportState* pViewportState = ctx.CreateViewportState(viewportInfo);

	/////////////////

	GFXRasterizationStateInfo rasterizationStatInfo = {};
	rasterizationStatInfo.fillMode	= GFX_POLYGON_FILL_MODE_FILL;
	rasterizationStatInfo.lineWidth = 1.0f;
	rasterizationStatInfo.cullFace	= GFX_CULL_FACE_NONE;
	rasterizationStatInfo.frontFace = GFX_FRONT_FACE_CLOCKWISE;

	GFXRasterizationState* pRasterizationState = ctx.CreateRasterizationState(rasterizationStatInfo);

	/////////////////

	GFXMultisampleStateInfo multisampleStateInfo = {};
	multisampleStateInfo.enableMultisampling	= false;
	multisampleStateInfo.multisamples			= 1;

	GFXMultisampleState* pMultisampleState = ctx.CreateMultisampleState(multisampleStateInfo);

	/////////////////

	GFXColorBlendAttachment colorAttachment = {};
	colorAttachment.blendEnable				= true;
	colorAttachment.srcColorBlendFactor		= GFX_BLEND_FACTOR_SRC_ALPHA;
	colorAttachment.srcAlphaBlendFactor		= GFX_BLEND_FACTOR_ONE;
	colorAttachment.colorBlendOp			= GFX_BLEND_OP_ADD;
	colorAttachment.destColorBlendFactor	= GFX_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
	colorAttachment.destAlphaBlendFactor	= GFX_BLEND_FACTOR_ZERO;
	colorAttachment.alphaBlendOp			= GFX_BLEND_OP_ADD;
	colorAttachment.colorMask				= GFX_COLOR_COMPONENT_ALL;

	GFXColorBlendStateInfo blendStateInfo = {};
	blendStateInfo.logicalOpEnable		= false;
	blendStateInfo.logicalOp			= GFX_LOGICAL_OP_COPY;
	blendStateInfo.attachmentCount		= 1;
	blendStateInfo.pAttachments			= &colorAttachment;
	blendStateInfo.blendConstants[4]	= {};

	GFXColorBlendState* pColorBlendState = ctx.CreateColorBlendState(blendStateInfo);

	/////////////////

	GFXRenderAttachmentInfo colorAttachmentInfo = {};
	colorAttachmentInfo.imageFormat		= swapchainImages[0]->GetImageFormat();
	colorAttachmentInfo.multisamples	= 1;
	colorAttachmentInfo.loadOp			= GFX_LOAD_OP_DONT_CARE;//GFX_LOAD_OP_CLEAR;
	colorAttachmentInfo.storeOp			= GFX_STORE_OP_STORE;
	colorAttachmentInfo.stencilLoadOp	= GFX_LOAD_OP_DONT_CARE;
	colorAttachmentInfo.stencilStoreOp	= GFX_STORE_OP_DONT_CARE;
	colorAttachmentInfo.initialLayout	= GFX_IMAGE_LAYOUT_UNDEFINED;
	colorAttachmentInfo.finalLayout		= GFX_IMAGE_LAYOUT_PRESENT_SRC;

	GFXRenderAttachment* pColorAttachment = ctx.CreateRenderAttachment(colorAttachmentInfo);

	GFXRenderAttachmentReference colorReference = { 0, GFX_IMAGE_LAYOUT_COLOR_ATTACHMENT };

	GFXRenderSubpassInfo colorSubpassInfo = {};
	colorSubpassInfo.bindPoint = GFX_PIPELINE_BIND_POINT_GRAPHICS;
	colorSubpassInfo.colorAttachmentCount = 1;
	colorSubpassInfo.pColorAttachments = &colorReference;

	GFXRenderSubpass* pColorSubpass = ctx.CreateRenderSubpass(colorSubpassInfo);

	/////////////////

	GFXSubpassDependency dependency = {};
	dependency.srcSubpassIndex	= VK_SUBPASS_EXTERNAL;
	dependency.destSubpassIndex = 0;
	dependency.srcStageMask		= GFX_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.destStageMask	= GFX_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask	= GFX_ACCESS_NONE;
	dependency.destAccessMask	= GFX_ACCESS_COLOR_ATTACHMENT_READ_BIT | GFX_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	GFXRenderPassInfo renderPassInfo = {};
	renderPassInfo.attachmentCount = 1;
	renderPassInfo.ppAttachments = &pColorAttachment;
	renderPassInfo.subpassCount = 1;
	renderPassInfo.ppSubpasses = &pColorSubpass;
	renderPassInfo.dependencyCount = 1;
	renderPassInfo.pDependencies = &dependency;

	GFXRenderPass* pRenderPass = ctx.CreateRenderPass(pDevice, renderPassInfo);

	/////////////////

	float positions[3 * 3] =
	{
		0.0f, -0.5f, 0.0f,
		0.5f,  0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
	};

	UInt16 indices[3] =
	{
		0, 1, 2
	};

	void* data;

	GFXBufferInfo vertexBufferInfo = {};
	vertexBufferInfo.debugName = "VertexBuffer";
	vertexBufferInfo.memoryFlags = GFX_BUFFER_MEMORY_VISIBLE_BIT | GFX_BUFFER_MEMORY_COHERENT_BIT;
	vertexBufferInfo.sharingMode = GFX_SHARING_MODE_EXCLUSIVE;
	vertexBufferInfo.usageFlags = GFX_BUFFER_USAGE_VERTEX_BIT;
	vertexBufferInfo.size = sizeof(float) * 9;

	GFXBuffer* pVertexBuffer = ctx.CreateBuffer(pDevice, vertexBufferInfo);

	ctx.MapBufferMemory(pVertexBuffer, 0, pVertexBuffer->GetSize(), 0, &data);
	memcpy(data, positions, pVertexBuffer->GetSize());
	ctx.UnmapBufferMemory(pVertexBuffer);

	GFXBufferInfo indexBufferInfo = {};
	indexBufferInfo.debugName = "IndexBuffer";
	indexBufferInfo.memoryFlags = GFX_BUFFER_MEMORY_VISIBLE_BIT | GFX_BUFFER_MEMORY_COHERENT_BIT;
	indexBufferInfo.sharingMode = GFX_SHARING_MODE_EXCLUSIVE;
	indexBufferInfo.usageFlags = GFX_BUFFER_USAGE_INDEX_BIT;
	indexBufferInfo.size = sizeof(UInt16) * 3;

	GFXBuffer* pIndexBuffer = ctx.CreateBuffer(pDevice, indexBufferInfo);

	ctx.MapBufferMemory(pIndexBuffer, 0, pIndexBuffer->GetSize(), 0, &data);
	memcpy(data, indices, pIndexBuffer->GetSize());
	ctx.UnmapBufferMemory(pIndexBuffer);

	/////////////////

	struct UBO
	{
		Matrix4 mvp;
	};

	Matrix4 model = Matrix4().SetTranslation({0.0f, 0.0f, -1.0f});
	Matrix4 view = Matrix4().SetLookAt({ 0.5f, 1.0f, 0.5f }, { 0,0,0 }, Vector3::Up);
	//Matrix4 view = Matrix4().SetView(Vector3::Right, Vector3::Up, Vector3::Forward, { 0,0,-2.0f });
	Matrix4 proj = Matrix4().SetPerspective(ToRadians(90.0f), 1280.0f / 720.0f, 0.01f, 1000.0f);

	//Matrix4 mvp = model * view * proj;
	//Matrix4 mvp = proj * view * model;
	Matrix4 mvp = model * proj;

	//mvp = mvp.Transpose(); //TODO; Transposed()

	UBO ubo = { mvp };

	GFXBufferInfo uboBufferInfo = {};
	uboBufferInfo.debugName = "UBOBuffer";
	uboBufferInfo.memoryFlags = GFX_BUFFER_MEMORY_VISIBLE_BIT | GFX_BUFFER_MEMORY_COHERENT_BIT;
	uboBufferInfo.sharingMode = GFX_SHARING_MODE_EXCLUSIVE;
	uboBufferInfo.usageFlags = GFX_BUFFER_USAGE_UNIFORM_BIT;
	uboBufferInfo.size = sizeof(UBO);

	GFXBuffer** uboBuffers = new GFXBuffer*[swapChainImageViews.Size()];

	for (UInt32 i = 0; i < swapChainImageViews.Size(); i++)
	{
		uboBuffers[i] = ctx.CreateBuffer(pDevice, uboBufferInfo);

		ctx.MapBufferMemory(uboBuffers[i], 0, uboBuffers[i]->GetSize(), 0, &data);
		memcpy(data, &ubo, uboBuffers[i]->GetSize());
		ctx.UnmapBufferMemory(uboBuffers[i]);
	}

	/////////////////

	GFXDescriptorBinding uboBinding = {};
	uboBinding.binding = 0;
	uboBinding.count = 1;
	uboBinding.type = GFX_DESCRIPTOR_TYPE_UNIFORM;
	uboBinding.stageFlags = GFX_SHADER_STAGE_VERTEX;

	GFXDescriptorSetLayoutInfo descriptorSetLayoutInfo = {};
	descriptorSetLayoutInfo.bindingCount = 1;
	descriptorSetLayoutInfo.pBindings = &uboBinding;

	GFXDescriptorSetLayout* pDescriptorSetLayout = ctx.CreateDescriptorSetLayout(pDevice, descriptorSetLayoutInfo);

	/////////////////

	GFXPipelineLayoutInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.layoutCount = 1;
	pipelineLayoutInfo.pDescriptorSetLayouts = pDescriptorSetLayout;

	GFXPipelineLayout* pPipelineLayout = ctx.CreatePipelineLayout(pDevice, pipelineLayoutInfo);

	/////////////////

	GFXPipelineInfo pipelineInfo		= {};
	pipelineInfo.shaderCount			= 2;
	pipelineInfo.ppShaders				= pShaderStages;
	pipelineInfo.pVertexInputState		= pVertexInputState;
	pipelineInfo.pInputAssemblyState	= pInputAssemblyState;
	pipelineInfo.pViewportState			= pViewportState;
	pipelineInfo.pRasterizationState	= pRasterizationState;
	pipelineInfo.pMultisampleState		= pMultisampleState;
	pipelineInfo.pColorBlendState		= pColorBlendState;
	pipelineInfo.pPipelineLayout		= pPipelineLayout;
	pipelineInfo.pRenderPass			= pRenderPass;
	pipelineInfo.subpassIndex			= 0;
	pipelineInfo.pParentPipeline		= NULL;
	pipelineInfo.parentPipelineIndex	= -1;

	GFXPipeline* pPipeline = ctx.CreatePipeline(pDevice, pipelineInfo);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	VkPhysicalDevice physicalDevice = static_cast<VulkanGFXPhysicalDevice*>(pPhysicalDevice)->GetVkPhysicalDevice();
	VkDevice device = static_cast<VulkanGFXDevice*>(pDevice)->GetVkDevice();
	VkSwapchainKHR swapchain = static_cast<VulkanGFXSwapchain*>(pSwapchain)->GetVkSwapchain();

	VkBuffer vertexBuffer = (static_cast<VulkanGFXBuffer*>(pVertexBuffer))->GetVkBuffer();
	VkBuffer indexBuffer = (static_cast<VulkanGFXBuffer*>(pIndexBuffer))->GetVkBuffer();
	
	VkBuffer* vkUboBuffers = new VkBuffer[swapChainImageViews.Size()];

	for (UInt32 i = 0; i < swapChainImageViews.Size(); i++)
	{
		vkUboBuffers[i] = (static_cast<VulkanGFXBuffer*>(uboBuffers[i]))->GetVkBuffer();
	}
	
	VkPipelineLayout vkPipelineLayout = static_cast<VulkanGFXPipelineLayout*>(pPipelineLayout)->GetVkPipelineLayout();


	VkDescriptorPool descriptorPool;

	VkDescriptorPoolSize poolSize {};
	poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
	poolSize.descriptorCount = static_cast<uint32_t>(swapChainImageViews.Size());

	VkDescriptorPoolCreateInfo poolInfo {};
	poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
	poolInfo.poolSizeCount = 1;
	poolInfo.pPoolSizes = &poolSize;
	poolInfo.maxSets = static_cast<uint32_t>(swapChainImageViews.Size());

	if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS)
	{
		Log.Error("Failed to create descriptor pool: vkCreateDescriptorPool failed!");
	}

	VkDescriptorSetLayout vkDescriptorSetLayout = static_cast<VulkanGFXDescriptorSetLayout*>(pDescriptorSetLayout)->GetVkDescriptorSetLayout();

	std::vector<VkDescriptorSetLayout> layouts(swapChainImageViews.Size(), vkDescriptorSetLayout);
	VkDescriptorSetAllocateInfo descAllocInfo{};
	descAllocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
	descAllocInfo.descriptorPool = descriptorPool;
	descAllocInfo.descriptorSetCount = static_cast<uint32_t>(swapChainImageViews.Size());
	descAllocInfo.pSetLayouts = layouts.data();

	std::vector<VkDescriptorSet> descriptorSets;
	descriptorSets.resize(swapChainImageViews.Size());

	if (vkAllocateDescriptorSets(device, &descAllocInfo, descriptorSets.data()) != VK_SUCCESS)
	{
		Log.Error("Failed to allocate descriptor sets: vkAllocateDescriptorSets failed!");
	}

	for (size_t i = 0; i < swapChainImageViews.Size(); i++) {
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = vkUboBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UBO);

		VkWriteDescriptorSet descriptorWrite{};
		descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrite.dstSet = descriptorSets[i];
		descriptorWrite.dstBinding = 0;
		descriptorWrite.dstArrayElement = 0;
		descriptorWrite.descriptorCount = 1;
		descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrite.pBufferInfo = &bufferInfo;
		descriptorWrite.pImageInfo = nullptr; // Optional
		descriptorWrite.pTexelBufferView = nullptr; // Optional

		vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
	}




	Array<GFXFramebuffer*> swapChainFramebuffers(swapChainImageViews.Size());

	for (UInt32 i = 0; i < swapChainImageViews.Size(); i++)
	{
		GFXFramebufferInfo framebufferInfo = {};
		framebufferInfo.pRenderPass = pRenderPass;
		framebufferInfo.attachmentCount = 1;
		framebufferInfo.ppAttachments = &swapChainImageViews[i];
		framebufferInfo.width = pSurface->GetWidth();
		framebufferInfo.height = pSurface->GetHeight();
		framebufferInfo.layers = 1;

		swapChainFramebuffers[i] = ctx.CreateFramebuffer(pDevice, framebufferInfo);
	}

	GFXCommandPoolInfo commandPoolInfo = {};
	commandPoolInfo.pFamily = pFamily;
	commandPoolInfo.flags = 0;

	GFXCommandPool* pCommandPool = ctx.CreateCommandPool(pDevice, commandPoolInfo);
	VkCommandPool commandPool = static_cast<VulkanGFXCommandPool*>(pCommandPool)->GetVkCommandPool();

	std::vector<VkCommandBuffer> commandBuffers;
	commandBuffers.resize(swapChainFramebuffers.Size());

	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = commandPool;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

	if (vkAllocateCommandBuffers(device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
	{
		Log.Error("Failed to allocate command buffers!");
	}

	for (size_t i = 0; i < commandBuffers.size(); i++)
	{
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = 0; // Optional
		beginInfo.pInheritanceInfo = nullptr; // Optional

		if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
		{
			Log.Error("Failed to begin recording command buffer!");
		}

		VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = static_cast<VulkanGFXRenderPass*>(pRenderPass)->GetVkRenderPass();
		renderPassInfo.framebuffer = static_cast<VulkanGFXFramebuffer*>(swapChainFramebuffers[i])->GetVkFramebuffer();
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = { pSurface->GetWidth(), pSurface->GetHeight() },
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, static_cast<VulkanGFXPipeline*>(pPipeline)->GetVkPipeline());

		VkBuffer vertexBuffers[] = { vertexBuffer };
		VkDeviceSize offsets[] = { 0 };

		vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);
		vkCmdBindIndexBuffer(commandBuffers[i], indexBuffer, 0, VK_INDEX_TYPE_UINT16);
		vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, vkPipelineLayout, 0, 1, &descriptorSets[i], 0, nullptr);

		vkCmdDrawIndexed(commandBuffers[i], 3, 1, 0, 0, 0);
		vkCmdEndRenderPass(commandBuffers[i]);

		if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
		{
			Log.Error("Failed to record command buffer!");
		}
	}

	constexpr int MAX_FRAMES_IN_FLIGHT = 8;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;

	imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
	inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
	imagesInFlight.resize(swapchainImages.Size(), VK_NULL_HANDLE);

	VkSemaphoreCreateInfo semaphoreInfo = {};
	semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	VkFenceCreateInfo fenceInfo = {};
	fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
	{
		if (vkCreateSemaphore(device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
			vkCreateSemaphore(device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
			vkCreateFence(device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)
		{
			Log.Error("Failed to create semaphores or fences!");
		}
	}

	uint32_t imageIndex;

	VkQueue graphicsQueue = static_cast<VulkanGFXQueue*>(pGraphicsQueue)->GetVkQueue();
	VkQueue presentQueue = static_cast<VulkanGFXQueue*>(pPresentQueue)->GetVkQueue();

	size_t currentFrame = 0;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/* Start Engine */
	Log.Info("Starting Engine...");
	Engine engine = Engine(nullptr);//Engine::GetInstanceHandle();

	/* Game Loop*/
	Time64 lastTime = 0.0;
	Time64 currentTime = 0.0; 
	Time64 deltaTime = 0.0;
	Time64 elapsedTime = 0.0;

	UInt32 fps = 0;

	currentTime = PlatformSystem::GetTimeNanoseconds();

	while (true)
	{
		//((float*)data)[0] += 0.0001f;

		engine.Update();
		pInput->Update();
		pWindow->Update();

		lastTime = currentTime;
		currentTime = PlatformSystem::GetTimeNanoseconds();
		deltaTime = currentTime - lastTime;
		elapsedTime += deltaTime;

		if (elapsedTime >= 1.0)
		{
			//Log.Info("MS: %.4llfms, FPS: %d", deltaTime * 1000.0, fps);
			elapsedTime = 0.0;
			fps = 0;
		}

#if 1
		vkWaitForFences(device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

		vkAcquireNextImageKHR(device, swapchain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

		if (imagesInFlight[imageIndex] != VK_NULL_HANDLE)
		{
			vkWaitForFences(device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
		}

		imagesInFlight[imageIndex] = inFlightFences[currentFrame];

		VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };

		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

		VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		vkResetFences(device, 1, &inFlightFences[currentFrame]);

		if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
		{
			Log.Error("Failed to submit draw command buffer!");
		}

		VkSwapchainKHR swapChains[] = { swapchain };

		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr; // Optional

		vkQueuePresentKHR(presentQueue, &presentInfo);
#endif
		
		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

		fps++;

	}

	vkDeviceWaitIdle(device);

	/*
	for (auto imageView : swapChainImageViews)
	{
		vkDestroyImageView(device, imageView, nullptr);
	}
	*/


	ctx.DestroyBuffer(pVertexBuffer);
	ctx.DestroyBuffer(pIndexBuffer);

	//vkDestroyBuffer(device, vertexBuffer, nullptr);
	//vkFreeMemory(device, vertexBufferMemory, nullptr);

	vkDestroyDescriptorPool(device, descriptorPool, nullptr);

	ctx.DestroyCommandPool(pCommandPool);
	ctx.DestroyPipleline(pPipeline);
	ctx.DestroyRenderPass(pRenderPass);
	ctx.DestroyRenderSubpass(pColorSubpass);
	ctx.DestroyRenderAttachment(pColorAttachment);
	ctx.DestroyPipelineLatout(pPipelineLayout);
	ctx.DestroyColorBlendState(pColorBlendState);
	ctx.DestroyMultisampleState(pMultisampleState);
	ctx.DestroyRasterizationState(pRasterizationState);
	ctx.DestroyViewportState(pViewportState);
	ctx.DestroyInputAssemblyState(pInputAssemblyState);
	ctx.DestroyVertexInputState(pVertexInputState);
	ctx.DestroySwapchain(pSwapchain);
	ctx.DestroySurface(pSurface);
	ctx.DestroyDevice(pDevice);
	ctx.DestroyGraphicsContext();

	/* Shutdown */
	Log.Info("Engine shutdown complete.");

	return 0;
}