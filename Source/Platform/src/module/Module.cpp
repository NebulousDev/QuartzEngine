#include "Module.h"

namespace Quartz
{
	Bool8 Module::InitModule()
	{
		mModuleName = GetModuleNameImpl();
		
		const Char** pfailMessage = NULL;

		if (InitModuleImpl(pfailMessage))
		{
			mInitialized = true;
			mDestroyed = false;
		}

		return true;
	}

	Bool8 Module::DestroyModule()
	{
		if (mDestroyed)
		{
			return false;
		}

		const Char** pfailMessage = NULL;

		if (DestroyModuleImpl(pfailMessage))
		{
			mInitialized = false;
			mDestroyed = true;
		}
		else
		{
			return false;
		}

		return true;
	}
}

