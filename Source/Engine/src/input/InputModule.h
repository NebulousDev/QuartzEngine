#pragma once

#include "../Module.h"
#include "Peripherals.h"
#include "InputAction.h"

#include "util/Array.h"

namespace Quartz
{
	struct InputCallback
	{

	};

	struct InputBinding
	{
		Array<InputCallback> callbacks;
	};

	class QUARTZ_API Input : public Module
	{
	private:

	public:
		void RegisterMouseCallback(Mouse* pMouse, );
	};
}