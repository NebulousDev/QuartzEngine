#pragma once

#include "Common.h"
#include <cstdio>

namespace Quartz
{
	struct CopyTester
	{
		static UInt32	sNextId;
		UInt32			id;
		UInt32			value;

		CopyTester() : value(0), id(sNextId++)
		{
			printf("Default Constructed: [Value = %d, ID = %d]\n", value, id);
		}

		CopyTester(UInt32 value) : value(value), id(sNextId++)
		{
			printf("Constructed: [Value = %d, ID = %d]\n", value, id);
		}

		CopyTester(const CopyTester& tester) : value(tester.value), id(sNextId++)
		{
			printf("Copy Constructed: [Value = %d, ID = %d]\n", value, id);
		}

		CopyTester(CopyTester&& tester)
		{
			Swap(value, tester.value);
			Swap(id, tester.id);
			printf("Move Constructed: [Value = %d, ID = %d]\n", value, id);
		}

		void operator=(CopyTester tester)
		{
			Swap(value, tester.value);
			printf("Equal Assignment: [Value = %d, ID = %d]\n", value, id);
		}

		Bool8 operator==(const CopyTester& tester)
		{
			return value == tester.value;
		}

		~CopyTester()
		{
			printf("Destructed: [Value = %d, ID = %d]\n", value, id);
		}
	};

	UInt32 CopyTester::sNextId = 0;
}