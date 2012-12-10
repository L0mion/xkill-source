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

static const std::string objPath = "../../xkill-resources/xkill-models/";
static const std::string pgyPath = "../../xkill-resources/xkill-models/";
static const std::string mtlPath = "../../xkill-resources/xkill-models/";

static const std::string bthName	= "bth.obj";
static const std::string projectileName	= "projectile.obj";
static const std::string arenaName1	= "xkillArena1.obj";
static const std::string arenaName2	= "xkillArena2.obj";

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
	bool initBth(); //temp
	bool initArena1(); //temp
	bool initArena2(); //temp
	bool initProjectile(); //temp

	/*Makers*/
	MeshMakerObj* makerBTH_;
	MeshMakerObj* makerArena1_;
	MeshMakerObj* makerArena2_;
	MeshMakerObj* makerProjectile_;
};

#endif //XKILL_IO_IOCOMPONENT_H