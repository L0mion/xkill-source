#ifndef XKILL_RENDERER_MANAGEMENTINSTANCE_H
#define XKILL_RENDERER_MANAGEMENTINSTANCE_H

namespace DirectX
{
	struct XMFLOAT4X4;
};

struct Attribute_Spatial;
struct Attribute_Position;
struct Attribute_Render;

#include <vector>
#include <map>

#include "MeshVerticesInstanced.h"
#include "InstancedData.h"

class ManagementInstance
{
public:
	ManagementInstance();
	~ManagementInstance();

	void init();
	void update();
protected:
private:
	void addRenderAtInstance(Attribute_Render* renderAt);
	bool getInstancesFromMeshID(unsigned int meshID, InstancedData& inout_instancedData);
	DirectX::XMFLOAT4X4 calculateWorldMatrix(
		Attribute_Spatial*	spaAt, 
		Attribute_Position*	posAt);

	std::map<
		unsigned int, 
		InstancedData> instancesMap_;
};

#endif //XKILL_RENDERER_MANAGEMENTRENDERAT_H