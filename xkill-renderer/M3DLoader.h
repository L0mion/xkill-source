#ifndef XKILL_RENDERER_M3DLOADER_H
#define XKILL_RENDERER_M3DLOADER_H

#include <fstream>
#include <vector>
#include <map>
#include <string>

#include <xkill-utilities/MeshVertices.h>

namespace DirectX
{
	struct XMFLOAT4X4;
}

struct M3DMaterial;
struct M3DSubset;
class AnimationClip;
class BoneAnimation;
class SkinnedData;

//! Class that loads a m3d-file.
/*!
\ingroup xkill-renderer
*/
class M3DLoader
{
public:
	//! Initializes M3DLoader to its default state.
	M3DLoader();
	//! Releases all memory and resets M3DLoader to its default state.
	~M3DLoader();

	//! Loads a m3d-file.
	/*!
	\param filename Name of the file to be loaded.
	\param vertices A vector where the laoded vertices will be stored.
	\param indices A vector where the laoded indices will be stored.
	\param subsets A vector where the loaded subsets will be stored.
	\param materials A vector where the loaded materials will be loaded.
	\param skinInfo A SkinnedData object where the loaded animatiion data will be stored.
	*/
	void loadM3D(const std::string&					filename,
		std::vector<VertexPosNormTexTanSkinned>*	vertices,
		std::vector<unsigned int>*					indices,
		std::vector<M3DSubset>*						subsets,
		std::vector<M3DMaterial>*					materials,
		SkinnedData*								skinInfo);

private:
	//! Loads materials
	/*!
	\param infile An ifstream object that has the m3d-file open.
	\param numMaterials The number of materials to be loaded.
	\param materials A vector where the loaded materials will be stored.
	*/
	void parseMaterials(std::ifstream& infile, unsigned int numMaterials,
						std::vector<M3DMaterial>* materials);
	//! Loads subsets
	/*!
	\param infile An ifstream object that has the m3d-file open.
	\param numSubsets The number of subsets to be loaded.
	\param subsets A vector where the loaded subsets will be stored.
	*/
	void parseSubsetTable(std::ifstream& infile, unsigned int numSubsets,
						  std::vector<M3DSubset>* subsets);
	//! Loads vertices
	/*!
	\param infile An ifstream object that has the m3d-file open.
	\param numVertices The number of vertices to be loaded.
	\param vertices A vector where the loaded vertices will be stored.
	*/
	void parseSkinnedVertices(std::ifstream& infile, unsigned int numVertices,
							  std::vector<VertexPosNormTexTanSkinned>* vertices);
	//! Loads triangles
	/*!
	\param infile An ifstream object that has the m3d-file open.
	\param numTriangles The number of triangles to be loaded.
	\param indices A vector where the loaded indices describing trinagles will be stored.
	*/
	void parseTriangles(std::ifstream& infile, unsigned int numTriangles,
						std::vector<unsigned int>* indices);
	//! Loads bone offsets
	/*!
	\param infile An ifstream object that has the m3d-file open.
	\param numBones The number of bones to be loaded.
	\param boneOffsets A vector where the loaded bone offsets will be stored.
	*/
	void parseBoneOffsets(std::ifstream& infile, unsigned int numBones,
						  std::vector<DirectX::XMFLOAT4X4>* boneOffsets);
	//! Loads bone hierarchy
	/*!
	\param infile An ifstream object that has the m3d-file open.
	\param numBones The number of bones to be loaded.
	\param boneIndexToParentIndex A vector where the loaded hierarchy will be stored.
	*/
	void parseBoneHierarchy(std::ifstream& infile, unsigned int numBones,
							std::vector<int>* boneIndexToParentIndex);
	//! Loads animation clips
	/*!
	\param infile An ifstream object that has the m3d-file open.
	\param numAnimationClips The number of animation clips to be loaded.
	\param numBones The number of bones in each animation clip.
	\param materials A vector where the loaded materials will be stored.
	*/
	void parseAnimationClips(std::ifstream& infile, unsigned int numAnimationClips,
							 unsigned int numBones, std::map<std::string, AnimationClip*>* animations);
	//! Loads all keyframes for a bone.
	/*!
	\param infile An ifstream object that has the m3d-file open.
	\param boneAnimation A BoneAnimation object where the keyframes will be stored.
	*/
	void parseBoneKeyframes(std::ifstream& infile, BoneAnimation* boneAnimation);
};

#endif //XKILL_RENDERER_M3DLOADER_H