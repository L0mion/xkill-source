#ifndef XKILL_RENDERER_OBJ_H
#define XKILL_RENDERER_OBJ_H

#include <Windows.h>
#include <vector>
#include <string>

#include "MeshGeometry.h"
#include "ObjDependencies.h"

struct VertexPosNormTex;

class Obj
{
public:
	Obj();
	Obj(
		const LPCWSTR filename,
		const ObjDependencies objDependencies,
		const MeshGeometry<VertexPosNormTex> meshGeometry);
	~Obj();

	LPCWSTR getFileName();
	std::vector<std::string> getMTLs();
	MeshGeometry<VertexPosNormTex> getMeshGeometry();
protected:
private:
	LPCWSTR fileName_;
	ObjDependencies objDependencies_;
	MeshGeometry<VertexPosNormTex> meshGeometry_;
};

#endif //XKILL_RENDERER_OBJ_H