#pragma once

#include "Common.h"

namespace Quartz
{
	/*
		An integer handle created for sparse set lookups.
		Contains version bits to assure lifetime and an index bits
		offset into the sparse array
	*/
	template<typename _HandleIntType, typename _VersionIntType, USize versionBits>
	class EntityHandle
	{
	public:
		using HandleIntType = _HandleIntType;
		using VersionIntType = _VersionIntType;
		using SparseHandleType = EntityHandle<_HandleIntType, _VersionIntType, versionBits>;

	private:
		constexpr static USize INDEX_BITS = (sizeof(HandleIntType) * 8) - versionBits;
		constexpr static USize VERSION_BITS = versionBits;
		constexpr static USize NULL_HANDLE = HandleIntType(0);

	public:
		union
		{
			struct
			{
				/* The index into the sparse array */
				HandleIntType index : INDEX_BITS;

				/* The version of the handle */
				VersionIntType version : VERSION_BITS;
			};

			/* The compact sparse handle value */
			HandleIntType handle;
		};

		operator HandleIntType() const
		{
			return handle;
		}

		constexpr EntityHandle()
			: handle(NULL_HANDLE) {}

		constexpr EntityHandle(HandleIntType handle)
			: handle(handle) {}

		constexpr EntityHandle(HandleIntType index, VersionIntType version)
			: index(index), version(version) {}
	};

	/* 4-bits version, 12-bits index */
	using Entity16 = EntityHandle<UInt16, UInt8, 4>;

	/* 8-bits version, 24-bits index */
	using Entity32 = EntityHandle<UInt32, UInt8, 8>;

	/* 16-bits version, 48-bits index */
	using Entity64 = EntityHandle<UInt64, UInt16, 16>;

	/* Standard Entity */
	using Entity = Entity32;

	/* Null Entity */
	constexpr Entity NullEntity(0);
}