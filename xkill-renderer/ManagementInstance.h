#ifndef XKILL_RENDERER_MANAGEMENTINSTANCE_H
#define XKILL_RENDERER_MANAGEMENTINSTANCE_H

namespace DirectX
{
	struct XMFLOAT4X4;
};

struct ID3D11Device;
struct ID3D11DeviceContext;

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
	void update(ID3D11Device* device, ID3D11DeviceContext* devcon);

	std::map<
		unsigned int,
		InstancedData*>& getInstancesMap();
protected:
private:
	void addRenderAtInstance(Attribute_Render* renderAt);
	InstancedData* getInstancesFromMeshID(unsigned int meshID);
	DirectX::XMFLOAT4X4 calculateWorldMatrix(
		Attribute_Spatial*	spaAt, 
		Attribute_Position*	posAt);

	std::map<
		unsigned int, 
		InstancedData*> instancesMap_;
};

#endif //XKILL_RENDERER_MANAGEMENTRENDERAT_H