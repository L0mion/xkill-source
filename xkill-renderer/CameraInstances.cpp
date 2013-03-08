#include "renderingUtilities.h"
#include "CameraInstances.h"

CameraInstances::CameraInstances()
{

}
CameraInstances::~CameraInstances()
{
	for(
		auto i = instancesMap_.begin(); 
		i != instancesMap_.end(); 
		i++)
	{
		SAFE_DELETE(i->second);
	}
}

void CameraInstances::reset()
{
	//Reset all buffers.
	for(
		auto i = instancesMap_.begin(); 
		i != instancesMap_.end(); 
		i++)
	{
		i->second->resetStream();
	}
}
void CameraInstances::update(ID3D11Device* device, ID3D11DeviceContext* devcon)
{
	for(
		auto i = instancesMap_.begin();
		i != instancesMap_.end();
		i++)
	{
		i->second->updateDataStream(device, devcon);
	}
}
void CameraInstances::addInstance(unsigned int meshID, VertexInstanced instance)
{
	InstancedData* instancedData = getInstancesFromMeshID(meshID);
	if(instancedData != nullptr)
	{ //add new instance to corresponding instance vector.
		instancedData->pushData(instance);
	}
	else
	{ //no existing instanced data of mesh id, create new one.
		instancedData = new InstancedData(D3D11_BIND_VERTEX_BUFFER, 0);
		instancedData->pushData(instance);
	
		instancesMap_.insert(std::pair<unsigned int, InstancedData*>(meshID, instancedData));
	}
}

InstancedData* CameraInstances::getInstancesFromMeshID(unsigned int meshID)
{
	InstancedData* instancedData = nullptr;
	
	auto it = instancesMap_.find(meshID);
	if(it != instancesMap_.end())
	{
		instancedData = it->second;
	}
	
	return instancedData;
}

std::map<
	unsigned int, 
	InstancedData*>& CameraInstances::getInstancesMap()
{
	return instancesMap_;
}