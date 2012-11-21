#ifndef XKILL_ARCHITECTURE_ARCHITECTURE_H
#define XKILL_ARCHITECTURE_ARCHITECTURE_H

#include "dllArchitecture.h"

#include "GameManager.h"

class DLL Architecture
{
public:
	Architecture();
	~Architecture();

	void run()
	{
		GameManager gm;
		gm.run();
	}
};

#endif //XKILL_ARCHITECTURE_ARCHITECTURE_H