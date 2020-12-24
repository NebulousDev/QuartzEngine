#pragma once

#include "../Common.h"

namespace Quartz
{
	typedef UInt64 TypeId;

	template<typename Type>
	class TypeInfo
	{
	private:
		constexpr static UInt64 Hash(const char* signature)
		{
			const char* val = signature;
			UInt64 value = 14695981039346656037ull;

			while (*val != 0)
			{
				value = (value ^ static_cast<UInt64>(*(val++))) * 1099511628211ull;
			}

			return value;
		}

		template<USize StrSize>
		constexpr static UInt64 TypeHash(const char (&signature)[StrSize])
		{
			return Hash(signature);
		}

	public:
		constexpr static TypeId Id()
		{
			constexpr TypeId value = static_cast<TypeId>(TypeHash(__FUNCSIG__));
			return value;
		}
	};

}