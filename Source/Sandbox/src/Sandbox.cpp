
#include "Common.h"

#include "util/Array.h"
#include "util/String.h"

#include "math/Math.h"

#include "Win32Vulkan.h"
#include "Win32PlatformConsole.h"
#include "Win32Platform.h"
#include "Win32PlatformTime.h"

#include "Engine.h"
#include "application/WindowManager.h"
#include "application/Input.h"

#include "object/OBJLoader.h"

#include <iostream>
#include <fstream>

#include "log/Log.h"

_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;

namespace Quartz
{
	/*=====================================
		WINDOW/GRAPHICS PROPERITES
	=====================================*/

	#define DEFAULT_WIDTH 1280
	#define DEFAULT_HEIGHT 720
	#define SWAPCHAIN_IMAGE_COUNT 3
	#define FRAME_RESOURCE_COUNT 3

	/*=====================================
		SCENE RESOURCES
	=====================================*/

	#define MODEL_PATH "models/testScene.obj"
	#define DIFFUSE_PATH "textures/default.png"

	/*=====================================
		TEMP FILE READER
	=====================================*/

	static Array<Byte> ReadFile(const String& filename)
	{
		std::ifstream file(filename.Str(), std::ios::ate | std::ios::binary);

		if (!file.is_open())
		{
			Log::Error("Cannot open file %s", filename.Str());
			throw std::runtime_error("failed to open file!");
		}

		size_t fileSize = (size_t)file.tellg();
		Array<Byte> buffer(fileSize);

		file.seekg(0);
		file.read((char*)buffer.Data(), fileSize);

		file.close();

		return buffer;
	}

	/*=====================================
		TEMP GLOBALS
	=====================================*/

	Quaternion cameraOrientation;
	Vector3 cameraPosition;
	Double64 deltaTime;

	Input* gpInput;
	Window* gpWindow;

	/*=====================================
		TEMP WINDOW/INPUT CALLBACKS
	=====================================*/

	void MouseMoveCallback(HVPInputMouse mouse, Float32 rx, Float32 ry)
	{
		if (gpInput->IsMouseCaptured())
		{
			Vector3 globalUp = Vector3(0, 1, 0);
			Vector3 right = cameraOrientation * Vector3(1, 0, 0);
			cameraOrientation *= Quaternion().SetAxisAngle(globalUp, (Double64) - rx * deltaTime * 4.0);
			cameraOrientation *= Quaternion().SetAxisAngle(right, (Double64) - ry * deltaTime * 4.0);
		}
	}

	void CaptureMouseCallback(HVPInputMouse mouse, UInt32 button, InputActions actions, Float32 value)
	{
		Bounds2i bounds = gpWindow->GetBounds();
		RECT* clip = new RECT{ bounds.start.x, bounds.start.y, bounds.end.x, bounds.end.y };

		if (gpWindow->IsFocused() && gpWindow->IsMouseInside())
		{
			gpInput->CaptureCursor(*gpWindow);
			Log::Info("CAPTURING CURSOR");
			ShowCursor(false);
		}
	}

	void MoveCameraCallback(HVPInputKeyboard keyboard, UInt32 key, InputActions actions, Float32 value)
	{
		if (key == 1 && (actions & ACTION_PRESSED)) // Escape
		{
			gpInput->ReleaseCursor();
			Log::Info("RELEASING CURSOR");
			ShowCursor(true);
		}

		else if (key == 17) // W
		{
			Vector3 forward = cameraOrientation * Vector3(0, 0, -1);
			cameraPosition += forward * (value * deltaTime * 8.0f);
		}

		else if (key == 31) // S
		{
			Vector3 forward = cameraOrientation * Vector3(0, 0, 1);
			cameraPosition += forward * (value * deltaTime * 8.0f);
		}

		else if (key == 30) // A
		{
			Vector3 right = cameraOrientation * Vector3(1, 0, 0);
			cameraPosition += right * (value * deltaTime * 8.0f);
		}

		else if (key == 32) // D
		{
			Vector3 right = cameraOrientation * Vector3(-1, 0, 0);
			cameraPosition += right * (value * deltaTime * 8.0f);
		}
	}

	void WindowFocus(Window& window, Bool8 focus)
	{
		if (!focus)
		{
			Log::Debug("Unfocused!");
			Log::Info("RELEASING CURSOR");
			ShowCursor(true);
		}
	}
}

/*=====================================
	QUARTZ MAIN
=====================================*/

// @TODO: Move to using platform main instead

int main(int argc, char* argv[])
{
	using namespace Quartz;

	/*=====================================
		ENGINE INITIALIZATION
	=====================================*/

	Win32Platform		win32Platform;
	Win32VulkanContext	win32VulkanContext;

	EngineInfo engineInfo;
	engineInfo.gameInfo.name		= L"Sandbox";
	engineInfo.gameInfo.version		= L"1.0.0";
	engineInfo.pPlatform			= &win32Platform;
	engineInfo.pGraphics			= &win32VulkanContext;
	engineInfo.showDebugConsole		= true;

	Engine& engine = Engine::CreateInstance(engineInfo);
	engine.Start();

	/*=====================================
		INPUT BINDINGS
	=====================================*/

	InputBindings bindings;
	bindings.BindGlobalMouseMoveCallback("MouseMoveCallback", ANY_MOUSE, MouseMoveCallback, 1.0f, 1.0f);
	bindings.BindGlobalMouseButtonCallback("CaptureMouseCallback", ANY_MOUSE, 0, ACTION_PRESSED, CaptureMouseCallback, 1.0f);
	bindings.BindGlobalKeyboardKeyCallback("MoveCameraCallback", ANY_KEYBOARD, ANY_BUTTON, ACTION_DOWN, MoveCameraCallback, 1.0f);

	engine.GetInput().RegisterInputBindings(&bindings);

	GraphicsWindow* pWindow = engine.GetWindowManager().CreateGraphicsWindow(100, 100, DEFAULT_WIDTH, DEFAULT_HEIGHT, L"Sandbox", SWAPCHAIN_IMAGE_COUNT);
	pWindow->Show();
	pWindow->Focus();

	engine.GetWindowManager().RegisterWindowFocusCallback(*pWindow, WindowFocus);

	/*=====================================
		SET TEMP GLOBALS
	=====================================*/

	gpInput		= &engine.GetInput();
	gpWindow	= pWindow;

	/*=====================================
		GRAPHICS INITIALIZATION
	=====================================*/

	Win32VulkanContext* pVulkanContext = &win32VulkanContext;

	GFXRenderAttachment colorAttachment;
	colorAttachment.format			= GFX_IMAGE_FORMAT_BGRA8_UNORM;
	colorAttachment.initalLayout	= GFX_IMAGE_LAYOUT_UNDEFINED;
	colorAttachment.finalLayout		= GFX_IMAGE_LAYOUT_PRESENT;
	colorAttachment.stencilLoadOp	= GFX_LOAD_OP_DONT_CARE;
	colorAttachment.stencilStoreOp	= GFX_STORE_OP_DONT_CARE;
	colorAttachment.loadOp			= GFX_LOAD_OP_CLEAR;
	colorAttachment.storeOp			= GFX_STORE_OP_STORE;

	GFXRenderAttachment depthAttachment;
	depthAttachment.format			= GFX_IMAGE_FORMAT_DEPTH24_UNORM_STENCIL8_UINT;
	depthAttachment.initalLayout	= GFX_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout		= GFX_IMAGE_LAYOUT_DEPTH_STENCIL_OUTPUT;
	depthAttachment.stencilLoadOp	= GFX_LOAD_OP_DONT_CARE;
	depthAttachment.stencilStoreOp	= GFX_STORE_OP_DONT_CARE;
	depthAttachment.loadOp			= GFX_LOAD_OP_CLEAR;
	depthAttachment.storeOp			= GFX_STORE_OP_DONT_CARE;

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

	GFXGraphicsPipelineInfo pipelineInfo = {};
	{
		Bounds2f renderBounds(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);

		pipelineInfo.vertexShader		= pVulkanContext->CreateShader(GFX_SHADER_STAGE_VERTEX, ReadFile("shaders/vert.spv"), "main");
		pipelineInfo.pixelShader		= pVulkanContext->CreateShader(GFX_SHADER_STAGE_PIXEL, ReadFile("shaders/frag.spv"), "main");
		
		pipelineInfo.viewport.bounds	= renderBounds;
		pipelineInfo.viewport.minDepth	= 0.0f;
		pipelineInfo.viewport.maxDepth	= 1.0f;
		
		pipelineInfo.scissor.bounds		= renderBounds;
		
		pipelineInfo.topology			= GFX_PRIMITIVE_TOPOLOGY_TRIANGLES;
		pipelineInfo.polygonMode		= GFX_POLYGON_MODE_FILL;
		pipelineInfo.cullMode			= GFX_CULL_MODE_BACK;
		pipelineInfo.faceWind			= GFX_FACE_WIND_COUNTER_CLOCKWISE;
		pipelineInfo.lineWidth			= 1.0f;
		
		pipelineInfo.multisample		= GFX_MULTISAMPLE_DISABLED;
		
		pipelineInfo.depthTesting		= true;
		pipelineInfo.depthOperation		= GFX_COMPARE_OP_LESS_OR_EQUAL;

		GFXBufferAttachent vertexBufferAttachment;
		vertexBufferAttachment.binding	= 0;
		vertexBufferAttachment.stride	= 8 * sizeof(Float32);

		pipelineInfo.bufferAttachemnts.PushBack(vertexBufferAttachment);

		GFXVertexAttribute positionAttrib;
		positionAttrib.binding	= 0;
		positionAttrib.location	= 0;
		positionAttrib.type		= GFX_ATTRIBUTE_TYPE_FLOAT3;

		GFXVertexAttribute normalAttrib;
		normalAttrib.binding	= 0;
		normalAttrib.location	= 1;
		normalAttrib.type		= GFX_ATTRIBUTE_TYPE_FLOAT3;

		GFXVertexAttribute texCoordAttrib;
		texCoordAttrib.binding	= 0;
		texCoordAttrib.location = 2;
		texCoordAttrib.type		= GFX_ATTRIBUTE_TYPE_FLOAT2;

		pipelineInfo.vertexAttributes.PushBack(positionAttrib);
		pipelineInfo.vertexAttributes.PushBack(normalAttrib);
		pipelineInfo.vertexAttributes.PushBack(texCoordAttrib);

		GFXBlendAttachment colorOutputBlendAttachment;
		colorOutputBlendAttachment.blend = GFXColorBlend();
		pipelineInfo.blendAttachments.PushBack(colorOutputBlendAttachment);

		pipelineInfo.backbufferCount = SWAPCHAIN_IMAGE_COUNT;
	}

	HGFXGraphicsPipeline pipeline = pVulkanContext->CreateGraphicsPipeline(pipelineInfo, renderPass, 0);

	HGFXImage depthStencilImage = pVulkanContext->CreateImage(GFX_IMAGE_TYPE_2D, 
		GFX_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, 
		GFX_IMAGE_FORMAT_DEPTH24_UNORM_STENCIL8_UINT, DEFAULT_WIDTH, DEFAULT_HEIGHT, 1, 1, 1);

	HGFXImageView depthStencilView = pVulkanContext->CreateImageView(depthStencilImage, 
		GFX_IMAGE_VIEW_TYPE_2D, GFX_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT, 0, 1, 0, 1);

	/*=====================================
		UNIFORMS
	=====================================*/

	String modelPath = MODEL_PATH;

	if (argc > 1)
	{
		modelPath = argv[1];
	}

	Array<Byte> modelFileData = ReadFile(modelPath);
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
	ubo.proj.SetPerspective(ToRadians(80.0f), (Float32)DEFAULT_WIDTH / (Float32)DEFAULT_HEIGHT, 0.0001f, 1000.0f);

	/*=====================================
		BUFFERS
	=====================================*/

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

	/*=====================================
		GENERATE COMMAND BUFFERS
	=====================================*/

	HGFXCommandBuffer commandBuffers[SWAPCHAIN_IMAGE_COUNT];
	HGFXFramebuffer frameBuffers[SWAPCHAIN_IMAGE_COUNT];
	HGFXBuffer uniformBuffers[SWAPCHAIN_IMAGE_COUNT];

	for (UInt32 i = 0; i < SWAPCHAIN_IMAGE_COUNT; i++)
	{
		uniformBuffers[i] = pVulkanContext->CreateBuffer(GFX_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			GFX_BUFFER_ACCESS_HOST_VISIBLE_BIT | GFX_BUFFER_ACCESS_HOST_COHERENT_BIT, sizeof(UBO));
		void* pUniformData = pVulkanContext->MapBuffer(uniformBuffers[i]);
		memcpy(pUniformData, &ubo, sizeof(UBO));
		pVulkanContext->UnmapBuffer(uniformBuffers[i]);

		commandBuffers[i] = pVulkanContext->CreateCommandBuffer(GFX_COMMAND_BUFFER_USAGE_GRAPHICS_BIT);
		
		HGFXImageView swapChainImageView = pVulkanContext->GetSwapchainImageView(pWindow->GetSwapchain(), i);

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

	/*=====================================
		GAME LOOP
	=====================================*/

	Double64 currentTime = 0;
	Double64 lastTime = 0;
	//Double64 deltaTime = 0;
	Double64 accumulatedTime = 0;
	UInt32 frames = 0;
	
	Win32PlatformTime time;
	currentTime = time.GetTimeNanoseconds();
	lastTime = currentTime;

	UInt32 frameIndex = 0;
	UInt32 lastFrameIndex = 0;

	while (true)
	{
		currentTime = time.GetTimeNanoseconds();
		deltaTime = currentTime - lastTime;
		accumulatedTime += deltaTime;
		lastTime = currentTime;

		if (accumulatedTime >= 1.0)
		{
			Log::Debug("FPS: %d", frames);
			accumulatedTime = 0;
			frames = 0;
		}

		lastFrameIndex = frameIndex;
		frameIndex = pVulkanContext->AcquireSwapchainImageIndex(pWindow->GetSwapchain());

		ubo.view = Matrix4().SetTranslation(cameraPosition) * Matrix4().SetRotation(cameraOrientation);

		void* pUniformData = pVulkanContext->MapBuffer(uniformBuffers[frameIndex]);
		memcpy(pUniformData, &ubo, sizeof(UBO));
		pVulkanContext->UnmapBuffer(uniformBuffers[frameIndex]);

		pVulkanContext->Submit(commandBuffers[frameIndex], pWindow->GetSwapchain());
		pVulkanContext->Present(pWindow->GetSwapchain());

		win32Platform.PollEvents();

		frames++;

		//win32Platform.PollConnections();
		win32Platform.PollInput();

		engine.GetInput().Update();
	}

	/*=====================================
		SHUTDOWN
	=====================================*/

	engine.GetWindowManager().DestroyGraphicsWindow(pWindow);

	return 0;
}