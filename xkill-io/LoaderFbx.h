#ifndef XKILL_IO_LOADERFBX_H
#define XKILL_IO_LOADERFBX_H

#include <string>
#include <vector>

#include "LoaderFbxModelDesc.h"

namespace fbxsdk_2014_0_beta2
{
	class FbxManager;
	class FbxScene;
	class FbxNode;
}

class LoaderFbxMesh;
class LoaderFbxMaterial;
class LoaderFbxTexture;
class LoaderFbxAnimation;
struct VertexPosNormTex;

//! Class for loading a FBX file
/*!
\ingroup xkill-io-fbx
*/
class LoaderFbx
{
public:
	//!Initializes LoaderFbx to its default state.
	LoaderFbx();
	//! Releases all memory.
	~LoaderFbx();
	//Allocates memory for member pointers and creates the FbxManager.
	bool init();
	//! Creates a FbxScene object and parses the data contained in the scene.
	/*!
	\return A vector containing data extracted from the scene.
	\param filename Name of the file to be loaded.
	\sa LoaderFbxModelDesc
	*/
	std::vector<LoaderFbxModelDesc> load(std::string filename);
	
private:
	//! Creates a FbxManager object.
	/*!
	\return True if succeeded, false if any error was encountered.
	*/
	bool createFbxManager();
	//! Creates a FbxScene object.
	/*!
	\return True if succeeded, false if any error was encountered.
	*/
	bool createFbxScene();
	//! Loads a file into the FbxScene object. 
	/*!
	\return True if succeeded, false if any error was encountered.
	\param filename Name of the file to be loaded.
	*/
	bool loadScene(std::string filename);
	//! Recursivley parses data from the node hierarchy in the scene.
	/*!
	\param node The starting node.
	*/
	void parseNode(fbxsdk_2014_0_beta2::FbxNode* node);
	//! Parses data from a mesh object.
	/*!
	\param node A FbxNode with a FbxNodeAttribute of tye eMesh.
	*/
	void parseMesh(fbxsdk_2014_0_beta2::FbxNode* node);
	

	LoaderFbxMesh*		meshLoader_;		//<! An object used to load meshe data.
	LoaderFbxMaterial*	materialLoader_;	//<! An object used to load material data.
	LoaderFbxTexture*	textureLoader_;		//<! An object used to load texture data.
	LoaderFbxAnimation*	animationLoader_;	//<! An object used to load animation data.

	fbxsdk_2014_0_beta2::FbxManager* fbxManager_;	//<! Manages all fbx related objects.
	fbxsdk_2014_0_beta2::FbxScene*	 fbxScene_;		//<! Object containing information for a loaded fbx-scene

	std::vector<LoaderFbxModelDesc> modelDescs_; //<! Vector containg descriptions of all the loaded models
};

#endif