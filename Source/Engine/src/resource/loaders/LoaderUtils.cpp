#include "LoaderUtils.h"

#include <iostream>
#include <fstream>

#include "../../log/Log.h"

namespace Quartz
{
    Bool8 ReadFileBinary(const String& filename, Array<Byte>& buffer)
    {
        std::ifstream file(filename.Str(), std::ios::ate | std::ios::binary);

        if (!file.is_open())
        {
            Log::Error("Cannot open file %s", filename.Str());
            throw std::runtime_error("failed to open file!");
            return false;
        }

        size_t fileSize = (size_t)file.tellg();
        buffer.Resize(fileSize);

        file.seekg(0);
        file.read((char*)buffer.Data(), fileSize);

        file.close();

        return true;
    }

    Bool8 ReadFileString(const String& filename, String& str)
    {
        std::ifstream file(filename.Str(), std::ios::ate | std::ios::binary);

        if (!file.is_open())
        {
            Log::Error("Cannot open file %s", filename.Str());
            throw std::runtime_error("failed to open file!");
            return false;
        }

        size_t fileSize = (size_t)file.tellg();
        str.Resize(fileSize);

        file.seekg(0);
        file.read((char*)str.Data(), fileSize);

        file.close();

        return true;
    }
}