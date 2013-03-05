#ifndef XKILL_IO_IOCOMPONENT_H
#define XKILL_IO_IOCOMPONENT_H

struct MdlDescModel;
struct LoaderMD5ModelDesc;

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
#include "LoaderFbxTextureDesc.h"

static const std::string PATH_XKILL_RESOURCES		 = "../../xkill-resources/";
static const std::string PATH_XKILL_RESOURCES_LEVELS = "../../xkill-resources/xkill-level/";
static const LPCTSTR PATH_TEXDESC					 = L"../../xkill-resources/";
static const LPCTSTR EXTENSION_TEXDESC				 = L".texdesc";
static const LPCTSTR PATH_MDLDESC					 = L"../../xkill-resources/";
static const LPCTSTR EXTENSION_MDLDESC				 = L".mdldesc";

enum FileExtension
{
	FILE_EXTENSION_UNKNOWN,
	FILE_EXTENSION_OBJ,
	FILE_EXTENSION_FBX,
	FILE_EXTENSION_MD5MESH,
	FILE_EXTENSION_MD5ANIM
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
	bool loadFbx(std::string modelName, std::string modelPath, MdlDescModel* modelDesc, MeshDesc& meshDesc, SkinnedData* skinnedData);
	void loadFbxMesh(LoaderFbxMeshDesc* mesh, LoaderFbxMaterialDesc* material, MeshDesc& meshDesc, std::vector<LoaderFbxTextureDesc> texDescs);
	void loadFbxAnimation(std::vector<LoaderFbxAnimationDesc> animationDescs, LoaderFbxMeshDesc mesh, SkinnedData* skinnedData);
	
	bool loadMD5(std::string modelName, std::string modelPath, MdlDescModel* modelDesc, MeshDesc& meshDesc);
	void loadMD5AssembleVertices(std::vector<VertexDesc>* vertices, LoaderMD5ModelDesc* md5Model);
	void loadMD5AssembleSubsets(std::vector<SubsetDesc>* subsets, LoaderMD5ModelDesc* md5Model);
	void loadMD5AssembleMaterials(std::vector<MaterialDesc>* materials, LoaderMD5ModelDesc* md5Model);

	bool loadPGY(std::string modelName, std::string modelPath, MdlDescModel* modelDesc, MeshDesc& meshDesc, SkinnedData** skinnedData);
	bool writePGY(std::string modelName, std::string modelPath, MeshDesc meshDesc, VertexType vertexType, SkinnedData* skinnedData);

	FileExtension findFileType(std::string modelName);

	bool pollFile(std::string path, std::string fileName);

	std::vector<std::string> getFileNames(LPCTSTR filepath, LPCTSTR filename, bool recursiveSearch = false);

	unsigned int getTexIDfromName(std::string texFilename);

	std::map<std::string, unsigned int>* texNameToTexID_;

	LoaderFbx* fbxLoader_;
};

#endif //XKILL_IO_IOCOMPONENT_H