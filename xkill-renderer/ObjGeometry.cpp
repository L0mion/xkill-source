#include "ObjGeometry.h"

ObjGeometry::ObjGeometry()
{
	//Do nothing.
}
ObjGeometry::ObjGeometry(const std::vector<VertexPosNormTex> vertices)
{
	this->vertices_ = vertices;
}
ObjGeometry::~ObjGeometry()
{
	//Do nothing.
}

void ObjGeometry::pushGroup(const ObjGroup mg)
{
	objGroups_.push_back(mg);
}

const std::vector<VertexPosNormTex> ObjGeometry::getVertices()
{
	return vertices_;
}
const std::vector<ObjGroup> ObjGeometry::getObjGroups()
{
	return objGroups_;
}