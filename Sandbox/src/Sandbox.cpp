#pragma once

#include "Types.h"
#include "util\Array.h"
#include "util\String.h"

#include <iostream>

int main()
{
	Quartz::Array<Quartz::String> strings;

	strings.PushBack("Hello");
	strings.PushBack("World!");

	printf("%s\n", (strings[0] + " " + strings[1]).Str());

	return 0;
}