#pragma once

#include "Common.h"

namespace Quartz
{
	template<typename _Type>
	class RefPtr
	{
	public:
		typedef RefPtr<_Type>	RefPtrType;
		typedef typename _Type	RefType;

	private:
		RefType*	mpRef;
		UInt32*		mpCount;

	public:
		RefPtr(RefType* pValue) :
			mpRef(pValue), mpCount(new UInt32(1))
		{
			// Nothing
		}

		RefPtr(const RefPtrType& refPtr) :
			mpRef(refPtr.mpRef), mpCount(refPtr.mpCount)
		{
			++(*mpCount);
		}

		RefPtr(RefPtrType&& rRefPtr) noexcept :
			RefPtrType()
		{
			Swap(*this, rRefPtr);
		}

		~RefPtr()
		{
			if (--(*mpCount) == 0)
			{
				delete mpRef;
				delete mpCount;
			}
		}

		friend void Swap(RefPtrType& ref1, RefPtrType& ref2)
		{
			using Quartz::Swap;

			Swap(ref1.mpRef, ref2.mpRef);
			Swap(ref1.mpCount, ref2.mpCount);
		}

		RefPtrType& operator=(RefPtrType refPtr)
		{
			Swap(*this, refPtr);
			return *this;
		}

		operator RefType&()
		{ 
			return *mpRef;
		};

		operator RefType*()
		{ 
			return mpRef;
		};

		Bool8 operator==(const RefType* pValue) const
		{ 
			return mpRef == pValue;
		}

		Bool8 operator!=(const RefType* pValue) const
		{ 
			return mpRef != pValue;
		}

		Bool8 operator==(const RefPtrType& refPtr) const
		{ 
			return mpRef == refPtr.mpRef;
		}

		Bool8 operator!=(const RefPtrType& refPtr) const
		{ 
			return mpRef != refPtr.mpRef;
		}

		UInt64 RefCount() const
		{ 
			return *mpCount;
		}

		Bool8 IsUnique() const
		{
			return *mpCount == 1;
		}
	};
}