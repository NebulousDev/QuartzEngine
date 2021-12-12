#pragma once

#include "util/String.h"
#include "util/Array.h"

namespace Quartz
{
    QUARTZ_API Bool8 ReadFileBinary(const String& filename, Array<Byte>& buffer);
    QUARTZ_API Bool8 ReadFileString(const String& filename, String& str);
}