#ifndef XKILL_RENDERER_MESHMANAGEMENT_H
#define XKILL_RENDERER_MESHMANAGEMENT_H

#include <d3d11.h>
#include <d3dInterface.h>
#include <vector>

#include "MeshModel.h"

class MeshMakerObj;

static const std::string bthPath		= "../../xkill-resources/xkill-models/";
static const std::string pgyPath		= "";
static const std::string bthName		= "bth.obj";
static const std::string bthMTLPath		= bthPath;

class DLL_M MeshManagement //: public D3DInterface
{
public:
	MeshManagement();
	~MeshManagement();

	bool init();
	void reset();

	MeshModel getTempModel() { return models_.at(0); } //totally temp
protected:
private:
	/*Makers*/
	MeshMakerObj* makerObj_;

	/*Loaded models*/
	std::vector<MeshModel> models_;
};

#endif //XKILL_RENDERER_MESHMANAGEMENT_H