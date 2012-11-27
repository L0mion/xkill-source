#ifndef XKILL_RENDERER_MESHLOADER_H
#define XKILL_RENDERER_MESHLOADER_H

#include <Windows.h>
#include <fstream>

class MeshLoader
{
public:
	MeshLoader(LPCWSTR mlFileName);
	~MeshLoader();

	virtual bool init()			= 0;
	bool mlLoad();
protected:
	virtual bool mlLoadMesh()	= 0;

	const LPCWSTR getMLFileName();

	std::ifstream	mlIFS_;
private:
	LPCWSTR			mlFileName_;
};

#endif //XKILL_RENDERER_MESHLOADER_H