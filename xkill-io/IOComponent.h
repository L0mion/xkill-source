#ifndef XKILL_IO_IOCOMPONENT_H
#define XKILL_IO_IOCOMPONENT_H

struct MdlDescModel;

class MeshMakerObj;
class MeshModel;
class LoaderFbx;

#include <vector>
#include <map>

#include <xkill-utilities/IObserver.h>

#include "dllIO.h"

#include <xkill-utilities/LeanWindows.h>

static const std::string PATH_XKILL_RESOURCES	= "../../xkill-resources/";
static const LPCTSTR PATH_TEXDESC				= L"../../xkill-resources/*.texdesc";
static const LPCTSTR PATH_MDLDESC				= L"../../xkill-resources/*.mdldesc";

//! Component loading and writing from/to file.
/*!
\ingroup xkill-io
*/
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

	std::map<std::string, unsigned int>* texNameToTexID;
<<<<<<< HEAD
	std::vector<MeshModel*> meshModels_; //temporarily store models until these may be deleted in attribute

	LoaderFbx* fbxLoader_;
=======
>>>>>>> dev3
};

#endif //XKILL_IO_IOCOMPONENT_H