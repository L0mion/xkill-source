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
#include "DataStreamBuffer.h"

typedef DataStreamBuffer<VertexInstanced> InstancedData;

class ManagementInstance
{
public:
	ManagementInstance();	//!< Calls attributes init-macro.
	~ManagementInstance();	//!< Clears all InstancedData held by ManagementInstance.

	void update(ID3D11Device* device, ID3D11DeviceContext* devcon); //!< Resets all InstancedData in map. Fills each InstancedData with updated instances. Calls update-method on all InstancedData-objects.

	std::map<
		unsigned int,
		InstancedData*>& getInstancesMap();	//!< Returns map holding meshIDs and corresponding InstancedData-objects.
protected:
private:
	void addRenderAtInstance(AttributePtr<Attribute_Render> ptr_render);		//!< Adds a RenderAttribute to the correct InstancedData. If such an object does not exist, it is created.
	InstancedData* getInstancesFromMeshID(unsigned int meshID);	//!< Checks map for existing InstancedData-object corresponding to meshID. Returns nullptr if such an object is not found.
	DirectX::XMFLOAT4X4 calculateWorldMatrix(
		AttributePtr<Attribute_Spatial>	ptr_spatial, 
		AttributePtr<Attribute_Position> ptr_position);	//!< Calculates the world-matrix of an instance.

	std::map<
		unsigned int, 
		InstancedData*> instancesMap_; //!< Holds all InstancedData objects, which maintains the queues of all instances. First element is meshID, the second is the corresponding InstancedData-object.
};

#endif //XKILL_RENDERER_MANAGEMENTRENDERAT_H