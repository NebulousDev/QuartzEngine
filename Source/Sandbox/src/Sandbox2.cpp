// Sandbox Main

#include "Common.h"

#include "Win32Platform.h"
#include "Engine.h"

#include "util\Map.h"
#include "util\Array.h"

#include "VulkanContext.h"
#include "VulkanSurface.h"
#include "VulkanShader.h"
#include "VulkanPipeline.h"
#include "VulkanBuffer.h"
#include "graphics\GFXUtil.h"

#include "log\Log.h"

#include <iostream>
#include <string>
#include <fstream>

#include "entity\World.h"
#include "TestSystem.h"

using namespace Quartz;

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

struct TestComponent0
{
	int val = 0;
	TestComponent0(int val) : val(val) { printf("Created 0!\n"); }
	TestComponent0(const TestComponent0& val) : val(val.val) { printf("Copied 0!\n"); }
	TestComponent0(TestComponent0&& val) noexcept : val(val.val) { printf("Moved 0!\n"); }

	TestComponent0& operator=(TestComponent0 component)
	{
		val = component.val;
		printf("Swapped 1\n");
		return *this;
	}
};

struct TestComponent1
{
	int val = 0;
	TestComponent1(int val) : val(val) { printf("Created 1!\n"); }
	TestComponent1(const TestComponent1& val) : val(val.val) { printf("Copied 1!\n"); }
	TestComponent1(TestComponent1&& val) noexcept : val(val.val) { printf("Moved 1!\n"); }

	TestComponent1& operator=(TestComponent1 component)
	{
		val = component.val;
		printf("Swapped 1\n");
		return *this;
	}
};

struct TestComponent2
{
	int val = 0;
	TestComponent2(int val) : val(val) { printf("Created 2!\n"); }
	TestComponent2(const TestComponent2& val) : val(val.val) { printf("Copied 2!\n"); }
	TestComponent2(TestComponent2&& val) noexcept : val(val.val) { printf("Moved 2!\n"); }

	TestComponent2& operator=(TestComponent2 component)
	{
		val = component.val;
		printf("Swapped 1\n");
		return *this;
	}
};

int main()
{
	{
		EntityWorld world;

		Entity testEntity0 = world.CreateEntity();
		Entity testEntity1 = world.CreateEntity();
		Entity testEntity2 = world.CreateEntity();
		Entity testEntity3 = world.CreateEntity();

		Entity cmpEntity0 = world.CreateEntity(TestComponent0(15));
		Entity cmpEntity1 = world.CreateEntity(TestComponent1(26));
		Entity cmpEntity2 = world.CreateEntity(TestComponent0(35), TestComponent1(36), TestComponent2(37));

		Bool8 has0 = world.HasComponent<TestComponent0>(cmpEntity0);
		Bool8 has1 = world.HasComponent<TestComponent1>(cmpEntity0);
		Bool8 has2 = world.HasComponent<TestComponent0, TestComponent1, TestComponent2>(cmpEntity0);
		Bool8 has3 = world.HasComponent<TestComponent0, TestComponent1, TestComponent2>(cmpEntity2);

		world.RemoveComponent<TestComponent0>(cmpEntity2);

		Bool8 has4 = world.HasComponent<TestComponent0, TestComponent1, TestComponent2>(cmpEntity2);

		TestComponent0& test0 = world.GetComponent<TestComponent0>(cmpEntity0);
		test0.val = 35;
		TestComponent0& test0_1 = world.GetComponent<TestComponent0>(cmpEntity0);

		TestComponent2& test1 = world.GetComponent<TestComponent2>(cmpEntity2);

		TestComponent1 after1(100);
		world.AddComponent(cmpEntity0, after1);
		TestComponent1& after1_get = world.GetComponent<TestComponent1>(cmpEntity0);

		EntityView<TestComponent1, TestComponent2> view12 = world.CreateView<TestComponent1, TestComponent2>();

		for (Entity entity : view12)
		{
			TestComponent1& cmp1 = world.GetComponent<TestComponent1>(entity);
			TestComponent2& cmp2 = world.GetComponent<TestComponent2>(entity);
		}

		EntityView<TestComponent0> view0 = world.CreateView<TestComponent0>();

		for (Entity entity : view0)
		{
			TestComponent0& cmp0 = world.GetComponent<TestComponent0>(entity);
		}

		EntityView<TestComponent1> view1 = world.CreateView<TestComponent1>();

		for (Entity entity : view1)
		{
			TestComponent1& cmp1 = world.GetComponent<TestComponent1>(entity);
		}

		world.RegisterSystem<TestSystem>();
		
		world.CreateEntity<TestComponent>(TestComponent(100));
		world.CreateEntity<TestComponent>(TestComponent(200));
		world.CreateEntity<TestComponent>(TestComponent(300));
		world.CreateEntity<TestComponent>(TestComponent(400));

		world.Update();

		world.DeregisterSystem<TestSystem>();
	}

	Engine& engine = Engine::GetInstanceHandle();

	/* Initialize Platform */

	Platform* pPlatform = new Win32Platform();
	engine.SetPlatform(pPlatform);

	/* Create Window */

	Window* pWindow = engine.GetPlatform().CreateWindow(1280, 720, "Quartz", 0);
	pWindow->Show();
	engine.SetWindow(pWindow);

	/* Graphics Context */

	Array<String> extensions;
	extensions.PushBack(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	extensions.PushBack(VK_KHR_SURFACE_EXTENSION_NAME);
	extensions.PushBack("VK_KHR_win32_surface");

	Array<String> validationLayers;
	validationLayers.PushBack("VK_LAYER_KHRONOS_validation");

	VulkanContext* pContext = new VulkanContext("Sandbox", "Quartz Engine", extensions, validationLayers);
	engine.SetGraphicsContext(pContext);

	/* Start Engine */

	engine.Init();
	engine.Start();
}