#include <d3d11.h>
#include <Windows.h>

#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/AttributeType.h>
#include <xkill-utilities/MeshModel.h>

#include "VB.h"
#include "IB.h"
#include "renderingUtilities.h"
#include "ModelD3D.h"
#include "ModelManagement.h"

ModelManagement::ModelManagement()
{
	//Do nothing.
}
ModelManagement::~ModelManagement()
{
	for(unsigned int i = 0; i < meshModelD3Ds_.size(); i++)
	{
		if(meshModelD3Ds_[i])
			delete meshModelD3Ds_[i];
	}
}

HRESULT ModelManagement::init()
{
	HRESULT hr = S_OK;

	//Do nothing.

	return hr;
}

ModelD3D* ModelManagement::getMeshModelD3D(
	const unsigned int	modelID, 
	ID3D11Device*		device)	
{
	if(!existingMeshModelD3D(modelID))
	{
		HRESULT hr = S_OK;
		hr = createMeshModelD3D(modelID, device);
	}

	unsigned int meshModelD3DIndex = getMeshModelD3DIndex(modelID);
	return meshModelD3Ds_[meshModelD3DIndex];
}

HRESULT ModelManagement::createMeshModelD3D(
	const unsigned int	modelID, 
	ID3D11Device*		device)
{
	HRESULT hr = S_OK;

	MeshAttribute meshAt;
	if(getMeshAttribute(modelID, meshAt))
	{
		MeshModel* model = meshAt.mesh;

		VB*					vb = new VB();
		std::vector<IB*>	ibs;

		hr = createVertexBuffer(
			modelID, 
			model->getGeometry(), 
			vb,
			device);
		if(SUCCEEDED(hr))
		{
			hr = createIndexBuffers(
				modelID, 
				model->getGeometry(), 
				ibs, 
				device);
		}
		if(SUCCEEDED(hr))
		{
			pushMeshModelD3D(
			modelID,
			new ModelD3D(vb, ibs));
		}
	}
	else
	{
		//Could not find mesh loaded, error or warning?
	}

	return hr;
}
bool ModelManagement::getMeshAttribute(unsigned int modelID, MeshAttribute& inout)
{
	std::vector<MeshAttribute>*	allMesh; GET_ATTRIBUTES(allMesh, MeshAttribute,	ATTRIBUTE_MESH);
	
	bool foundAt = false;
	for(unsigned int i = 0; i < allMesh->size() && !foundAt; i++)
	{
		if(allMesh->at(i).meshID == modelID)
		{
			inout	= allMesh->at(i);
			foundAt	= true;
		}
	}

	return foundAt;
}
HRESULT ModelManagement::createVertexBuffer(
		const unsigned int	modelID, 
		MeshGeometry&		geometry,
		VB*					vb,
		ID3D11Device*		device)
{
	HRESULT hr = S_OK;

	hr = vb->init(geometry.getVertices(), device);
	if(FAILED(hr))
	{
		std::string failed = "Failed to create Vertex Buffer from MeshModel ID: " + modelID;
		SHOW_MESSAGEBOX(failed);
	}

	return hr;
}
HRESULT ModelManagement::createIndexBuffers(
	const unsigned int	modelID, 
	MeshGeometry&		geometry, 
	std::vector<IB*>&	ibs,
	ID3D11Device*		device)
{
	HRESULT hr = S_OK;

	std::vector<MeshSubset> subsets = geometry.getSubsets();
	for(unsigned int i = 0; i < subsets.size() && !FAILED(hr); i++)
	{
		IB* ib = new IB();
		hr = createIndexBuffer(
			modelID,
			subsets[i],
			ib,
			device);
		if(SUCCEEDED(hr))
			ibs.push_back(ib);
	}

	return hr;
}
HRESULT ModelManagement::createIndexBuffer(
	const unsigned int	modelID,
	MeshSubset&			subset,
	IB*					ib,
	ID3D11Device*		device)
{
	HRESULT hr = S_OK;

	hr = ib->init(subset.getIndices(), device);
	if(FAILED(hr))
	{
		std::string failed = "Failed to create Index Buffer from MeshModel at index: " + modelID;
		SHOW_MESSAGEBOX(failed);
	}

	return hr;
}

void ModelManagement::pushMeshModelD3D(
	const unsigned int	modelID, 
	ModelD3D*		meshModelD3D)
{
	meshModelD3Ds_.push_back(meshModelD3D);

	unsigned int meshModelD3DIndex = meshModelD3Ds_.size() - 1;
	map.insert(std::pair<unsigned int, unsigned int>(modelID, meshModelD3DIndex));
}

bool ModelManagement::existingMeshModelD3D(const int unsigned modelID)
{
	bool valExists = false;

	std::map<unsigned int, unsigned int>::iterator it = map.find(modelID);
	if(it != map.end())
		valExists = true;

	return valExists;
}

unsigned int ModelManagement::getMeshModelD3DIndex(const int unsigned modelID)
{
	std::map<unsigned int, unsigned int>::iterator it = map.find(modelID);

	return (*it).second;
}