#ifndef XKILL_RENDERER_ANIMATEDMESH_H
#define XKILL_RENDERER_ANIMATEDMESH_H

#include <fstream>
#include <vector>
#include <map>
#include <string>

#include <xkill-utilities/MeshVertices.h>

#include "M3DMeshData.h"
#include "AnimationClip.h"
#include "BoneAnimation.h"
#include "SkinnedData.h"

class AnimatedMesh
{
public:
	AnimatedMesh();
	~AnimatedMesh();

	std::vector<VertexPosNormTexTanSkinned> vertices;
	std::vector<unsigned int>				indices;
	std::vector<M3DSubset>					subsets;
	std::vector<M3DMaterial>				materials;
	SkinnedData								skinInfo;
private:
};

#endif //XKILL_RENDERER_ANIMATEDMESH_H