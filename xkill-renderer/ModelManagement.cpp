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
	const unsigned int	index, 
	ID3D11Device*		device)	
{
	if(!existingMeshModelD3DIndex(index))
	{
		HRESULT hr = S_OK;
		hr = createMeshModelD3D(index, device);
	}

	unsigned int meshModelD3DIndex = getMeshModelD3DIndex(index);
	return meshModelD3Ds_[meshModelD3DIndex];
}

HRESULT ModelManagement::createMeshModelD3D(
	const unsigned int	index, 
	ID3D11Device*		device)
{
	HRESULT hr = S_OK;

	std::vector<MeshAttribute>*	allMesh; GET_ATTRIBUTES(allMesh, MeshAttribute,	ATTRIBUTE_MESH);
	if(index < allMesh->size())
	{
		MeshAttribute*	meshAt	= &allMesh->at(index);
		MeshModel*		model	= meshAt->mesh;

		VB*					vb = new VB();
		std::vector<IB*>	ibs;

		hr = createVertexBuffer(
			index, 
			model->getGeometry(), 
			vb,
			device);
		if(SUCCEEDED(hr))
		{
			hr = createIndexBuffers(
				index, 
				model->getGeometry(), 
				ibs, 
				device);
		}
		if(SUCCEEDED(hr))
		{
			pushMeshModelD3D(index,
			new ModelD3D(vb, ibs));
		}
	}
	else
	{ /*Error*/
		std::string failed = "Nonexistant index in Mesh-Attributes. Index: " + index;
		SHOW_MESSAGEBOX(failed);
	}

	return hr;
}
HRESULT ModelManagement::createVertexBuffer(
		const unsigned int	index, 
		MeshGeometry&		geometry,
		VB*					vb,
		ID3D11Device*		device)
{
	HRESULT hr = S_OK;

	hr = vb->init(geometry.getVertices(), device);
	if(FAILED(hr))
	{
		std::string failed = "Failed to create Vertex Buffer from MeshModel at index: " + index;
		SHOW_MESSAGEBOX(failed);
	}

	return hr;
}
HRESULT ModelManagement::createIndexBuffers(
	const unsigned int				index, 
	MeshGeometry&					geometry, 
	std::vector<IB*>&				ibs,
	ID3D11Device*					device)
{
	HRESULT hr = S_OK;

	std::vector<MeshSubset> subsets = geometry.getSubsets();
	for(unsigned int i = 0; i < subsets.size() && !FAILED(hr); i++)
	{
		IB* ib = new IB();
		hr = createIndexBuffer(
			index,
			subsets[i],
			ib,
			device);
		if(SUCCEEDED(hr))
			ibs.push_back(ib);
	}

	return hr;
}
HRESULT ModelManagement::createIndexBuffer(
	const unsigned int	index,
	MeshSubset&			subset,
	IB*					ib,
	ID3D11Device*		device)
{
	HRESULT hr = S_OK;

	hr = ib->init(subset.getIndices(), device);
	if(FAILED(hr))
	{
		std::string failed = "Failed to create Index Buffer from MeshModel at index: " + index;
		SHOW_MESSAGEBOX(failed);
	}

	return hr;
}

void ModelManagement::pushMeshModelD3D(
	const unsigned int	index, 
	ModelD3D*		meshModelD3D)
{
	meshModelD3Ds_.push_back(meshModelD3D);

	unsigned int meshModelD3DIndex = meshModelD3Ds_.size() - 1;
	map.insert(std::pair<unsigned int, unsigned int>(index, meshModelD3DIndex));
}

bool ModelManagement::existingMeshModelD3DIndex(const int unsigned index)
{
	bool valExists = false;

	std::map<unsigned int, unsigned int>::iterator it = map.find(index);
	if(it != map.end())
		valExists = true;

	return valExists;
}

unsigned int ModelManagement::getMeshModelD3DIndex(const int unsigned index)
{
	std::map<unsigned int, unsigned int>::iterator it = map.find(index);

	return (*it).second;
}