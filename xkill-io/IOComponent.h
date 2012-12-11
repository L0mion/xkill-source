#ifndef XKILL_IO_IOCOMPONENT_H
#define XKILL_IO_IOCOMPONENT_H

#include <vector>

#include <xkill-utilities/IObserver.h>
#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/AttributeType.h>

#include "dllIO.h"

class MeshMakerObj;
class MeshModel;

static const std::string PATH_XKILL_RESOURCES	= "../../xkill-resources/";
static const LPCTSTR PATH_TEXDESC				= L"../../xkill-resources/*.texdesc";

//temp
static const std::string objPath = "../../xkill-resources/xkill-models/";
static const std::string pgyPath = "../../xkill-resources/xkill-models/";
static const std::string mtlPath = "../../xkill-resources/xkill-models/";

static const std::string bthName		= "bth.obj";
static const std::string projectileName	= "projectile.obj";
static const std::string arenaName		= "xkillArena.obj";

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
	bool initTexDescs();
	bool initTexDesc(std::string filename);

	std::vector<std::string> getFileNames(LPCTSTR filename);

	bool initBth(); //temp
	bool initArena(); //temp
	bool initProjectile(); //temp

	/*Makers*/
	MeshMakerObj* makerBTH_;
	MeshMakerObj* makerArena_;
	MeshMakerObj* makerProjectile_;
};

#endif //XKILL_IO_IOCOMPONENT_H