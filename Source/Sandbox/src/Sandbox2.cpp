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

#include "input/InputContext.h"

#include "application/WindowManager.h"
#include "Win32WindowManager.h"

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

int main()
{
	Engine& engine = Engine::GetInstanceHandle();

	/* Initialize Platform */

	Platform* pPlatform = new Win32Platform();
	engine.SetPlatform(pPlatform);

	/* Create Window */

	Win32WindowManager manager;
	engine.SetWindowManager(manager);
	Window& window = manager.CreateWindow(StringW(L"Quartz Engine"), 100, 100, 1280, 720, false, true);
	engine.SetWindow(&window);
	window.Show();
	window.Focus();

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