#pragma once

#include "Common.h"
#include "util\String.h"

namespace Quartz
{
	class QUARTZ_API Module
	{
	private:
		Bool8 mInitialized;
		Bool8 mDestroyed;
		const Char* mModuleName;

		virtual Bool8 InitModuleImpl(const Char** pFailMessage) = 0;
		virtual Bool8 DestroyModuleImpl(const Char** pFailMessage) = 0;
		virtual const Char* GetModuleNameImpl() const = 0;

	public:
		Bool8 InitModule();
		Bool8 DestroyModule();

		FORCE_INLINE Bool8 IsModuleInitialized() const { return mInitialized; }
		FORCE_INLINE Bool8 IsModuleDestroyed() const { return mDestroyed; }
		FORCE_INLINE const Char* GetModuleName() const { return mModuleName; }
	};
}