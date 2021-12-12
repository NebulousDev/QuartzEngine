#include "SpirvLoader.h"

#include "LoaderUtils.h"

namespace Quartz
{
    QUARTZ_API Bool8 LoadSpirv(const String& path, SpirvObject* pSpirvObject)
    {
        //TODO: add metadata and validation
        ReadFileBinary(path, pSpirvObject->binary);
        return true;
    }
}

