#ifndef XKILL_RENDERER_OBJGEOMETRY_H
#define XKILL_RENDERER_OBJGEOMETRY_H

#include <vector>

#include "ObjGroup.h"

template <typename Vertex>
class ObjGeometry
{
public:
	ObjGeometry()
	{
		//Do nothing.
	}
	ObjGeometry(const std::vector<Vertex> vertices)
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

	const std::vector<Vertex>		getVertices()	{ return vertices_;	}
	const std::vector<ObjGroup>		getObjGroups()	{ return objGroups_;		}
protected:
private:
	std::vector<Vertex>			vertices_;
	std::vector<ObjGroup>		objGroups_;
};

/*
Template vertex-type?
*/

#endif //XKILL_RENDERER_OBJGEOMETRY_H