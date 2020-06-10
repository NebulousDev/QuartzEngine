#pragma once

#include "../Common.h"
#include "Hash.h"

#include <cstring>

namespace Quartz
{
	template<typename CharType>
	UInt32 StringLength(const CharType* pString)
	{ 
		return 0;
	}

	template<typename CharType>
	Int32 StringCompare(const CharType* pString1, const CharType* pString2)
	{
		return false;
	}

	template<typename CharType>
	UInt32 StringHash(const CharType* pString)
	{
		return false;
	}

	template<typename _CharType>
	class StringBase
	{
	public:
		using StringType	= StringBase<_CharType>;
		using CharType		= typename _CharType;
		
	protected:
		struct StringMeta
		{
			UInt32 count;
			UInt32 length;

			StringMeta() :
				count(1), length(0) { }

			StringMeta(UInt32 count, UInt32 length) :
				count(count), length(length) { }
		};

		static constexpr UInt32 charSize = sizeof(CharType);
		static constexpr UInt32 metaSize = sizeof(StringMeta);

		union
		{
			Byte*		mpData;
			StringMeta* mpMeta;
		};

		static StringType Append(const StringType& string1, const CharType* pString2)
		{
			StringType result;
			UInt32 length = StringLength(pString2);
			result.Resize(string1.Length() + length);
			memcpy(result.Data(), string1.Str(), string1.Length() * sizeof(CharType));
			memcpy(result.Data() + string1.Length(), pString2, length * sizeof(CharType));
			return result;
		}

		static StringType Append(const CharType* pString1, const StringType& string2)
		{
			StringType result;
			UInt32 length = StringLength(pString1);
			result.Resize(length + string2.Length());
			memcpy(result.Data(), pString1, length * sizeof(CharType));
			memcpy(result.Data() + length, string2.Str(), string2.Length() * sizeof(CharType));
			return result;
		}

		static StringType Append(const StringType& string1, const StringType& string2)
		{
			StringType result;
			result.Resize(string1.Length() + string2.Length());
			memcpy(result.Data(), string1.Str(), string1.Length() * sizeof(CharType));
			memcpy(result.Data() + string1.Length(), string2.Str(), string2.Length() * sizeof(CharType));
			return result;
		}

	public:
		StringBase() :
			mpMeta(new StringMeta()) { }

		StringBase(const CharType* pString)
		{
			const UInt32 length = StringLength(pString);
			const UInt32 stringBufferSize = (length + 1) * charSize;
			const UInt32 fullBufferSize = metaSize + stringBufferSize;

			mpData = new Byte[fullBufferSize];

			*mpMeta = StringMeta(1, length);
			memcpy(mpData + metaSize, pString, stringBufferSize);

			// Set last value to zero (null-termination)
			reinterpret_cast<CharType*>(mpData + metaSize)[length] = 0;
		}

		StringBase(const StringType& string) :
			mpData(string.mpData)
		{
			++mpMeta->count;
		}

		StringBase(StringType&& rString) noexcept :
			StringType()
		{
			Swap(*this, rString);
		}

		~StringBase()
		{
			if (--mpMeta->count == 0)
			{
				delete mpData;
			}
		}

		friend void Swap(StringType& string1, StringType& string2)
		{
			using Quartz::Swap;
			Swap(string1.mpData, string2.mpData);
		}

		Bool8 operator==(const StringType& string) const
		{
			return (Length() == string.Length()) && 
				(StringCompare(Str(), string.Str()) == 0);
		}

		StringType& operator=(StringType string)
		{
			Swap(*this, string);
			return *this;
		}

		StringType operator+(const CharType* pString) const
		{
			return Append(*this, pString);
		}

		StringType operator+(const StringType& string) const
		{
			return Append(*this, string);
		}

		friend StringType operator+(const CharType* pString1, const StringType& string2)
		{
			return Append(pString1, string2);
		}

		StringType& operator+=(const CharType* pString)
		{
			*this = Append(*this, pString);
			return *this;
		}

		StringType& operator+=(const StringType& string)
		{
			*this = Append(*this, string);
			return *this;
		}

		StringType& Resize(UInt32 length)
		{
			const UInt32 stringBufferSize = (length + 1) * charSize;
			const UInt32 fullBufferSize = metaSize + stringBufferSize;

			const Byte* mpPrev = mpData;

			mpData = new Byte[fullBufferSize];
			mpMeta->count = ((StringMeta*)mpPrev)->count;
			mpMeta->length = length;

			reinterpret_cast<CharType*>(mpData + metaSize)[length] = 0;

			delete mpPrev;

			return *this;
		}

		const UInt32 Hash() const
		{
			return StringHash(Str());
		}

		const CharType* Str() const
		{
			return reinterpret_cast<const CharType*>(mpData + metaSize);
		}

		CharType* Data()
		{
			return reinterpret_cast<CharType*>(mpData + metaSize);
		}

		UInt32 Length() const
		{
			return mpMeta->length;
		}

		UInt32 RefCount() const
		{
			return mpMeta->count;
		}

		Bool8 IsUnique() const
		{
			return mpMeta->count == 1;
		}

		Bool8 IsEmpty() const
		{
			return mpMeta->length == 0;
		}
	};

	template<>
	FORCE_INLINE UInt32 StringLength<char>(const char* pString)
	{
		return static_cast<UInt32>(strlen(pString));
	}

	template<>
	FORCE_INLINE UInt32 StringLength<wchar_t>(const wchar_t* pString)
	{
		return static_cast<UInt32>(wcslen(pString));
	}

	template<>
	FORCE_INLINE Int32 StringCompare<char>(const char* pString1, const char* pString2)
	{
		return static_cast<UInt32>(strcmp(pString1, pString2));
	}

	template<>
	FORCE_INLINE Int32 StringCompare<wchar_t>(const wchar_t* pString1, const wchar_t* pString2)
	{
		return static_cast<UInt32>(wcscmp(pString1, pString2));
	}

	template<>
	FORCE_INLINE UInt32 StringHash<char>(const char* pString)
	{
		// MurmurOAAT64 Hash

		UInt64 hash = 525201411107845655ull;

		while (*(pString++))
		{
			hash ^= *pString;
			hash *= 0x5bd1e9955bd1e995;
			hash ^= hash >> 47;
		}

		return static_cast<UInt32>(hash);
	}

	template<>
	FORCE_INLINE UInt32 StringHash<wchar_t>(const wchar_t* pString)
	{
		// MurmurOAAT64 Hash

		UInt64 hash = 525201411107845655ull;

		while (*(pString++))
		{
			hash ^= *pString;
			hash *= 0x5bd1e9955bd1e995;
			hash ^= hash >> 47;
		}

		return static_cast<UInt32>(hash);
	}

	using StringA = StringBase<char>;
	using StringW = StringBase<wchar_t>;

	using String = StringA;

	template<>
	FORCE_INLINE UInt32 Hash<String>(const String& value)
	{
		return value.Hash();
	}

	template<>
	FORCE_INLINE UInt32 Hash<StringW>(const StringW& value)
	{
		return value.Hash();
	}

	FORCE_INLINE StringW StringAToStringW(const StringA& stringA)
	{
		StringW wide;
		wide.Resize(stringA.Length());

		const char* pStr = stringA.Str();
		wchar_t* pWide = wide.Data();

		while (*pStr)
		{
			*pWide = (wchar_t)*pStr;
			++pWide;
			++pStr;
		}

		return wide;
	}
}