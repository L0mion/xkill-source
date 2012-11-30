#ifndef XKILL_RENDERER_MESHMAKER_H
#define XKILL_RENDERER_MESHMAKER_H

#include <Windows.h>
#include <fstream>

#include "MeshModel.h"

class LoaderObj;
class LoaderMTL;

class MeshMakerObj
{
public:
	MeshMakerObj(
		const LPCWSTR pathObj,
		const LPCWSTR fileNameObj);
	~MeshMakerObj();

	bool init();
protected:
private:
	bool loadObj();
	bool checkPGY();
	void makeMesh(const Obj obj);

	LPCWSTR pathObj_;
	LPCWSTR fileNameObj_;

	LoaderObj* loaderObj_;
	LoaderMTL* loaderMtl_;

	/*Result*/
	MeshModel meshModel_;
};

#endif //XKILL_RENDERER_MESHMAKER_H