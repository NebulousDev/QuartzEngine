#pragma once
#include "../Common.h"

namespace Quartz
{
	template<class RefType>
	class Ref
	{
	private:
		struct RefImpl
		{
			RefType* mpRefPtr;
			UInt64 mRefCount;

			FORCE_INLINE UInt64 AddRef();
			FORCE_INLINE UInt64 SubRef();
		};

	private:
		RefImpl* mpRef;

	public:
		Ref();
		Ref(RefType* pValue, Bool8 cache = false);
		Ref(const Ref<RefType>& ref);
		Ref(Ref<RefType>&& ref) noexcept;

		~Ref();

		FORCE_INLINE RefType& operator*();
		FORCE_INLINE RefType* operator->();

		FORCE_INLINE Ref& operator=(Ref<RefType>& ref);
		FORCE_INLINE Ref& operator=(Ref<RefType>&& ref) noexcept;

		FORCE_INLINE Bool8 operator==(const RefType* pValue) const;
		FORCE_INLINE Bool8 operator!=(const RefType* pValue) const;
		FORCE_INLINE Bool8 operator==(const Ref<RefType>& ref) const;
		FORCE_INLINE Bool8 operator!=(const Ref<RefType>& ref) const;

		FORCE_INLINE operator RefType&() { return *mpRef->mpRefPtr; };
		FORCE_INLINE operator RefType*() { return mpRef->mpRefPtr; };

		FORCE_INLINE UInt64 Count() const { return mpRef->mRefCount; }
		FORCE_INLINE Bool8 IsUnique() const { return mpRef->mRefCount == 1; }
	};

	template<class RefType>
	UInt64 Ref<RefType>::RefImpl::AddRef()
	{
		return ++mRefCount;
	}

	template<class RefType>
	UInt64 Ref<RefType>::RefImpl::SubRef()
	{
		if (!--mRefCount)
			delete this;

		return mRefCount;
	}

	template<class RefType>
	Ref<RefType>::Ref()
		: mpRef(new RefImpl{ NULL, 1 })
	{
		// Nothing
	}

	template<class RefType>
	Ref<RefType>::Ref(RefType* pValue, Bool8 cache)
		: mpRef(new RefImpl{ pValue, cache ? (UInt64)1 : (UInt64)2 })
	{
		// Nothing
	}

	template<class RefType>
	Ref<RefType>::Ref(const Ref<RefType>& ref)
		: mpRef(ref.mpRef)
	{
		mpRef->AddRef();
	}

	template<class RefType>
	Ref<RefType>::Ref(Ref<RefType>&& ref) noexcept
		: mpRef(ref.mpRef)
	{
		mpRef->AddRef();
	}

	template<class RefType>
	Ref<RefType>::~Ref()
	{
		mpRef->SubRef();
	}

	template<class RefType>
	RefType& Ref<RefType>::operator*()
	{
		return *mpRef->mpRefPtr;
	}

	template<class RefType>
	RefType* Ref<RefType>::operator->()
	{
		return mpRef->mpRefPtr;
	}

	template<class RefType>
	Ref<RefType>& Ref<RefType>::operator=(Ref<RefType>& ref)
	{
		ref.mpRef->AddRef();
		mpRef->SubRef();
		mpRef = ref.mpRef;
		return *this;
	}

	template<class RefType>
	Ref<RefType>& Ref<RefType>::operator=(Ref<RefType>&& ref) noexcept
	{
		ref.mpRef->AddRef();
		mpRef->SubRef();
		mpRef = ref.mpRef;
		return *this;
	}

	template<class RefType>
	Bool8 Ref<RefType>::operator==(const RefType* pValue) const
	{
		return mpRef->mpRefPtr == pValue;
	}

	template<class RefType>
	Bool8 Ref<RefType>::operator!=(const RefType* pValue) const
	{
		return mpRef->mpRefPtr != pValue;
	}

	template<class RefType>
	Bool8 Ref<RefType>::operator==(const Ref<RefType>& ref) const
	{
		return mpRef == ref.mpRef;
	}

	template<class RefType>
	Bool8 Ref<RefType>::operator!=(const Ref<RefType>& ref) const
	{
		return mpRef != ref.mpRef;
	}
}