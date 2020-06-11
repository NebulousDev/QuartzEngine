#pragma once

#include "util\String.h"
#include "../event/Event.h"

namespace Quartz
{
	enum MessageCategory
	{
		MESSAGE_NONE,
		MESSAGE_DEBUG,
		MESSAGE_INFO,
		MESSAGE_GENERAL,
		MESSAGE_WARNING,
		MESSAGE_ERROR,
		MESSAGE_CRITICAL
	};

	struct DebugMessageEvent : public Event<DebugMessageEvent>
	{
		MessageCategory category;
		StringW			message;
	};
}