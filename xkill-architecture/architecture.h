#ifndef XKILL_ARCHITECTURE_ARCHITECTURE_H
#define XKILL_ARCHITECTURE_ARCHITECTURE_H

#include "dllArchitecture.h"

class DLL Architecture
{
public:
	Architecture();
	~Architecture();

	unsigned int getTest();
private:
	unsigned int test;
};

#endif //XKILL_ARCHITECTURE_ARCHITECTURE_H