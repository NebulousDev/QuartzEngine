#pragma once

#include "Common.h"
#include "Viewport.h"
#include "Image.h"

namespace Quartz
{
	enum UniformType
	{
		UNIFORM_STATIC,
		UNIFORM_DYNAMIC
	};

	enum UniformFlagBits
	{
		UNIFORM_USE_STAGING = 0x1
	};

	typedef Flags32 UniformFlags;

	class QUARTZ_API Uniform
	{
	protected:
		UniformType		mType;
		UInt32			mElementSize;
		UInt32			mElementCount;
		UniformFlags	mFlags;

	public:
		Uniform(UniformType type, UInt32 elementSize, UInt32 elementCount, UniformFlags flags);

		virtual void Set(void* pData) = 0;
		virtual void SetElement(Context* pContext, UInt32 element, void* pData) = 0;
		virtual void SetElementArray(Context* pContext, UInt32 startIndex, UInt32 endIndex, void* pData) = 0;

		FORCE_INLINE UniformType	GetType() const { return mType; }
		FORCE_INLINE UInt32			GetElementSize() const { return mElementSize; }
		FORCE_INLINE UInt32			GetElementCount() const { return mElementCount; }
		FORCE_INLINE Bool8			IsUsingStaging() const { return mFlags & UNIFORM_USE_STAGING; }
	};

	class QUARTZ_API UniformTextureSampler
	{
	public:
		virtual void Set(ImageView* pImageView) = 0;
	};
}