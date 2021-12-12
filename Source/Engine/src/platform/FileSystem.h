#pragma once

#include "util/Map.h"
#include "util/String.h"

namespace Quartz
{
	class QUARTZ_API File
	{
	public:
		friend class FileSystem;

	private:
		Handle64 mHandle;
		String mFullPath;
		UInt64 mSizeBytes;
		Bool8 mOpen;

	public:
		File(Handle64 handle, const String& fullpath, UInt64 sizeBytes, Bool8 open);

		Handle64 GetNativeHandle() const { return mHandle; }
		const String& GetPath() const { return mFullPath; }
		UInt64 GetSizeBytes() const { return mSizeBytes; }
		Bool8 IsOpen() const { return mOpen; }
	};

	class QUARTZ_API PhysicalFileSystem
	{
	public:
		virtual Bool8 IsValidPath(const String& path) = 0;
		virtual Bool8 IsValidFolder(const String& path) = 0;
		virtual File* Open(const String& path) = 0;
		virtual Bool8 Close(File* pFile) = 0;
	};

	class QUARTZ_API VirtualFileSystem
	{

	};

	class QUARTZ_API FileSystem
	{
	private:
		PhysicalFileSystem*			mpPhysicalFileSystem;
		Map<String, Array<String>>	mMounts;

	public:
		Bool8 SetPhysicalFileSystem(PhysicalFileSystem* pPhysicalFileSystem);

		Bool8 Mount(const String& virtualPath, const String& physicalPath);
		//Bool8 MountBinary(const String& virtualPath, const String& blobPath);

		File* Open(const String& path);
		void Close(File* pFile);

		Bool8 IsVirtualPath(const String& path);
		Bool8 ResolvePhysicalPath(const String& virtualPath, String& resolvedPath);
		Bool8 ResolvePhysicalFolder(const String& virtualPath, String& resolvedPath);

		Bool8 IsValidPath(const String& path);
		Bool8 IsValidFolder(const String& path);
	};
}