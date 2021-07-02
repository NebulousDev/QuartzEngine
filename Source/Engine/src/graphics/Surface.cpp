
#include "Surface.h"

namespace Quartz
{
	Surface::Surface(UInt32 width, UInt32 height, Bool8 supportsHDR)
		: mWidth(width),
		mHeight(height),
		mSupportsHDR(supportsHDR)
	{
		// Nothing
	}
}