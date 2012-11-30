#ifndef XKILL_RENDERER_MESHGEOMETRY_H
#define XKILL_RENDERER_MESHGEOMETRY_H

#include <vector>

#include "ObjGroup.h"

template <typename Vertex>
class MeshGeometry
{
public:
	MeshGeometry()
	{
		//Do nothing.
	}
	MeshGeometry(const std::vector<Vertex> mgVertices)
	{
		this->mgVertices_ = mgVertices;
	}
	~MeshGeometry()
	{
		//Do nothing.
	}

	void mgPushGroup(const ObjGroup mg)
	{
		mgGroups_.push_back(mg);
	}

	bool isValid()
	{
		bool isValid = true;

		if(mgVertices_.size() <= 0)
			isValid = false;
		if(mgGroups_.size() <= 0)
			isValid = false;

		return isValid;
	}

	const std::vector<Vertex>		getMGVertices()	{ return mgVertices_;	}
	const std::vector<ObjGroup>	getMGGroups()	{ return mgGroups_;		}
protected:
private:
	std::vector<Vertex>			mgVertices_;
	std::vector<ObjGroup>		mgGroups_;
};

/*
Template vertex-type?
*/

#endif //XKILL_RENDERER_MESHGEOMETRY_H