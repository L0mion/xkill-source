#ifndef XKILL_RENDERER_MESHMODEL_H
#define XKILL_RENDERER_MESHMODEL_H

#include <vector>

class MeshDependencies;
class ObjGeometry;
class MeshMaterial;
class MeshSubset;

class MeshModel
{
public:
	MeshModel();
	~MeshModel();
protected:
private:
	std::vector<MeshMaterial>	materials_;
	std::vector<MeshSubset>		subsets_;
};

#endif //XKILL_RENDERER_MESHMODEL_H