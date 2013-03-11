#ifndef XKILL_RENDERER_CAMERAINSTANCES_H
#define XKILL_RENDERER_CAMERAINSTANCES_H

struct ID3D11Device;
struct ID3D11DeviceContext;

#include <map>

#include "MeshVerticesInstanced.h"
#include "DataStreamBuffer.h"

typedef DataStreamBuffer<VertexInstanced> InstancedData;

class CameraInstances
{
public:
	CameraInstances();
	~CameraInstances();

	void reset();
	void update(ID3D11Device* device, ID3D11DeviceContext* devcon);
	void addInstance(unsigned int meshID, VertexInstanced instance);

	std::map<
		unsigned int, 
		InstancedData*>& getInstancesMap();
protected:
private:
	InstancedData* getInstancesFromMeshID(unsigned int meshID);

	std::map<
		unsigned int, 
		InstancedData*> instancesMap_;
};

#endif //XKILL_RENDERER_CAMERAINSTANCES_H