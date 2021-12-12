#include "FileSystem.h"

namespace Quartz
{
    File::File(Handle64 handle, const String& fullpath, UInt64 sizeBytes, Bool8 open)
        : mHandle(handle), mFullPath(fullpath), mSizeBytes(sizeBytes), mOpen(open)
    {
        // Nothing
    }

    Bool8 FileSystem::IsVirtualPath(const String& path)
    {
        return path.Str()[0] == '/';
    }

    Bool8 FileSystem::ResolvePhysicalPath(const String& virtualPath, String& resolvedPath)
    {
        const Char* pPathStr = virtualPath.Str();
        String pathHeader;

        UInt32 index = 0;
        while (*pPathStr != '\0')
        {
            if (*pPathStr == '/' || *pPathStr == '\\')
            {
                pathHeader = String(virtualPath.Str(), index);

                Array<String>* pMounts = mMounts.Get(pathHeader);

                if (pMounts)
                {
                    // Resolve backward (newest mounts first)
                    for (UInt32 i = pMounts->Size(); i > 0; i++)
                    {
                        String finalPath = (*pMounts)[i - 1] + String(pPathStr, virtualPath.Length() - index);

                        if (mpPhysicalFileSystem->IsValidPath(finalPath))
                        {
                            resolvedPath = finalPath;
                            return true;
                        }
                    }

                    // No file in mount
                    return false;
                }
            }

            index++;
            pPathStr++;
        }

        // No mount found
        return false;
    }

    Bool8 FileSystem::ResolvePhysicalFolder(const String& virtualPath, String& resolvedPath)
    {
        const Char* pPathStr = virtualPath.Str();
        String pathHeader;

        UInt32 index = 0;
        while (*pPathStr != '\0')
        {
            if (*pPathStr == '/' || *pPathStr == '\\')
            {
                pathHeader = String(virtualPath.Str(), index);

                Array<String>* pMounts = mMounts.Get(pathHeader);

                if (pMounts)
                {
                    // Resolve backward (newest mounts first)
                    for (UInt32 i = pMounts->Size(); i > 0; i++)
                    {
                        String finalPath = (*pMounts)[i - 1] + String(pPathStr, virtualPath.Length() - index);

                        if (mpPhysicalFileSystem->IsValidFolder(finalPath))
                        {
                            resolvedPath = finalPath;
                            return true;
                        }
                    }

                    // No file in mount
                    return false;
                }
            }

            index++;
            pPathStr++;
        }

        // No mount found
        return false;
    }

    Bool8 FileSystem::SetPhysicalFileSystem(PhysicalFileSystem* pPhysicalFileSystem)
    {
        if (!pPhysicalFileSystem)
        {
            return false;
        }

        mpPhysicalFileSystem = pPhysicalFileSystem;

        return true;
    }

    Bool8 FileSystem::Mount(const String& virtualPath, const String& physicalPath)
    {
        if (!mpPhysicalFileSystem->IsValidFolder(physicalPath))
        {
            // Invalid physical path
            return false;
        }

        Array<String>* pMounts = mMounts.Get(virtualPath);

        if (!pMounts)
        {
            pMounts = &mMounts.Put(virtualPath, Array<String>());
        }

        pMounts->PushBack(physicalPath);

        return true;
    }

    File* FileSystem::Open(const String& path)
    {
        if (IsVirtualPath(path))
        {
            String physicalPath;

            if (!ResolvePhysicalPath(path, physicalPath))
            {
                // Failed to resolve path
                return nullptr;
            }

            return mpPhysicalFileSystem->Open(physicalPath);
        }
        else
        {
            return mpPhysicalFileSystem->Open(path);
        }
    }

    void FileSystem::Close(File* pFile)
    {
        mpPhysicalFileSystem->Close(pFile);
    }

    Bool8 FileSystem::IsValidPath(const String& path)
    {
        if (IsVirtualPath(path))
        {
            String physicalPath;

            if (!ResolvePhysicalPath(path, physicalPath))
            {
                false;
            }

            return true;
        }
        else
        {
            return mpPhysicalFileSystem->IsValidPath(path);
        }
    }

    Bool8 FileSystem::IsValidFolder(const String& path)
    {
        if (IsVirtualPath(path))
        {
            String physicalPath;

            if (!ResolvePhysicalFolder(path, physicalPath))
            {
                false;
            }

            return true;
        }
        else
        {
            return mpPhysicalFileSystem->IsValidFolder(path);
        }
    }
}

