#pragma once
#include "../Common.h"
#include "../debug/Debug.h"

#include "Array.h"

#include <utility>

namespace Quartz
{
	class BitField
	{
	private:
		Array<UInt8> mBytes;
		UInt32 mSize;

		static UInt32 CalcByteSize(UInt32 bits);

	public:
		BitField();
		BitField(UInt32 size);
		BitField(const BitField& bitfield);

		void Extend(UInt32 count);
		void Extend(UInt32 index, UInt32 count);

		void Shrink(UInt32 count);
		void Shrink(UInt32 index, UInt32 count);

		Bool8 Get(UInt32 index) const;

		void Set(UInt32 index);
		void Reset(UInt32 index);
		void Flip(UInt32 index);

		FORCE_INLINE UInt32 GetSize() const { return mSize; }
		FORCE_INLINE UInt32 GetByteSize() const { return mBytes.Size(); }
	};

	FORCE_INLINE UInt32 BitField::CalcByteSize(UInt32 bits)
	{
		return (bits / 8) + (bits % 8) > 0 ? 1 : 0;
	}

	BitField::BitField()
		: mSize(0)
	{
		// Nothing
	}

	BitField::BitField(UInt32 size)
		: mBytes(CalcByteSize(size), 0), mSize(size)
	{
		// Nothing
	}

	BitField::BitField(const BitField& bitfield)
	{
		mBytes = bitfield.mBytes;
		mSize = bitfield.mSize;
	}

	void BitField::Extend(UInt32 count)
	{
		mSize += count;
		const UInt32 byteSize = CalcByteSize(mSize);
		if (byteSize > mBytes.Size())
			mBytes.Resize(byteSize);
	}

	void BitField::Extend(UInt32 index, UInt32 count)
	{
		// TODO: DO!
	}

	void BitField::Shrink(UInt32 count)
	{
		mSize -= count;
	}

	void BitField::Shrink(UInt32 index, UInt32 count)
	{
		// TODO: DO!
	}

	Bool8 BitField::Get(UInt32 index) const
	{
		DEBUG_ASSERT(index < mSize);

		const UInt32 byteIndex = index >> 3;
		const UInt32 bitIndex = index - (byteIndex << 3);
		const UInt32 bitMask = 1 << bitIndex;

		return mBytes[byteIndex] & bitMask;
	}

	void BitField::Set(UInt32 index)
	{
		DEBUG_ASSERT(index < mSize);

		const UInt32 byteIndex = index >> 3;
		const UInt32 bitIndex = index - (byteIndex << 3);
		const UInt32 bitMask = 1 << bitIndex;

		mBytes[byteIndex] |= bitMask;
	}

	void BitField::Reset(UInt32 index)
	{
		DEBUG_ASSERT(index < mSize);

		const UInt32 byteIndex = index >> 3;
		const UInt32 bitIndex = index - (byteIndex << 3);
		const UInt32 bitMask = 1 << bitIndex;

		mBytes[byteIndex] &= ~bitMask;
	}

	void BitField::Flip(UInt32 index)
	{
		DEBUG_ASSERT(index < mSize);

		const UInt32 byteIndex = index >> 3;
		const UInt32 bitIndex = index - (byteIndex << 3);
		const UInt32 bitMask = 1 << bitIndex;

		mBytes[byteIndex] ^= bitMask;
	}

	///////////////////////////////////////////////////////////////////////////////

	class BitMatrix
	{
	private:
		Array<BitField> mBitFields;
		UInt32 mHeight;

	public:
		BitMatrix(UInt32 columns, UInt32 rows);
		BitMatrix(const BitMatrix& bitmatrix);
		BitMatrix(BitMatrix&& bitmatrix) noexcept;

		Bool8 Get(UInt32 col, UInt32 row) const;

		void AddColumns(UInt32 index, UInt32 count);
		void AddRows(UInt32 index, UInt32 count);

		void RemoveColumns(UInt32 index, UInt32 count);
		void RemoveRows(UInt32 index, UInt32 count);

		void Set(UInt32 col, UInt32 row);
		void Reset(UInt32 col, UInt32 row);
		void Flip(UInt32 col, UInt32 row);

		FORCE_INLINE UInt32 GetWidth() const { return mBitFields.Size(); }
		FORCE_INLINE UInt32 GetHeight() const { return mHeight; }
	};

	BitMatrix::BitMatrix(UInt32 columns, UInt32 rows)
		: mBitFields(columns, BitField(rows))
	{
		// Nothing
	}

	BitMatrix::BitMatrix(const BitMatrix& bitmatrix)
	{
		mBitFields = bitmatrix.mBitFields;
	}

	BitMatrix::BitMatrix(BitMatrix&& bitmatrix) noexcept
	{
		mBitFields = bitmatrix.mBitFields;
	}

	Bool8 BitMatrix::Get(UInt32 col, UInt32 row) const
	{
		return mBitFields[col].Get(row);
	}

	void BitMatrix::AddColumns(UInt32 index, UInt32 count)
	{
		mBitFields.Extend(count, BitField(mHeight));
	}

	void BitMatrix::AddRows(UInt32 index, UInt32 count)
	{
		for (BitField& field : mBitFields)
			field.Extend(count);
	}

	void BitMatrix::RemoveColumns(UInt32 index, UInt32 count)
	{
		mBitFields.Shrink(index, count);
	}

	void BitMatrix::RemoveRows(UInt32 index, UInt32 count)
	{
		for (BitField& field : mBitFields)
			field.Shrink(index, count);
	}

	void BitMatrix::Set(UInt32 col, UInt32 row)
	{
		mBitFields[col].Set(row);
	}

	void BitMatrix::Reset(UInt32 col, UInt32 row)
	{
		mBitFields[col].Reset(row);
	}

	void BitMatrix::Flip(UInt32 col, UInt32 row)
	{
		mBitFields[col].Flip(row);
	}
}