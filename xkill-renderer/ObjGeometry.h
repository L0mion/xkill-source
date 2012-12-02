#ifndef XKILL_RENDERER_OBJGEOMETRY_H
#define XKILL_RENDERER_OBJGEOMETRY_H

#include <vector>

#include "ObjGroup.h"
#include "vertices.h"

class ObjGeometry
{
public:
	ObjGeometry()
	{
		//Do nothing.
	}
	ObjGeometry(const std::vector<VertexPosNormTex> vertices)
	{
		this->vertices_ = vertices;
	}
	~ObjGeometry()
	{
		//Do nothing.
	}

	void pushGroup(const ObjGroup mg)
	{
		objGroups_.push_back(mg);
	}

	const std::vector<VertexPosNormTex>		getVertices()	{ return vertices_;		}
	const std::vector<ObjGroup>				getObjGroups()	{ return objGroups_;	}
protected:
private:
	std::vector<VertexPosNormTex>			vertices_;
	std::vector<ObjGroup>					objGroups_;
};

/*
Template vertex-type?
*/

#endif //XKILL_RENDERER_OBJGEOMETRY_H