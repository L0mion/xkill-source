#ifndef XKILL_RENDERER_MESHMODELD3D_H
#define XKILL_RENDERER_MESHMODELD3D_H

#include <vector>

class VB;
class IB;

class MeshModelD3D
{
public:
	MeshModelD3D(
		VB* vb,
		const std::vector<IB*> ibs);
	~MeshModelD3D();

	VB* getVB();
	const std::vector<IB*>&	getIBs();
protected:
private:
	VB* vb_;
	std::vector<IB*> ibs_;
};

#endif //XKILL_RENDERER_MESHMODELD3D_H