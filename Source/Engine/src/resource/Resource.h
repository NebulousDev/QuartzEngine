#pragma once

#include "Common.h"
#include "util/UUID.h"
#include "util/String.h"

namespace Quartz
{
	class ResourceManager;
	class ResourceLoaderBase;

	struct QUARTZ_API ResourceReference
	{
		ResourceLoaderBase*		loader;
		UInt32					count;
	};

	class QUARTZ_API ResourceBase
	{
	public:
		friend ResourceManager;

	protected:
		UUID				mUUID;
		String				mpPathName;
		ResourceManager*	mpResourceManager;
		ResourceReference*	mpResourceReference;

	private:
		void IncrementReference();
		void DecrementReference();

	public:
		ResourceBase();
		~ResourceBase();

		Bool8			IsValid() const;
		UInt32			ReferenceCount() const;
		const String&	GetPathName() const { return mpPathName; }
		const UUID		GetUUID() const { return mUUID; }
		const Bool8		IsVirtual() const { return mpPathName.IsEmpty(); }
 	};

	template<typename ResourceType>
	struct QUARTZ_API Resource : public ResourceBase
	{
		Resource() : ResourceBase::ResourceBase() {};
	};
}