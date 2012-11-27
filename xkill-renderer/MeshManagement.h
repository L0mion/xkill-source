#ifndef XKILL_RENDERER_MESHMANAGEMENT_H
#define XKILL_RENDERER_MESHMANAGEMENT_H

#include <d3d11.h>

#include <d3dInterface.h>

class MeshManagement : public D3DInterface
{
public:
	MeshManagement();
	~MeshManagement();

	HRESULT init();
	void reset();
protected:
private:
	unsigned int mmUnsigned;
};

#endif //XKILL_RENDERER_MESHMANAGEMENT_H