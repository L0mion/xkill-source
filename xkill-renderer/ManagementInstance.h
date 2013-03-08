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

class CameraInstances;

#include <map>

#include "MeshVerticesInstanced.h"

class ManagementInstance
{
public:
	ManagementInstance();	//!< Calls attributes init-macro.
	~ManagementInstance();	//!< Clears all InstancedData held by ManagementInstance.

	void update(ID3D11Device* device, ID3D11DeviceContext* devcon); //!< Resets all InstancedData in map. Fills each InstancedData with updated instances. Calls update-method on all InstancedData-objects.

	CameraInstances* getCameraInstancesFromCameraIndex(unsigned int camIndex);
protected:
private:
	void addInstance(AttributePtr<Attribute_Render> ptr_render);
	DirectX::XMFLOAT4X4 calculateWorldMatrix(
		AttributePtr<Attribute_Spatial>	ptr_spatial, 
		AttributePtr<Attribute_Position> ptr_position);	//!< Calculates the world-matrix of an instance.
	void addCameraInstance(
		AttributePtr<Attribute_Camera> ptr_camera,
		unsigned int meshID,
		VertexInstanced instance);

	std::map<
		unsigned int,
		CameraInstances*> cameraInstancesMap_;
};

#endif //XKILL_RENDERER_MANAGEMENTRENDERAT_H