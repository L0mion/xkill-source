#ifndef XKILL_RENDERER_OBJ_H
#define XKILL_RENDERER_OBJ_H

#include <Windows.h>
#include <vector>
#include <string>

#include "ObjGeometry.h"
#include "ObjDependencies.h"

struct VertexPosNormTex;

class Obj
{
public:
	Obj();
	Obj(
		const LPCWSTR filename,
		const ObjDependencies objDependencies,
		const ObjGeometry<VertexPosNormTex> objGeometry);
	~Obj();

	const LPCWSTR getFileName();
	const std::vector<std::string> getMTLs();
	const ObjGeometry<VertexPosNormTex> getObjGeometry();
protected:
private:
	LPCWSTR fileName_;
	ObjDependencies objDependencies_;
	ObjGeometry<VertexPosNormTex> objGeometry_;
};

#endif //XKILL_RENDERER_OBJ_H