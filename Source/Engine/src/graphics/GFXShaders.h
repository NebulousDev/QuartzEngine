#pragma once

#include "Common.h"
#include "GFXResource.h"

namespace Quartz
{
	class QUARTZ_API GFXVertexShader : public GFXResource
	{
	private:
		int temp;

	public:
		GFXVertexShader() = default;
	};

	class QUARTZ_API GFXPixelShader : public GFXResource
	{
	private:
		int temp;

	public:
		GFXPixelShader() = default;
	};

	class QUARTZ_API GFXHullShader : public GFXResource
	{
	private:
		int temp;

	public:
		GFXHullShader() = default;
	};

	class QUARTZ_API GFXDomainShader : public GFXResource
	{
	private:
		int temp;

	public:
		GFXDomainShader() = default;
	};

	class QUARTZ_API GFXGeometryShader : public GFXResource
	{
	private:
		int temp;

	public:
		GFXGeometryShader() = default;
	};

	class QUARTZ_API GFXComputeShader : public GFXResource
	{
	private:
		int temp;

	public:
		GFXComputeShader() = default;
	};
}