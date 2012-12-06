#ifndef XKILL_IO_IOCOMPONENT_H
#define XKILL_IO_IOCOMPONENT_H

#if defined (DEBUG) || (DEBUG_)
//#include <vld.h>
#endif //DEBUG || DEBUG_

#include <vector>

#include <xkill-utilities/IObserver.h>
#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/AttributeType.h>

#include "dllIO.h"

class MeshMakerObj;
class MeshModel;

static const std::string bthPath		= "../../xkill-resources/xkill-models/";
static const std::string pgyPath		= "../../xkill-resources/xkill-models/";
static const std::string bthName		= "bth.obj";
static const std::string bthMTLPath		= bthPath;

class DLL_IO IOComponent : public IObserver
{
public:
	IOComponent();
	~IOComponent();

	bool init();
	void reset();

	void onEvent(Event* e);
protected:
private:
	/*Makers*/
	MeshMakerObj* makerObj_;
};

#endif //XKILL_IO_IOCOMPONENT_H