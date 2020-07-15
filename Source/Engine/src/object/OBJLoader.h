#pragma once

#include "Model.h"
#include "util\String.h"
#include "util\StringParser.h"
#include "util\Array.h"
#include "util\Map.h"
#include "math\Math.h"

namespace Quartz
{
	// Load a Model from OBJ
	Model LoadOBJ(const String& data);
}