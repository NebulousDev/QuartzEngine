#pragma once

#include "PhysicalDevice.h"
#include "GraphicsDevice.h"
#include "GraphicsQueue.h"

#include "PlatformWindow.h"

#include "util\Array.h"

namespace Quartz
{
	class QUARTZ_API GraphicsContext : public Castable
	{
	private:

	public:
		virtual void CreateInstance() {};

		virtual void CreateSurface(const Window& window, GraphicsDevice& device, UInt32 width, UInt32 height, Bool8 vSync, Bool8 fullscreen) {};

		virtual void CreateGraphicsPipeline() {};
		virtual void CreateComputePipeline() {};

		virtual void CreateVertexShader(GraphicsDevice& device, const String& shaderCode) {};
		virtual void CreatePixelShader(GraphicsDevice& device, const String& shaderCode) {};
		virtual void CreateHullShader(GraphicsDevice& device, const String& shaderCode) {};
		virtual void CreateDomainShader(GraphicsDevice& device, const String& shaderCode) {};;
		virtual void CreateGeometryShader(GraphicsDevice& device, const String& shaderCode) {};
		virtual void CreateComputeShader(GraphicsDevice& device, const String& shaderCode) {};
	
		virtual void CreateVertexBuffer() {};
		virtual void CreateIndexBuffer() {};
		virtual void CreateUniformBuffer() {};
	};

}