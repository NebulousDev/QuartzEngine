#include "Resource.h"
#include "ResourceManager.h"

namespace Quartz
{
	ResourceBase::ResourceBase() :
		mpResourceManager(nullptr),
		mpResourceReference(nullptr)
	{
		// Nothing
	}

	ResourceBase::~ResourceBase()
	{
		if (mpResourceReference != nullptr)
		{
			DecrementReference();
		}
	}

	void ResourceBase::IncrementReference()
	{
		mpResourceReference->count++;
	}

	void ResourceBase::DecrementReference()
	{
		mpResourceReference->count--;
	}

	Bool8 ResourceBase::IsValid() const
	{
		return mpResourceReference != nullptr;
	}

	UInt32 ResourceBase::ReferenceCount() const
	{
		if (mpResourceReference != nullptr)
		{
			return mpResourceReference->count;
		}

		return 0;
	}
}