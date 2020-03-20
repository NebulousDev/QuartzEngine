#include "Module.h"

#include "../io/Log.h"

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
		else
		{
			Log.Error("Failed to initialize module '%s' with reason: %s", mModuleName, *pfailMessage);
			return false;
		}

		Log.Info("Initialized module '%s'", mModuleName);

		return true;
	}

	Bool8 Module::DestroyModule()
	{
		if (mDestroyed)
		{
			Log.Warning("Attempted to destroy an already destroyed module '%s'", mModuleName);
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
			Log.Error("Failed to destroy module '%s' with reason: %s", mModuleName, *pfailMessage);
			return false;
		}

		Log.Info("Destroyed module '%s'", mModuleName);

		return true;
	}
}

