#pragma once
#include "../Types.h"
#include "../util/Intrinsics.h"

namespace Quartz
{
	//void* MemSet(const void* const pSrc, const UInt64 size, const char value);

	void* MemCopy(const void* const pSrc, void* const pDest, const UInt64 size);

	void* MemMove(const void* const pSrc, void* const pDest, const UInt64 size);

	void* AlignForward(void* pPtr, UInt8 alignment);

	UInt8 GetAdjustment(void* pPtr, UInt8 alignment);

	UInt64 VECTOR_CALL StrLen(const char* pStr);

	UInt64 VECTOR_CALL StrLen16(const char16_t* pStr);

	UInt64 VECTOR_CALL StrLen32(const char32_t* pStr);

	const char* VECTOR_CALL StrStr(const char* pStr, const char* pStr2);

	UInt64 VECTOR_CALL StrStr(const char* pStr, UInt64 len1, const char* pStr2, UInt64 len2);

	const char16_t* VECTOR_CALL StrStr16(const char16_t* pStr, const char16_t* pStr2);

	UInt64 VECTOR_CALL StrStr16(const char16_t* pStr, UInt64 len1, const char16_t* pStr2, UInt64 len2);

	const char32_t* VECTOR_CALL StrStr32(const char32_t* pStr, const char32_t* pStr2);

	UInt64 VECTOR_CALL StrStr32(const char32_t* pStr, UInt64 len1, const char32_t* pStr2, UInt64 len2);

	UInt64 VECTOR_CALL StrCmp(const char* pStr1, const char* pStr2);

	UInt64 VECTOR_CALL StrCmp16(const char16_t* pStr1, const char16_t* pStr2);

	UInt64 VECTOR_CALL StrCmp32(const char32_t* pStr1, const char32_t* pStr2);

	void VECTOR_CALL StrCpy(char* pDest, UInt64 size, const char* pStr);

	void VECTOR_CALL StrCat(char* pDest, UInt64 size, const char* pStr);

	UInt32 VECTOR_CALL CRC32(const char* pStr);

	UInt32 VECTOR_CALL SDBM(const char *str);
}