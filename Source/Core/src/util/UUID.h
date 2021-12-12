#pragma once

#include "../Common.h"
#include "Utils.h"
#include "Hash.h"

namespace Quartz
{
#define INVALID_UUID_VALUE 0
#define INVALID_UUID (UUID)INVALID_UUID_VALUE

	template<typename IntType>
	IntType GenerateUUID();

	template<>
	FORCE_INLINE UInt32 GenerateUUID()
	{
		return Mersenne19937_32();
	}

	template<>
	FORCE_INLINE UInt64 GenerateUUID()
	{
		return Mersenne19937_64();
	}

	template<typename IntType>
	class QUARTZ_API UUIDValue
	{
	private:
		IntType mId;

	public:
		UUIDValue() : mId(GenerateUUID<IntType>()) {}
		explicit UUIDValue(IntType uuid) : mId(uuid) {}

		IntType ID() const { return mId; }
		Bool8 IsValid() const { return mId != INVALID_UUID_VALUE; }

		operator IntType() const { return mId; }

		Bool8 operator==(const UUIDValue& value) const { return value.mId == mId; }
	};

	using UUID32 = UUIDValue<UInt32>;
	using UUID64 = UUIDValue<UInt64>;

	using UUID = UUID64;

	template<>
	FORCE_INLINE UInt32 Hash<UUID32>(const UUID32& value)
	{
		return static_cast<UInt32>(value.ID());
	}

	template<>
	FORCE_INLINE UInt32 Hash<UUID64>(const UUID64& value)
	{
		return static_cast<UInt32>(value.ID());
	}
}