
#include "Common.h"
#include <stdio.h>

int QUARTZ_API TempTemp()
{
	return 35;
}

int main()
{
	printf("Hello World %d\n", TempTemp());
	return 1;
}