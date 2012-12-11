#ifndef XKILL_RENDERER_M3DLOADER_H
#define XKILL_RENDERER_M3DLOADER_H

#include <fstream>
#include <vector>
#include <map>
#include <string>

#include <xkill-utilities/MeshVertices.h>

#include "M3DMeshData.h"
#include "AnimationClip.h"
#include "BoneAnimation.h"
#include "SkinnedData.h"

class M3DLoader
{
public:
	M3DLoader();
	~M3DLoader();

	void loadM3D(const std::string& filename,
		std::vector<VertexPosNormTexTanSkinned>& vertices,
		std::vector<unsigned int>& indices,
		std::vector<M3DSubset>& subsets,
		std::vector<M3DMaterial>& materials,
		SkinnedData& skinInfo);

private:
	void parseMaterials(std::ifstream& infile, unsigned int numMaterials,
						std::vector<M3DMaterial>& materials);
	
	void parseSubsetTable(std::ifstream& infile, unsigned int numSubsets,
						  std::vector<M3DSubset>& subsets);
	
	void parseSkinnedVertices(std::ifstream& infile, unsigned int numVertices,
							  std::vector<VertexPosNormTexTanSkinned>& vertices);
	
	void parseTriangles(std::ifstream& infile, unsigned int numTriangles,
						std::vector<unsigned int>& indices);
	
	void parseBoneOffsets(std::ifstream& infile, unsigned int numBones,
						  std::vector<DirectX::XMFLOAT4X4>& boneOffsets);
	
	void parseBoneHierarchy(std::ifstream& infile, unsigned int numBones,
							std::vector<int>& boneIndexToParentIndex);
	
	void parseAnimationClips(std::ifstream& infile, unsigned int numAnimationClips,
							 std::map<std::string, AnimationClip>& animations);
	
	void parseBoneKeyFrames(std::ifstream& infile, unsigned int numBones, BoneAnimation& boneAnimation);
};

#endif //XKILL_RENDERER_M3DLOADER_H