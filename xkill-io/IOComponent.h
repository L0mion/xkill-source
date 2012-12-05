#ifndef XKILL_RENDERER_IOCOMPONENT_H
#define XKILL_RENDERER_IOCOMPONENT_H

#include <vector>

//#include <xkill-utilities/IObserver.h>
//#include <xkill-utilities/EventManager.h>
//#include <xkill-utilities/AttributeType.h>

#include "dllIO.h"

class MeshMakerObj;
class MeshModel;

static const std::string bthPath		= "../../xkill-resources/xkill-models/";
static const std::string pgyPath		= "";
static const std::string bthName		= "bth.obj";
static const std::string bthMTLPath		= bthPath;

class DLL_IO IOComponent
{
public:
	IOComponent();
	~IOComponent();

	bool init();
	void reset();

	MeshModel* getTempModel() { return models_.at(0); } //totally temp
protected:
private:
	/*Makers*/
	MeshMakerObj* makerObj_;

	/*Loaded models*/
	std::vector<MeshModel*> models_;
};

#endif //XKILL_RENDERER_IOCOMPONENT_H

/*
void onEvent(Event* e);
void onUpdate(float delta)
{
// Setup
std::vector<MeshAttribute>* allMesh; GET_ATTRIBUTES(allMesh, MeshAttribute, ATTRIBUTE_MESH);

// Update
for(unsigned i=0; i<allMesh->size(); i++)
{
MeshAttribute* mesh	=	&allMesh->at(i);
mesh->id;
}
}
*/