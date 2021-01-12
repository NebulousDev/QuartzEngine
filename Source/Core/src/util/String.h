#pragma once

#include "../Common.h"
#include "Hash.h"

#include <cstring>
#include <cassert>

namespace Quartz
{
	template<typename CharType>
	USize StringLength(const CharType* pString)
	{
		return 0;
	}

	template<typename CharType>
	USize StringCompare(const CharType* pString1, const CharType* pString2)
	{
		return false;
	}

	template<typename CharType>
	USize StringCompareCount(const CharType* pString1, const CharType* pString2, USize count)
	{
		return false;
	}

	template<typename CharType>
	UInt32 StringHash(const CharType* pString)
	{
		return false;
	}

	template<typename _CharType>
	class SubStringBase;

	template<typename _CharType>
	class StringBase
	{
	public:
		using StringType = StringBase<_CharType>;
		using SubStringType = SubStringBase<_CharType>;
		using CharType = typename _CharType;

		friend SubStringType;

	protected:
		struct StringMeta
		{
			USize count;
			USize length;

			StringMeta() :
				count(1), length(0) { }

			StringMeta(USize count, USize length) :
				count(count), length(length) { }
		};

		static constexpr USize charSize = sizeof(CharType);
		static constexpr USize metaSize = sizeof(StringMeta);

		union
		{
			Byte* mpData;
			StringMeta* mpMeta;
		};

		static StringType Append(const StringType& string1, const CharType* pString2)
		{
			StringType result;
			USize length = StringLength(pString2);
			result.Resize(string1.Length() + length);
			memcpy(result.Data(), string1.Str(), string1.Length() * sizeof(CharType));
			memcpy(result.Data() + string1.Length(), pString2, length * sizeof(CharType));
			return result;
		}

		static StringType Append(const CharType* pString1, const StringType& string2)
		{
			StringType result;
			USize length = StringLength(pString1);
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

		StringBase(const StringType& string)
			: mpData(string.mpData)
		{
			++mpMeta->count;
		}

		StringBase(StringType&& rString) noexcept
			: StringType()
		{
			Swap(*this, rString);
		}

		StringBase(const CharType* pString)
			: StringBase(pString, StringLength(pString)) { }

		StringBase(const CharType* pString, USize length)
		{
			const USize stringBufferSize = (length + 1) * charSize;
			const USize fullBufferSize = metaSize + stringBufferSize;

			mpData = new Byte[fullBufferSize];

			*mpMeta = StringMeta(1, length);
			memcpy(mpData + metaSize, pString, stringBufferSize);

			// Set last value to zero (null-termination)
			reinterpret_cast<CharType*>(mpData + metaSize)[length] = 0;
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

		Bool8 operator==(const SubStringType& substring) const
		{
			return (Length() == substring.Length()) &&
				(StringCompare(Str(), substring.Str()) == 0);
		}

		Bool8 operator==(const CharType* pString) const
		{
			return StringCompare(Str(), pString) == 0;
		}

		Bool8 operator!=(const StringType& string) const
		{
			return !operator==(string);
		}

		Bool8 operator!=(const SubStringType& substring) const
		{
			return !operator==(substring);
		}

		Bool8 operator!=(const CharType* pString) const
		{
			return !operator==(pString);
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

		StringType& Resize(USize length)
		{
			const USize stringBufferSize = (length + 1) * charSize;
			const USize fullBufferSize = metaSize + stringBufferSize;

			const Byte* mpPrev = mpData;

			mpData = new Byte[fullBufferSize];
			mpMeta->count = ((StringMeta*)mpPrev)->count;
			mpMeta->length = length;

			reinterpret_cast<CharType*>(mpData + metaSize)[length] = 0;

			delete mpPrev;

			return *this;
		}

		USize Hash() const
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

		USize Length() const
		{
			return mpMeta->length;
		}

		USize RefCount() const
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

	template<typename _CharType>
	class SubStringBase
	{
	public:
		using StringType = StringBase<_CharType>;
		using SubStringType = SubStringBase<_CharType>;
		using CharType = typename _CharType;

	protected:
		USize	mSubLength;
		Byte* mpData;

		// By keeping a copy of the original,
		// we guarantee the data is not deleted until
		// all substrings are destroyed
		StringType	mSrcString;

	public:
		SubStringBase()
			: mSubLength(0), mpData(nullptr), mSrcString() {}

		SubStringBase(const SubStringType& substring)
			: mSubLength(substring.mSubLength), mpData(substring.mpData), mSrcString(substring.mSrcString) {}

		SubStringBase(SubStringType&& rSubstring) noexcept :
			SubStringType()
		{
			Swap(*this, rSubstring);
		}

		SubStringBase(StringType& string, USize start, USize end)
			: mSrcString(string)
		{
			assert(end >= start && "Substring end index is greater than start index!");
			assert(start <= string.Length() && end <= string.Length() && "Substring is out of bounds!");

			mSubLength = end - start;
			mpData = reinterpret_cast<Byte*>(string.Data() + start);
		}

		SubStringBase(SubStringType& substring, USize start, USize end)
			: mSrcString(substring.mSrcString)
		{
			assert(end >= start && "Substring end index is greater than start index!");
			assert(start <= substring.Length() && end <= substring.Length() && "Substring is out of bounds!");

			mSubLength = end - start;
			mpData = reinterpret_cast<Byte*>(substring.Data() + start);
		}

		friend void Swap(SubStringType& substring1, SubStringType& substring2)
		{
			using Quartz::Swap;
			Swap(substring1.mSubLength, substring2.mSubLength);
			Swap(substring1.mpData, substring2.mpData);
			Swap(substring1.mSrcString, substring2.mSrcString);
		}

		Bool8 operator==(const StringType& string) const
		{
			return (mSubLength == string.Length()) &&
				(StringCompareCount(Str(), string.Str(), mSubLength) == 0);
		}

		Bool8 operator==(const SubStringType& substring) const
		{
			return (mSubLength == substring.mSubLength) &&
				(StringCompareCount(Str(), substring.Str(), mSubLength) == 0);
		}

		Bool8 operator==(const CharType* pString) const
		{
			return StringCompareCount(Str(), pString, mSubLength) == 0;
		}

		Bool8 operator!=(const StringType& string) const
		{
			return !operator==(string);
		}

		Bool8 operator!=(const SubStringType& substring) const
		{
			return !operator==(substring);
		}

		Bool8 operator!=(const CharType* pString) const
		{
			return !operator==(pString);
		}

		SubStringType& operator=(SubStringType substring)
		{
			Swap(*this, substring);
			return *this;
		}

		operator StringType()
		{
			return String(Str(), mSubLength);
		}

		USize Hash() const
		{
			return StringHash(Str());
		}

		const CharType* Str() const
		{
			return reinterpret_cast<const CharType*>(mpData);
		}

		CharType* Data()
		{
			return reinterpret_cast<CharType*>(mpData);
		}

		USize Length() const
		{
			return mSubLength;
		}
	};

	template<>
	FORCE_INLINE USize StringLength<char>(const char* pString)
	{
		return static_cast<USize>(strlen(pString));
	}

	template<>
	FORCE_INLINE USize StringLength<wchar_t>(const wchar_t* pString)
	{
		return static_cast<USize>(wcslen(pString));
	}

	template<>
	FORCE_INLINE USize StringCompare<char>(const char* pString1, const char* pString2)
	{
		return static_cast<USize>(strcmp(pString1, pString2));
	}

	template<>
	FORCE_INLINE USize StringCompare<wchar_t>(const wchar_t* pString1, const wchar_t* pString2)
	{
		return static_cast<USize>(wcscmp(pString1, pString2));
	}

	template<>
	FORCE_INLINE USize StringCompareCount<char>(const char* pString1, const char* pString2, USize count)
	{
		return static_cast<USize>(strncmp(pString1, pString2, count));
	}

	template<>
	FORCE_INLINE USize StringCompareCount<wchar_t>(const wchar_t* pString1, const wchar_t* pString2, USize count)
	{
		return static_cast<USize>(wcsncmp(pString1, pString2, count));
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
	using SubStringA = SubStringBase<char>;
	using SubStringW = SubStringBase<wchar_t>;

	using String = StringA;
	using SubString = SubStringA;

	template<>
	FORCE_INLINE UInt32 Hash<String>(const String& value)
	{
		return static_cast<UInt32>(value.Hash());
	}

	template<>
	FORCE_INLINE UInt32 Hash<StringW>(const StringW& value)
	{
		return static_cast<UInt32>(value.Hash());
	}

	// @Todo: Speed up
	FORCE_INLINE StringW StringAToStringW(const StringA& stringA)
	{
		StringW wide;
		wide.Resize(stringA.Length());

		char* pStr = const_cast<StringA&>(stringA).Data();
		wchar_t* pWide = wide.Data();

		while (*pStr)
		{
			*pWide = (wchar_t)*pStr;
			++pWide;
			++pStr;
		}

		return wide;
	}

	// @Todo: Speed up
	FORCE_INLINE StringA StringWToStringA(const StringW& stringW)
	{
		StringA ascii;
		ascii.Resize(stringW.Length());

		wchar_t* pStr = const_cast<StringW&>(stringW).Data();
		char* pAscii = ascii.Data();

		while (*pStr)
		{
			*pAscii = *reinterpret_cast<char*>(pStr);
			++pAscii;
			++pStr;
		}

		return ascii;
	}

	FORCE_INLINE String operator"" _STRING(const char* str, USize size)
	{
		return String(str, static_cast<USize>(size));
	}

	FORCE_INLINE StringW operator"" _STRINGW(const wchar_t* str, USize size)
	{
		return StringW(str, static_cast<USize>(size));
	}

#define STRING(charstr) charstr##_STRING
#define STRINGW(wcharstr) wcharstr##_STRINGW
}