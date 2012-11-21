
#include "GameManager.h"
#include <iostream>

int main()
{
	// Detect memory leaks
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	GameManager gm;
	gm.run();
}