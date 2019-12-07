#pragma once
#include "../Common.h"

#include "Hash.h"
#include "../memory/Memory.h"

#include <utility>

#define STR(str) str##_S
#define STR8(str) str##_S8
#define STR16(str) str##_S16
#define STR32(str) str##_S32

namespace Quartz
{
	using UTF8Char = char;
	using UTF16Char = char16_t;
	using UTF32Char = char32_t;

#define STRING_IMPL_HEADER \
template \
< \
	typename CharType, \
	UInt32(*StrLenFunc)(const CharType *), \
	const CharType *(*StrFindFunc)(const CharType *, const CharType *), \
	UInt32(*StrCmpFunc)(const CharType *, const CharType *) \
>

#define STRING_IMPL_TYPE \
	StringImpl<CharType, StrLenFunc, StrFindFunc, StrCmpFunc>

	STRING_IMPL_HEADER
	class StringImpl
	{
	public:
		using CharValue = CharType;

	private:
		CharType* mpStr;
		UInt32* mpLength;
		UInt32* mpCount;

		StringImpl(const CharType* pStr, UInt32 len, 
			const CharType* pStr2, UInt32 len2);

	public:
		StringImpl();
		StringImpl(const CharType* pStr);
		StringImpl(const CharType* pStr, UInt32 length);
		StringImpl(const StringImpl& str);
		StringImpl(StringImpl&& str);

		~StringImpl();

		const CharType* Find(const CharType* pStr) const;
		const CharType* Find(const StringImpl& str) const;

		Int32 IndexOf(const CharType* pStr) const;
		Int32 IndexOf(const StringImpl& str) const;

		Bool8 StartsWith(const CharType* pStr) const;
		Bool8 StartsWith(const StringImpl& str) const;
		Bool8 EndsWith(const CharType* pStr) const;
		Bool8 EndsWith(const StringImpl& str) const;

		StringImpl SubString(UInt32 begin, UInt32 end) const;

		UInt32 Compare(const CharType* pStr) const;
		UInt32 Compare(const StringImpl& str) const;

		Bool8 operator==(const CharType* pStr) const;
		Bool8 operator==(const StringImpl& str) const;
		Bool8 operator!=(const CharType* pStr) const;
		Bool8 operator!=(const StringImpl& str) const;

		StringImpl& operator=(const CharType* pStr);
		StringImpl& operator=(StringImpl str);

		StringImpl operator+(const CharType* pStr) const;
		StringImpl operator+(const StringImpl& str) const;
		StringImpl& operator+=(const CharType* pStr);
		StringImpl& operator+=(const StringImpl& str);

		const CharType& operator[](const UInt32 idx) const { return mpStr[idx]; };

		FORCE_INLINE CharType* Data() { return mpStr; }
		FORCE_INLINE const CharType* Str() const { return mpStr; }
		FORCE_INLINE UInt32 Length() const { return *mpLength; }
		FORCE_INLINE UInt32 Count() const { return *mpCount; }
		FORCE_INLINE Bool8 Empty() const { return *mpLength == 0; }

		friend void swap(StringImpl& first, StringImpl& second)
		{
			using std::swap;

			swap(first.mpStr, second.mpStr);
			swap(first.mpLength, second.mpLength);
			swap(first.mpCount, second.mpCount);
		}
	};

	STRING_IMPL_HEADER
	STRING_IMPL_TYPE::StringImpl(const CharType* pStr, UInt32 len, 
		const CharType* pStr2, UInt32 len2)
	{
		const UInt32 length = len + len2;

		mpStr = new CharType[length + 1];
		MemCopy(pStr, mpStr, len);
		MemCopy(pStr2, mpStr + len, len2);
		mpStr[length] = '\0';

		mpLength = new UInt32(length);
		mpCount = new UInt32(1);
	}

	STRING_IMPL_HEADER
	STRING_IMPL_TYPE::StringImpl()
		: mpStr(nullptr), mpLength(new UInt32(0)), mpCount(new UInt32(1))
	{
		// Nothing
	}

	STRING_IMPL_HEADER
	STRING_IMPL_TYPE::StringImpl(const CharType* pStr)
	{
		const UInt32 length = StrLenFunc(pStr);

		mpStr = new CharType[length + 1];
		MemCopy(pStr, mpStr, length);
		mpStr[length] = '\0';

		mpLength = new UInt32(length);
		mpCount = new UInt32(1);
	}

	STRING_IMPL_HEADER
	STRING_IMPL_TYPE::StringImpl(const CharType* pStr, UInt32 length)
	{
		mpStr = new CharType[length + 1];
		MemCopy(pStr, mpStr, length);
		mpStr[length] = '\0';

		mpLength = new UInt32(length);
		mpCount = new UInt32(1);
	}

	STRING_IMPL_HEADER
	STRING_IMPL_TYPE::StringImpl(const StringImpl& str)
		: mpStr(str.mpStr), mpLength(str.mpLength), mpCount(str.mpCount)
	{
		++(*mpCount);
	}

	STRING_IMPL_HEADER
	STRING_IMPL_TYPE::StringImpl(StringImpl&& str)
		: STRING_IMPL_TYPE()
	{
		swap(*this, str);
	}

	STRING_IMPL_HEADER
	STRING_IMPL_TYPE::~StringImpl()
	{
		if (!(--(*mpCount)))
		{
			delete[] mpStr;
			delete mpLength;
			delete mpCount;
		}
	}

	STRING_IMPL_HEADER
	const CharType* STRING_IMPL_TYPE::Find(const CharType* pStr) const
	{
		return StrFindFunc(mpStr, pStr);
	}

	STRING_IMPL_HEADER
	const CharType* STRING_IMPL_TYPE::Find(const StringImpl& str) const
	{
		return StrFindFunc(mpStr, str.mpStr);
	}

	STRING_IMPL_HEADER
	Int32 STRING_IMPL_TYPE::IndexOf(const CharType* pStr) const
	{
		const CharType* pLoc = Find(pStr);
		return pLoc ? static_cast<UInt32>(pLoc - mpStr) : -1;
	}

	STRING_IMPL_HEADER
	Int32 STRING_IMPL_TYPE::IndexOf(const StringImpl& str) const
	{
		const CharType* pLoc = Find(str);
		return pLoc ? static_cast<UInt32>(pLoc - mpStr) : -1;
	}

	STRING_IMPL_HEADER
	Bool8 STRING_IMPL_TYPE::StartsWith(const CharType* pStr) const
	{
		return Find(pStr) == mpStr;
	}

	STRING_IMPL_HEADER
	Bool8 STRING_IMPL_TYPE::StartsWith(const StringImpl& str) const
	{
		return Find(str) == mpStr;
	}

	STRING_IMPL_HEADER
	Bool8 STRING_IMPL_TYPE::EndsWith(const CharType* pStr) const
	{
		//TODO: Implement reverse find
		return Find(pStr) == mpStr + *mpLength - StrLenFunc(pStr);
	}

	STRING_IMPL_HEADER
	Bool8 STRING_IMPL_TYPE::EndsWith(const StringImpl& str) const
	{
		//TODO: Implement reverse find
		return Find(str) == mpStr + *mpLength - *str.mpLength;
	}

	STRING_IMPL_HEADER
	STRING_IMPL_TYPE STRING_IMPL_TYPE::SubString(UInt32 begin, UInt32 end) const
	{
		assert(end > begin);
		return String(mpStr + begin, end - begin);
	}

	STRING_IMPL_HEADER
	UInt32 STRING_IMPL_TYPE::Compare(const CharType* pStr) const
	{
		return StrCmpFunc(mpStr, pStr);
	}

	STRING_IMPL_HEADER
	UInt32 STRING_IMPL_TYPE::Compare(const StringImpl& str) const
	{
		return StrCmpFunc(mpStr, str.mpStr);
	}

	STRING_IMPL_HEADER
	Bool8 STRING_IMPL_TYPE::operator==(const CharType* pStr) const
	{
		return Compare(pStr) == 0;
	}

	STRING_IMPL_HEADER
	Bool8 STRING_IMPL_TYPE::operator==(const StringImpl& str) const
	{
		return Compare(str) == 0;
	}

	STRING_IMPL_HEADER
		Bool8 STRING_IMPL_TYPE::operator!=(const CharType* pStr) const
	{
		return Compare(pStr) != 0;
	}

	STRING_IMPL_HEADER
		Bool8 STRING_IMPL_TYPE::operator!=(const StringImpl& str) const
	{
		return Compare(str) != 0;
	}

	STRING_IMPL_HEADER
	STRING_IMPL_TYPE& STRING_IMPL_TYPE::operator=(const CharType* pStr)
	{
		return (*this = String(pStr));
	}

	STRING_IMPL_HEADER
	STRING_IMPL_TYPE& STRING_IMPL_TYPE::operator=(StringImpl str)
	{
		swap(*this, str);
		return *this;
	}

	STRING_IMPL_HEADER
	STRING_IMPL_TYPE STRING_IMPL_TYPE::operator+(const CharType* pStr) const
	{
		return StringImpl(mpStr, *mpLength, pStr, StrLenFunc(pStr));
	}

	STRING_IMPL_HEADER
	STRING_IMPL_TYPE STRING_IMPL_TYPE::operator+(const StringImpl& str) const
	{
		return StringImpl(mpStr, *mpLength, str.mpStr, *str.mpLength);
	}

	STRING_IMPL_HEADER
	STRING_IMPL_TYPE& STRING_IMPL_TYPE::operator+=(const CharType* pStr)
	{
		return *this = *this + pStr;
	}

	STRING_IMPL_HEADER
	STRING_IMPL_TYPE& STRING_IMPL_TYPE::operator+=(const StringImpl& str)
	{
		return *this = *this + str;
	}

	FORCE_INLINE UInt32 UTF8StrLen(const UTF8Char* pStr)
	{
		return static_cast<UInt32>(StrLen(pStr));
	}

	FORCE_INLINE UInt32 UTF16StrLen(const UTF16Char* pStr)
	{
		return static_cast<UInt32>(StrLen16(pStr));
	}

	FORCE_INLINE UInt32 UTF32StrLen(const UTF32Char* pStr)
	{
		return static_cast<UInt32>(StrLen32(pStr));
	}

	FORCE_INLINE const UTF8Char* UTF8StrStr(const UTF8Char* pStr, const UTF8Char* pStr2)
	{
		return StrStr(pStr, pStr2);
	}

	FORCE_INLINE const UTF16Char* UTF16StrStr(const UTF16Char* pStr, const UTF16Char* pStr2)
	{
		return StrStr16(pStr, pStr2);
	}

	FORCE_INLINE const UTF32Char* UTF32StrStr(const UTF32Char* pStr, const UTF32Char* pStr2)
	{
		return StrStr32(pStr, pStr2);
	}

	FORCE_INLINE UInt32 UTF8StrCmp(const UTF8Char* pStr, const UTF8Char* pStr2)
	{
		return static_cast<UInt32>(StrCmp(pStr, pStr2));
	}

	FORCE_INLINE UInt32 UTF16StrCmp(const UTF16Char* pStr, const UTF16Char* pStr2)
	{
		return static_cast<UInt32>(StrCmp16(pStr, pStr2));
	}

	FORCE_INLINE UInt32 UTF32StrCmp(const UTF32Char* pStr, const UTF32Char* pStr2)
	{
		return static_cast<UInt32>(StrCmp32(pStr, pStr2));
	}

	using String = StringImpl<UTF8Char, UTF8StrLen, UTF8StrStr, UTF8StrCmp>;
	using String16 = StringImpl<UTF16Char, UTF16StrLen, UTF16StrStr, UTF16StrCmp>;
	using String32 = StringImpl<UTF32Char, UTF32StrLen, UTF32StrStr, UTF32StrCmp>;
	
	FORCE_INLINE String operator"" _S(const char* str, USize size)
	{
		return String(str, static_cast<UInt32>(size));
	}

	FORCE_INLINE String operator"" _S8(const char* str, USize size)
	{
		return String(str, static_cast<UInt32>(size));
	}

	FORCE_INLINE String16 operator"" _S16(const char16_t* str, USize size)
	{
		return String16(str, static_cast<UInt32>(size));
	}

	FORCE_INLINE String32 operator"" _S32(const char32_t* str, USize size)
	{
		return String32(str, static_cast<UInt32>(size));
	}

	template<>
	FORCE_INLINE UInt32 Hash<String>(const String& value)
	{
		return CRC32(value.Str());
	}

	template<>
	FORCE_INLINE UInt32 Hash<String16>(const String16& value)
	{
		return 0;
	}

	template<>
	FORCE_INLINE UInt32 Hash<String32>(const String32& value)
	{
		return 0;
	}
}