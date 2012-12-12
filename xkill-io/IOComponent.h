#ifndef XKILL_IO_IOCOMPONENT_H
#define XKILL_IO_IOCOMPONENT_H

struct MdlDescModel;

class MeshMakerObj;
class MeshModel;

#include <vector>

#include <xkill-utilities/IObserver.h>
#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/AttributeType.h>

#include "dllIO.h"

static const std::string PATH_XKILL_RESOURCES	= "../../xkill-resources/";
static const LPCTSTR PATH_TEXDESC				= L"../../xkill-resources/*.texdesc";
static const LPCTSTR PATH_MDLDESC				= L"../../xkill-resources/*.mdldesc";

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
	bool initMdlDescs();
	bool initMdlDesc(std::string filename);

	bool loadModel(
		std::string		modelName,
		std::string		modelPath,
		MdlDescModel*	modelDesc);

	std::vector<std::string> getFileNames(LPCTSTR filename);

	std::vector<MeshModel*> meshModels_; //temporarily store models until these may be deleted in attribute
};

#endif //XKILL_IO_IOCOMPONENT_H