#ifndef XKILL_IO_IOCOMPONENT_H
#define XKILL_IO_IOCOMPONENT_H

struct MdlDescModel;

class MeshMakerObj;
class MeshModel;
class LoaderFbx;
class LoaderFbxModelDesc;
class LoaderFbxMeshDesc;
class LoaderFbxMaterialDesc;

#include <vector>
#include <map>

#include <xkill-utilities/IObserver.h>

#include "dllIO.h"

#include <xkill-utilities/LeanWindows.h>

#include <xkill-utilities/SkinnedData.h>
#include "LoaderFbxAnimationDesc.h"

static const std::string PATH_XKILL_RESOURCES		 = "../../xkill-resources/";
static const std::string PATH_XKILL_RESOURCES_LEVELS = "../../xkill-resources/xkill-level/";
static const LPCTSTR PATH_TEXDESC					 = L"../../xkill-resources/*.texdesc";
static const LPCTSTR PATH_MDLDESC					 = L"../../xkill-resources/*.mdldesc";

enum FileExtension
{
	FILE_EXTENSION_UNKNOWN,
	FILE_EXTENSION_OBJ,
	FILE_EXTENSION_FBX
};

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
	bool initLvlMdlDesc(std::string filename);

	bool loadModel(
		std::string		modelName,
		std::string		modelPath,
		MdlDescModel*	modelDesc);

	bool loadObj(std::string modelName, std::string modelPath, MdlDescModel* modelDesc, MeshDesc& meshDesc);
	bool loadFbx(std::string modelName, std::string modelPath, MdlDescModel* modelDesc, MeshDesc& meshDesc);
	void loadFbxMesh(LoaderFbxMeshDesc* mesh, LoaderFbxMaterialDesc* material, MeshDesc& meshDesc);
	void loadFbxAnimation(std::vector<LoaderFbxAnimationDesc> animationDescs, LoaderFbxMeshDesc mesh, SkinnedData* skinnedData);
	bool loadPGY(std::string modelName, std::string modelPath, MdlDescModel* modelDesc, MeshDesc& meshDesc);
	bool writePGY(std::string modelName, std::string modelPath, MeshDesc meshDesc, VertexType vertexType);

	FileExtension findFileType(std::string modelName);

	bool pollFile(std::string path, std::string fileName);

	std::vector<std::string> getFileNames(LPCTSTR filename);

	std::map<std::string, unsigned int>* texNameToTexID;

	LoaderFbx* fbxLoader_;
};

#endif //XKILL_IO_IOCOMPONENT_H