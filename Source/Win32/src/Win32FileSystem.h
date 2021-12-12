#pragma once

#include "platform/Filesystem.h"

namespace Quartz
{
	class QUARTZ_API Win32FileSystem : public PhysicalFileSystem
	{
		Bool8 IsValidPath(const String& path) override;
		Bool8 IsValidFolder(const String& path) override;
		File* Open(const String& path) override;
		Bool8 Close(File* pFile) override;
	};
}