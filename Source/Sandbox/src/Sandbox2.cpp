// Sandbox Main

#include "Common.h"

#include "Win32Platform.h"
#include "Engine.h"

#include "util\Map.h"
#include "util\Array.h"

using namespace Quartz;

int main()
{
	{
		Array<String> arr;
		arr.PushBack("Hello");
		arr.PushBack("World");

		Map<String, UInt32> map;
		map.Put("Hello", 1);
		map.Put("World", 2);

		Set<String> set;
		set.Add("Hello");
		set.Add("World");
	}


	/* Initialize Platform Layer */
	Platform* pPlatform = new Win32Platform();
	pPlatform->Initialize();

	/* Initialize Engine */
	Engine& engine = Engine::GetInstance(); 
	engine.SetPlatform(pPlatform);

	/* Start Engine */
	engine.Start();

	//if(KeyDown("Forward"))
	//SetInputCallback("Forward", myCallback);
}