#pragma once

#include "String.h"

namespace Quartz
{
	template<typename CharType>
	constexpr CharType END_OF_LINE = '\n';

	template<typename CharType>
	constexpr CharType END_OF_STRING = '\0';

	template<typename CharType>
	constexpr CharType TAB_SYMBOL = '\t';

	template<typename CharType>
	constexpr CharType SPACE_SYMBOL = ' ';

	template<typename CharType>
	Float32 ReadFloat2(CharType* pStr, CharType** ppEnd)
	{
		return 0.0f;
	}

	template<typename CharType>
	Int64 ReadInt2(CharType* pStr, CharType** ppEnd)
	{
		return 0;
	}

	template<typename _CharType>
	class StringParserBase
	{
	public:
		using StringType = StringBase<_CharType>;
		using SubStringType = SubStringBase<_CharType>;
		using CharType = typename _CharType;

	private:
		CharType*		mpHead;
		SubStringType	mSubString;

		UInt32 IndexOf(CharType* pChar)
		{
			return pChar - mSubString.Str();
		}

		Bool8 IsEnd(CharType* pChar)
		{
			return pChar == (mSubString.Str() + mSubString.Length());
		}

	public:
		explicit StringParserBase(const StringType& string)
			: mSubString(const_cast<StringType&>(string), 0, string.Length()), mpHead(const_cast<StringType&>(string).Data()) { }

		explicit StringParserBase(const SubStringType& substring)
			: mSubString(substring), mpHead(const_cast<SubStringType&>(substring).Data()) { }

		CharType PeakChar()
		{
			return *mpHead;
		}

		SubStringType ReadToChar(CharType value)
		{
			CharType* mpRead = mpHead;
			for (; *mpRead != value && !IsEnd(mpRead); ++mpRead);

			SubStringType result(mSubString, IndexOf(mpHead), IndexOf(mpRead));

			mpHead = mpRead;

			return result;
		}

		SubStringType ReadLine()
		{
			SubStringType result = ReadToChar(END_OF_LINE<CharType>);

			// Skip '\n'
			AdvanceChar();

			return result;
		}

		Float32 ReadFloat()
		{
			CharType* mpRead = nullptr;
			Float32 value = ReadFloat2<CharType>(mpHead, &mpRead);

			mpHead = mpRead;

			return value;
		}

		Int32 ReadInt()
		{
			CharType* mpRead = nullptr;
			Int64 value = ReadInt2<CharType>(mpHead, &mpRead);

			mpHead = mpRead;

			return (Int32)value;
		}

		Int64 ReadLong()
		{
			CharType* mpRead = nullptr;
			Int32 value = ReadInt2<CharType>(mpHead, &mpRead);

			mpHead = mpRead;

			return value;
		}

		void AdvanceChar()
		{
			if (!IsEnd())
			{
				++mpHead;
			}
		}

		void AdvanceWhitespace()
		{
			for (; (*mpHead == SPACE_SYMBOL<CharType> || *mpHead == TAB_SYMBOL<CharType>) && !IsEnd(); ++mpHead);
		}

		Bool8 IsEnd()
		{
			return IsEnd(mpHead);
		}
	};

	template<> constexpr char END_OF_LINE<char> = '\n';
	template<> constexpr wchar_t END_OF_LINE<wchar_t> = L'\n';

	template<> constexpr char END_OF_STRING<char> = '\0';
	template<> constexpr wchar_t END_OF_STRING<wchar_t> = L'\0';

	template<> constexpr char TAB_SYMBOL<char> = '\t';
	template<> constexpr wchar_t TAB_SYMBOL<wchar_t> = L'\t';

	template<> constexpr char SPACE_SYMBOL<char> = ' ';
	template<> constexpr wchar_t SPACE_SYMBOL<wchar_t> = L' ';

	template<>
	FORCE_INLINE Float32 ReadFloat2<char>(char* pStr, char** ppEnd)
	{
		return strtof(pStr, ppEnd);
	}

	template<>
	FORCE_INLINE Float32 ReadFloat2<wchar_t>(wchar_t* pStr, wchar_t** ppEnd)
	{
		return wcstof(pStr, ppEnd);
	}

	template<>
	FORCE_INLINE Int64 ReadInt2<char>(char* pStr, char** ppEnd)
	{
		return strtol(pStr, ppEnd, 0);
	}

	template<>
	FORCE_INLINE Int64 ReadInt2<wchar_t>(wchar_t* pStr, wchar_t** ppEnd)
	{
		return wcstol(pStr, ppEnd, 0);
	}

	using StringParserA = StringParserBase<char>;
	using StringParserW = StringParserBase<wchar_t>;

	using StringParser = StringParserA;
}