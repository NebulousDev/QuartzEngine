#include "Framebuffer.h"

namespace Quartz
{
	FramebufferCollection::FramebufferCollection(UInt32 width, UInt32 height, const Renderpass& renderpass, UInt32 bufferCount)
	{

	}

	UInt64 Framebuffer::RegenerateHash()
	{
		// TODO

		return 0;
	}

	UInt64 ImageView::RegenerateHash()
	{
		return 0;
	}
}

