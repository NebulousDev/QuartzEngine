#pragma once

#include "Common.h"
#include "util\Array.h"

#include "GFXResource.h"

namespace Quartz
{
	class QUARTZ_API GFXVertexBuffer : public GFXResource
	{
	private:
		UInt64 mSizeBytes;
		Bool8 mHostVisible;

	public:
		GFXVertexBuffer(UInt64 sizeBytes, Bool8 hostVisible)
			: mSizeBytes(sizeBytes), mHostVisible(hostVisible) {}

		UInt64 GetSizeBytes() const { return mSizeBytes; }
		Bool8 IsHostVisible() const { return mHostVisible; }
	};

	class QUARTZ_API GFXIndexBuffer : public GFXResource
	{
	private:
		UInt64 mStride;
		UInt64 mCount;
		Bool8 mHostVisible;

	public:
		GFXIndexBuffer(UInt64 stride, UInt64 size, Bool8 hostVisible)
			: mStride(stride), mCount(size), mHostVisible(hostVisible) {}

		UInt64 GetSizeBytes() const { return mStride * mCount; }
		UInt64 GetStride() const { return mStride; }
		UInt64 GetCount() const { return mCount; }
		Bool8 IsHostVisible() const { return mHostVisible; }
	};

	class QUARTZ_API GFXUniformBuffer : public GFXResource
	{
	private:
		UInt64 mSizeBytes;
		Bool8 mHostVisible;

	public:
		GFXUniformBuffer(UInt64 sizeBytes, Bool8 hostVisible)
			: mSizeBytes(sizeBytes), mHostVisible(hostVisible) {}

		UInt64 GetSizeBytes() const { return mSizeBytes; }
		Bool8 IsHostVisible() const { return mHostVisible; }
	};

	class QUARTZ_API GFXTextureBuffer : public GFXResource
	{
	private:
		UInt64 mSizeBytes;
		Bool8 mHostVisible;

	public:
		GFXTextureBuffer(UInt64 sizeBytes, Bool8 hostVisible)
			: mSizeBytes(sizeBytes), mHostVisible(hostVisible) {}

		UInt64 GetSizeBytes() const { return mSizeBytes; }
		Bool8 IsHostVisible() const { return mHostVisible; }
	};
}