#include "Win32FileSystem.h"

#include "Win32.h"

namespace Quartz
{
    Bool8 Win32FileSystem::IsValidPath(const String& path)
    {
        OFSTRUCT fileinfo;
        HFILE hfile = OpenFile(path.Str(), &fileinfo, OF_EXIST);
        return hfile != HFILE_ERROR;
    }

    Bool8 Win32FileSystem::IsValidFolder(const String& path)
    {
        DWORD attribs = GetFileAttributesA(path.Str());
        return (attribs != INVALID_FILE_ATTRIBUTES) && (attribs & FILE_ATTRIBUTE_DIRECTORY);
    }

    File* Win32FileSystem::Open(const String& path)
    {
        OFSTRUCT fileinfo;
        HFILE hfile = OpenFile(path.Str(), &fileinfo, OF_READ);
        
        if (hfile == HFILE_ERROR)
        {
            // Failed to open file
            return nullptr;
        }

        LARGE_INTEGER sizeBytes;
        GetFileSizeEx((HANDLE)hfile, &sizeBytes);

        return new File((Handle64)hfile, path, sizeBytes.QuadPart, true);
    }

    Bool8 Win32FileSystem::Close(File* pFile)
    {
        HFILE hfile = (HFILE)pFile->GetNativeHandle();

        if (!CloseHandle((HANDLE)hfile))
        {
            // Unable to close file
            return false;
        }

        delete pFile;

        return true;
    }
}

