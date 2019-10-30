#include "Memory.h"

#include <cstring>

namespace Quartz
{
	void* MemCopy(const void* const pSrc, void* const pDest, const UInt64 size)
	{
		/*
#if defined(WIN64)
		if (!pSrc || !pDest || !size)
			return nullptr;

		register UInt8* pSrcInc = (UInt8*)pSrc;
		register UInt8* pDestInc = (UInt8*)pDest;

		UInt64 count = (size / sizeof(UInt64));
		UInt64 remainder = (size % sizeof(UInt64)); // TODO: Use bit ops
		
		for (UInt64 i = 0; i < count; ++i)
		{
			*(UInt64*)pDestInc = *(UInt64*)pSrcInc;
			pSrcInc += sizeof(UInt64);
			pDestInc += sizeof(UInt64);
		}

		for (UInt64 i = 0; i < remainder; ++i)
			*pDestInc++ = *pSrcInc++;

		return pDest;
#else
		if (!pSrc || !pDest || !size)
			return nullptr;

		register UInt8* pSrcInc = (UInt8*)pSrc;
		register UInt8* pDestInc = (UInt8*)pDest;

		UInt64 count = (size / sizeof(UInt32));
		UInt64 remainder = (size % sizeof(UInt32)); // TODO: Use bit ops

		for (UInt64 i = 0; i < count; ++i)
		{
			*(UInt32*)pDestInc = *(UInt32*)pSrcInc;
			pSrcInc += sizeof(UInt32);
			pDestInc += sizeof(UInt32);
		}

		for (UInt64 i = 0; i < remainder; ++i)
			*pDestInc++ = *pSrcInc++;

		return pDest;
#endif
		*/

		return memcpy(pDest, pSrc, size);
	}

	void* MemMove(const void* const pSrc, void* const pDest, const UInt64 size)
	{
#if defined(WIN64)
		if (!pSrc || !pDest || !size)
			return nullptr;

		register UInt8* pSrcInc = (UInt8*)pSrc;
		register UInt8* pDestInc = (UInt8*)pDest;

		UInt64 count = (size / sizeof(UInt64));
		UInt64 remainder = (size % sizeof(UInt64)); // TODO: Use bit ops

		if ((UInt64)pDest < (UInt64)pSrc)
		{
			for (UInt64 i = 0; i < count; ++i)
			{
				*(UInt64*)pDestInc = *(UInt64*)pSrcInc;
				pSrcInc += sizeof(UInt64);
				pDestInc += sizeof(UInt64);
			}

			for (UInt64 i = 0; i < remainder; ++i)
				*pSrcInc++ = *pDestInc++;
		}

		else
		{
			pSrcInc += size - sizeof(UInt64);
			pDestInc += size - sizeof(UInt64);

			for (UInt64 i = 0; i < count; ++i)
			{
				*(UInt64*)pDestInc = *(UInt64*)pSrcInc;
				pSrcInc -= sizeof(UInt64);
				pDestInc -= sizeof(UInt64);
			}

			for (UInt64 i = 0; i < remainder; ++i)
				*pDestInc-- = *pSrcInc--;
		}

		return pDest;
#else
		if (!pSrc || !pDest || !size)
			return nullptr;

		register UInt8* pSrcInc = (UInt8*)pSrc;
		register UInt8* pDestInc = (UInt8*)pDest;

		UInt64 count = (size / sizeof(UInt32));
		UInt64 remainder = (size % sizeof(UInt32)); // TODO: Use bit ops

		if ((UInt64)pDest < (UInt64)pSrc)
		{
			for (UInt64 i = 0; i < count; ++i)
			{
				*(UInt32*)pDestInc = *(UInt32*)pSrcInc;
				pSrcInc += sizeof(UInt32);
				pDestInc += sizeof(UInt32);
			}

			for (UInt64 i = 0; i < remainder; ++i)
				*pSrcInc++ = *pDestInc++;
		}

		else
		{
			pSrcInc += size - sizeof(UInt32);
			pDestInc += size - sizeof(UInt32);

			for (UInt64 i = 0; i < count; ++i)
			{
				*(UInt32*)pDestInc = *(UInt32*)pSrcInc;
				pSrcInc -= sizeof(UInt32);
				pDestInc -= sizeof(UInt32);
			}

			for (UInt64 i = 0; i < remainder; ++i)
				*pDestInc-- = *pSrcInc--;
		}

		return pDest;
#endif
	}

	void* AlignForward(void * pPtr, UInt8 alignment)
	{
		return (void*)((reinterpret_cast<UInt64>(pPtr) + (alignment - 1)) & ~(alignment - 1));
	}

	UInt8 GetAdjustment(void * pPtr, UInt8 alignment)
	{
		if (alignment == 0) return 0;
		UInt8 adjustment = alignment - (reinterpret_cast<UInt64>(pPtr) & static_cast<UInt64>(alignment - 1));
		return adjustment == alignment ? 0 : adjustment;
	}

	UInt64 VECTOR_CALL StrLen(const char* pStr)
	{
		XMMVECi* pXmmStr = reinterpret_cast<XMMVECi*>(const_cast<char*>(pStr));
		const XMMVECi xmmZeros = _mm_setzero_si128();
		const UInt8 immMode = _SIDD_UBYTE_OPS | _SIDD_CMP_EQUAL_EACH | _SIDD_LEAST_SIGNIFICANT;

		UInt64 length = 0;

		while (true)
		{
			const XMMVECi xmmStr = _mm_loadu_si128(pXmmStr);

			if (_mm_cmpistrc(xmmStr, xmmZeros, immMode))
			{
				const Int32 idx = _mm_cmpistri(xmmStr, xmmZeros, immMode);
				return length + idx;
			}

			++pXmmStr; 
			length += 16;
		}
	}

	UInt64 VECTOR_CALL StrLen16(const char16_t* pStr)
	{
		return 0;
	}

	UInt64 VECTOR_CALL StrLen32(const char32_t* pStr)
	{
		return 0;
	}

	UInt32 VECTOR_CALL CountLeadingZeros(UInt32 value)
	{
		unsigned long leading_zero = 0;

		if (_BitScanReverse(&leading_zero, value))
			return 31 - leading_zero;

		return 32;
	}

	const char* VECTOR_CALL StrStr(const char* pStr, const char* pStr2)
	{
		return strstr(pStr, pStr2);
	}

	UInt64 VECTOR_CALL StrStr(const char* pStr, UInt64 len1, const char* pStr2, UInt64 len2)
	{
		/*
		const UInt8 immMode = _SIDD_UBYTE_OPS | _SIDD_BIT_MASK | _SIDD_CMP_EQUAL_EACH | _SIDD_NEGATIVE_POLARITY;

		XMMVECi* pXmmSearch = reinterpret_cast<XMMVECi*>(const_cast<char*>(pStr));
		XMMVECi* pXmmTarget = reinterpret_cast<XMMVECi*>(const_cast<char*>(pStr2));

		while ((UInt64)pXmmSearch < (UInt64)(pStr + len1))
		{
			const XMMVECi xmmSearch = _mm_loadu_si128(pXmmSearch);
			const XMMVECi xmmTarget = _mm_loadu_si128(pXmmTarget);

			if (!_mm_cmpistrc(xmmTarget, xmmSearch, immMode))
				pXmmTarget++;
			else
				pXmmTarget = reinterpret_cast<XMMVECi*>(const_cast<char*>(pStr2));

			++pXmmSearch;
		}

		return 0;

		//

		const __m128i prefix = _mm_loadu_si128(reinterpret_cast<const __m128i*>(pStr2));
		const __m128i zeros = _mm_setzero_si128();

		for (size_t i = 0; i < len1; i += 8) {

			const __m128i data = _mm_loadu_si128(reinterpret_cast<const __m128i*>(pStr + i));
			const __m128i result = _mm_mpsadbw_epu8(data, prefix, 0);

			const __m128i cmp = _mm_cmpeq_epi16(result, zeros);

			unsigned mask = _mm_movemask_epi8(cmp) & 0x5555;

			while (mask != 0) {

				const auto bitpos = clz(mask) / 2;

				if (memcmp(pStr + i + bitpos + 4, pStr2 + 4, len2 - 4) == 0) {
					return i + bitpos;
				}

				mask = mask & (mask - 1);
			}
		}

		return -1;
		*/

		const char* pOffset = strstr(pStr, pStr2);
		return pOffset ? pOffset - pStr : -1;
	}

	const char16_t* VECTOR_CALL StrStr16(const char16_t* pStr, const char16_t* pStr2)
	{
		return 0;
	}

	UInt64 VECTOR_CALL StrStr16(const char16_t* pStr, UInt64 len1, const char16_t* pStr2, UInt64 len2)
	{
		return 0;
	}

	const char32_t* VECTOR_CALL StrStr32(const char32_t* pStr, const char32_t* pStr2)
	{
		return 0;
	}

	UInt64 VECTOR_CALL StrStr32(const char32_t* pStr, UInt64 len1, const char32_t * pStr2, UInt64 len2)
	{
		return 0;
	}

	UInt64 VECTOR_CALL StrCmp(const char* pStr1, const char* pStr2)
	{
		return strcmp(pStr1, pStr2);
	}

	UInt64 VECTOR_CALL StrCmp16(const char16_t* pStr1, const char16_t* pStr2)
	{
		return 0;
	}

	UInt64 VECTOR_CALL StrCmp32(const char32_t* pStr1, const char32_t* pStr2)
	{
		return 0;
	}

	void VECTOR_CALL StrCpy(char* pDest, UInt64 size, const char* pStr)
	{
		strcpy_s(pDest, (rsize_t)size, pStr);
	}

	void VECTOR_CALL StrCat(char * pDest, UInt64 size, const char * pStr)
	{
		strcat_s(pDest, (rsize_t)size, pStr);
	}

	UInt32 VECTOR_CALL CRC32(const char* pStr)
	{
		UInt32 crc = 0xFFFFFFFF;

		while (*pStr)
			crc = _mm_crc32_u8(crc, *pStr++);

		return crc ^ 0xFFFFFFFF;
	}

	UInt32 VECTOR_CALL SDBM(const char *str)
	{
		UInt32 hash = 0;

		while (UInt32 c = *str++)
			hash = c + (hash << 6) + (hash << 16) - hash;

		return hash;
	}
}