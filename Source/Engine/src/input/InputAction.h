#pragma once

#include "Common.h"

namespace Quartz
{
	enum InputAction
	{
		/* Indiviual Actions */
		ACTION_NONE		= 0x00,
		ACTION_UP		= 0x01,
		ACTION_DOWN		= 0x02,
		ACTION_PRESSED	= 0x04,
		ACTION_RELEASED = 0x08,
		ACTION_REPEAT	= 0x10,

		/* Combo Actions */
		ACTION_ANY		= (ACTION_UP | ACTION_DOWN | ACTION_PRESSED | ACTION_RELEASED | ACTION_REPEAT),
		ACTION_ANY_UP	= (ACTION_UP | ACTION_RELEASED),
		ACTION_ANY_DOWN	= (ACTION_DOWN | ACTION_PRESSED | ACTION_REPEAT)
	};

	typedef Flags8 InputActions;

}