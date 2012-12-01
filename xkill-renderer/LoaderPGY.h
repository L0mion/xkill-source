#ifndef XKILL_RENDERER_LOADERPGY_H
#define XKILL_RENDERER_LOADERPGY_H

#include "Loader.h"

class LoaderPGY : public Loader
{
public:
	LoaderPGY(
		const std::string filePath, 
		const std::string fileName);
	~LoaderPGY();

	void init();
protected:
private:
};

#endif //XKILL_RENDERER_LOADERPGY_H