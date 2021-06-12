#pragma once

#include "Common.h"

namespace Quartz
{
	enum BufferUsageBits
	{
		BUFFER_USAGE_TRANSFER_SRC_BIT		= 0x01,
		BUFFER_USAGE_TRANSFER_DEST_BIT		= 0x02,
		BUFFER_USAGE_VERTEX_BUFFER_BIT		= 0x04,
		BUFFER_USAGE_INDEX_BUFFER_BIT		= 0x08,
		BUFFER_USAGE_UNIFORM_BUFFER_BIT		= 0x10,
		BUFFER_USAGE_STORAGE_BUFFER_BIT		= 0x20
	};

	typedef Flags32 BufferUsages;

	enum BufferAccessBits
	{
		BUFFER_ACCESS_DEVICE_LOCAL_BIT	= 0x01,
		BUFFER_ACCESS_HOST_VISIBLE_BIT	= 0x02,
		BUFFER_ACCESS_HOST_COHERENT_BIT = 0x04,
		BUFFER_ACCESS_HOST_CACHED_BIT	= 0x08,
	};

	typedef Flags32 BufferAccess;

	class QUARTZ_API Buffer
	{
	protected:
		UInt32			mSize;
		BufferUsages	mUsages;
		BufferAccess	mAccess;
		Bool8			mIsMapped;

	protected:
		Buffer(UInt32 size, BufferUsages usages, BufferAccess access);

	public:
		virtual void*	MapBuffer(UInt32 sizeBytes, UInt32 offsetBytes) = 0;
		virtual void	UnmapBuffer() = 0;

		FORCE_INLINE UInt32			GetSize() const { return mSize; }
		FORCE_INLINE BufferUsages	GetUsages() const { return mUsages; }
		FORCE_INLINE BufferAccess	GetAccess() const { return mAccess; }
		FORCE_INLINE Bool8			IsMapped() const { return mIsMapped; }
	};
}