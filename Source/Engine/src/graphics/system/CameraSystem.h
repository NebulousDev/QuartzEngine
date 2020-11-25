#pragma once

#include "Camera.h"
#include "Transform.h"

#include "../../input/InputHandler.h"

#define CAMERA_SPEED 2.0f

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

			cameraContext.BindMouseMoveInput("ViewHorizontal", 0, MOUSE_X, -1.0f * CAMERA_SPEED);
			cameraContext.BindMouseMoveInput("ViewVertical", 0, MOUSE_Y, -1.0f * CAMERA_SPEED);
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
}