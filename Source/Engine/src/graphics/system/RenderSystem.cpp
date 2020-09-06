#include "RenderSystem.h"

#include "util\Array.h"
#include "..\..\log\Log.h"

#include "../../Engine.h"

#include <iostream>
#include <string>
#include <fstream>

#include "../../object/OBJLoader.h"

#include "Camera.h"
#include "Transform.h"

#include "../../input/InputHandler.h"

namespace Quartz
{
	void TestNoScope(InputActionFlags actions, Float32 value)
	{
		printf("No scope!");
	}

	class CameraSystem : public SingletonSystem
	{
	private:
		Entity cameraEntity;
		Transform* pCameraTransform;
		Bool8 captured = false;

		void OnMouseMoveX(InputActionFlags actions, Float32 value)
		{
			if (captured)
			{
				Vector3 globalUp = Vector3(0, 1, 0);
				pCameraTransform->orientation *= Quaternion().SetAxisAngle(globalUp, value * Engine::GetInstanceHandle().GetDeltaTime() * 2.0f);
			}
		}

		void OnMouseMoveY(InputActionFlags actions, Float32 value)
		{
			if (captured)
			{
				Vector3 right = pCameraTransform->orientation * Vector3(1, 0, 0);
				pCameraTransform->orientation *= Quaternion().SetAxisAngle(right, value * Engine::GetInstanceHandle().GetDeltaTime() * 2.0f);
			}
		}

		void MoveForward(InputActionFlags actions, Float32 value)
		{
			Vector3 forward = pCameraTransform->orientation * Vector3(0, 0, 1);
			pCameraTransform->position += forward * (value * Float32(Engine::GetInstanceHandle().GetDeltaTime()) * -10.0f);
		}

		void MoveLeft(InputActionFlags actions, Float32 value)
		{
			Vector3 right = pCameraTransform->orientation * Vector3(1, 0, 0);
			pCameraTransform->position += right * (value * Float32(Engine::GetInstanceHandle().GetDeltaTime()) * 10.0f);
		}

		void Capture(InputActionFlags actions, Float32 value)
		{
			if (actions & INPUT_PRESSED)
			{
				Engine& engine = Engine::GetInstanceHandle();
				Window& window = engine.GetWindow();

				if (!engine.GetInputHandler().IsMouseCaptured() && PointInBounds(engine.GetInputHandler().GetMousePosition(), window.ClientBounds()))
				{
					engine.GetInputHandler().CaptureMouse(window, window.ClientBounds());
					printf("PRESS\n");
					captured = true;

					Log.General("CAPTURED");
				}
			}
		}

		void Release(InputActionFlags actions, Float32 value)
		{
			if (actions & INPUT_PRESSED)
			{
				Engine& engine = Engine::GetInstanceHandle();
				engine.GetInputHandler().ReleaseMouse();
				printf("ESC\n");
				captured = false;

				Log.General("RELEASED");
			}
		}

		InputContext cameraContext;

	public:
		CameraSystem()
			: cameraContext("cameraContext"), pCameraTransform(nullptr) { }

		void OnInit(EntityWorld& world) override
		{
			Camera camera;
			camera.fov = 90.0f;
			camera.projection = Matrix4().SetPerspective(ToRadians(camera.fov), 1260.0f / 720.0f, 0.001f, 1000.0f);
			
			Transform transform;
			transform.orientation = Quaternion(0, 0, 0, 1);
			transform.position = Vector3(0.0f, 0.0f, 5.0f);

			cameraEntity = world.CreateEntity(camera, transform);
			pCameraTransform = &world.GetComponent<Transform>(cameraEntity);

			cameraContext.BindMouseMoveInput("ViewHorizontal", 0, MOUSE_X, -1.0f);
			cameraContext.BindMouseMoveInput("ViewVertical", 0, MOUSE_Y, -1.0f);
			cameraContext.BindKeyboardInput("MoveForward", 0, 17, 1.0f);
			cameraContext.BindKeyboardInput("MoveForward", 0, 31, -1.0f);
			//cameraContext.BindControllerButtonInput("MoveForward", 0, 0, 1.0f);
			//cameraContext.BindControllerButtonInput("MoveForward", 0, 1, -1.0f);
			cameraContext.BindKeyboardInput("MoveLeft", 0, 30, 1.0f);
			cameraContext.BindKeyboardInput("MoveLeft", 0, 32, -1.0f);
			cameraContext.BindMouseButtonInput("Capture", 0, 0, 1.0f);
			cameraContext.BindKeyboardInput("Release", 0, 1, 1.0f);
			cameraContext.BindKeyboardInput("Test", 0, 2, 1.0f);

			Engine& engine = Engine::GetInstanceHandle();
			engine.GetInputHandler().SetInputContext(&cameraContext);
			engine.GetInputHandler().BindInputCallback("ViewHorizontal", &CameraSystem::OnMouseMoveX, this);
			engine.GetInputHandler().BindInputCallback("ViewVertical", &CameraSystem::OnMouseMoveY, this);
			engine.GetInputHandler().BindInputCallback("MoveForward", &CameraSystem::MoveForward, this);
			engine.GetInputHandler().BindInputCallback("MoveLeft", &CameraSystem::MoveLeft, this);
			engine.GetInputHandler().BindInputCallback("Capture", &CameraSystem::Capture, this);
			engine.GetInputHandler().BindInputCallback("Release", &CameraSystem::Release, this);

			engine.GetInputHandler().BindInputCallback("Test", &TestNoScope);
		
		}

		void OnUpdate(EntityWorld& world, Float32 deltaTime) override { }

		void OnTick(EntityWorld& world, Float32 deltaTime) override { }

		void OnDestroy(EntityWorld& world) override
		{
			Engine& engine = Engine::GetInstanceHandle();
			engine.GetInputHandler().UnbindInputCallbacks(this);
		}
	};

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

	void RenderSystem::OnInit(EntityWorld& world)
	{
		GFXContext& gfx = Engine::GetInstanceHandle().GetGFXContext();
		Window& window = Engine::GetInstanceHandle().GetWindow();

		mpSurface = gfx.CreateSurface(window, window.Width(), window.Height(), true, false);

		Array<Byte> modelFileData = ReadFile("models/bunny.obj");
		String modelDataString = String((char*)modelFileData.Data(), modelFileData.Size());

		Model model = LoadOBJ(modelDataString);

		mModel.SetTranslation({ 0.0f, 0.0f, 0.0f });
		mView.SetTranslation({ 0.0f, 0.0f, 5.0f });
		mProjection.SetIdentity();
		mProjection.SetPerspective(ToRadians(90.0f), (Float32)window.Width() / (Float32)window.Height(), 0.001f, 1000.0f);

		mModelUniform.AddMatrix4("model", mModel);
		mModelUniform.AddMatrix4("view", mView);
		mModelUniform.AddMatrix4("projection", mProjection);

		Array<Byte> vertexCode = ReadFile("shaders/vert.spv");
		GFXVertexShader* pVertexShader = gfx.CreateVertexShader(vertexCode);

		Array<Byte> pixelCode = ReadFile("shaders/frag.spv");
		GFXPixelShader* pPixelShader = gfx.CreatePixelShader(pixelCode);

		Array<GFXImage*> depthImages;

		GFXRenderPassInfo renderPassInfo = {};
		renderPassInfo.colorTargets.PushBack({ FORMAT_BGRA8_UNORM, GFX_LOAD_DONT_CARE, GFX_STORE_STORE }); // Swapchain image
		renderPassInfo.depthStencilTargets.PushBack({ FORMAT_DEPTH24_UNORM_STENCIL8_UINT, GFX_LOAD_CLEAR, GFX_STORE_STORE }); // depthStencil image
		GFXRenderPass* pRenderPass = gfx.CreateRenderPass(renderPassInfo);

		GFXGraphicsPipelineShaderState shaderState;
		shaderState.pVertexShader = pVertexShader;
		shaderState.pPixelShader = pPixelShader;

		mpGraphicsPipeline = gfx.CreateGraphicsPipeline(shaderState, model.vertexData.format, *pRenderPass, *mpSurface);

		mpVertexBuffer = gfx.CreateVertexBuffer(model.vertexData.buffer.Size(), true);
		void* pVertexData = gfx.MapVertexBuffer(mpVertexBuffer);
		memcpy(pVertexData, model.vertexData.buffer.Data(), mpVertexBuffer->GetSizeBytes());
		gfx.UnmapVertexBuffer(mpVertexBuffer);

		mpIndexBuffer = gfx.CreateIndexBuffer(sizeof(UInt32), model.indexData.buffer.Size() / sizeof(UInt32), true);
		void* pIndexData = gfx.MapIndexBuffer(mpIndexBuffer);
		memcpy(pIndexData, model.indexData.buffer.Data(), mpIndexBuffer->GetSizeBytes());
		gfx.UnmapIndexBuffer(mpIndexBuffer);

		for (UInt32 i = 0; i < mpSurface->GetBackBufferCount(); i++)
		{
			GFXUniformBuffer* pUniformBuffer = gfx.CreateUniformBuffer(mModelUniform.GetBuffer().Size() * sizeof(Float32), true);

			void* pUniformData = gfx.MapUniformBuffer(pUniformBuffer);
			memcpy(pUniformData, mModelUniform.GetBuffer().Data(), pUniformBuffer->GetSizeBytes());
			gfx.MapUniformBuffer(pUniformBuffer);

			mUniformBuffers.PushBack(pUniformBuffer);

			GFXImage* pDepthImage = gfx.CreateImage(IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT, mpSurface->GetWidth(), mpSurface->GetHeight(), FORMAT_DEPTH24_UNORM_STENCIL8_UINT, 1, 1);
			depthImages.PushBack(pDepthImage);

			Array<GFXImageView*> framebufferImages;
			framebufferImages.PushBack(&mpSurface->GetImageView(i));
			framebufferImages.PushBack(gfx.CreateImageView(IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT, pDepthImage, 0, 1, 0, 1));

			GFXFramebuffer* pPresentFramebuffer = gfx.CreateFramebuffer(mpGraphicsPipeline, mpSurface->GetWidth(), mpSurface->GetHeight(), framebufferImages);
			mPresentFramebuffers.PushBack(pPresentFramebuffer);

			GFXCommandBuffer* pGraphicsCommandBuffer = gfx.CreateGraphicsCommandBuffer();
			mGraphicsCommandBuffers.PushBack(pGraphicsCommandBuffer);

			pGraphicsCommandBuffer->Begin();
			gfx.BeginRenderPass(*pGraphicsCommandBuffer, *pRenderPass, *pPresentFramebuffer);

			gfx.BindGraphicsPipeline(*pGraphicsCommandBuffer, *mpGraphicsPipeline);
			gfx.BindVertexBuffer(*pGraphicsCommandBuffer, mpVertexBuffer);
			gfx.BindIndexBuffer(*pGraphicsCommandBuffer, mpIndexBuffer);
			gfx.BindUniformBuffer(*mpGraphicsPipeline, *pGraphicsCommandBuffer, pUniformBuffer);

			gfx.DrawIndexed(*pGraphicsCommandBuffer, mpIndexBuffer->GetCount(), 0);

			gfx.EndRenderPass(*pGraphicsCommandBuffer);
			pGraphicsCommandBuffer->End();
		}

		world.RegisterSystem<CameraSystem>();
	}

	void RenderSystem::OnUpdate(EntityWorld& world, Float32 deltaTime)
	{
		GFXContext& gfx = Engine::GetInstanceHandle().GetGFXContext();

		// Note: command buffers will need to be rebuilt for each camera
		// or more likely n-buffer them
		for (Entity entity : world.CreateView<Camera, Transform>())
		{
			Camera& camera = world.GetComponent<Camera>(entity);
			Transform& cameraTransfrom = world.GetComponent<Transform>(entity);

			mFrameIndex = mpSurface->AcquireNextImageIndex();

			/* Model */
			//mModel *= Matrix4().SetRotation(Quaternion().SetAxisAngle({ 0.0f, 1.0f, 0.0f }, 1.0f * Engine::GetInstanceHandle().GetDeltaTime()));
			//mModelUniform.SetMatrix4("model", mModel);

			/* View */
			Matrix4 view = Matrix4().SetTranslation(cameraTransfrom.position) * Matrix4().SetRotation(cameraTransfrom.orientation);
			mModelUniform.SetMatrix4("view", view);

			/* Projection */
			mModelUniform.SetMatrix4("projection", camera.projection);

			void* pUniformData = gfx.MapUniformBuffer(mUniformBuffers[mFrameIndex]);
			memcpy(pUniformData, mModelUniform.GetBuffer().Data(), mUniformBuffers[mFrameIndex]->GetSizeBytes());
			gfx.MapUniformBuffer(mUniformBuffers[mFrameIndex]);

			gfx.Submit(*mGraphicsCommandBuffers[mFrameIndex], *mpSurface);
			gfx.Present(mpSurface);
		}
	}

	void RenderSystem::OnTick(EntityWorld& world, Float32 deltaTime)
	{

	}

	void RenderSystem::OnDestroy(EntityWorld& world)
	{

	}
}

