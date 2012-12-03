#ifndef XKILL_RENDERER_OBJGEOMETRY_H
#define XKILL_RENDERER_OBJGEOMETRY_H

#include <vector>

#include "ObjGroup.h"
#include "vertices.h"

class ObjGeometry
{
public:
	ObjGeometry();
	ObjGeometry(const std::vector<VertexPosNormTex> vertices);
	~ObjGeometry();

	void pushGroup(const ObjGroup mg);

	const std::vector<VertexPosNormTex>		getVertices();
	const std::vector<ObjGroup>				getObjGroups();
protected:
private:
	std::vector<VertexPosNormTex>			vertices_;
	std::vector<ObjGroup>					objGroups_;
};

#endif //XKILL_RENDERER_OBJGEOMETRY_H