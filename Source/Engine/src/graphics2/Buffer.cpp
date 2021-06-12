#include "Buffer.h"

namespace Quartz
{
	Buffer::Buffer(UInt32 size, BufferUsages usages, BufferAccess access)
		: mSize(size),
		mUsages(usages),
		mAccess(access),
		mIsMapped(false)
	{
		// Nothing
	}
}

