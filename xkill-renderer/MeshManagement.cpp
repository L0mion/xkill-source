#include <d3d11.h>
#include <Windows.h>

#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/AttributeType.h>
#include <xkill-utilities/MeshModel.h>

#include "VB.h"
#include "IB.h"
#include "renderingUtilities.h"
#include "MeshModelD3D.h"
#include "MeshManagement.h"

MeshManagement::MeshManagement()
{
	//Do nothing.
}
MeshManagement::~MeshManagement()
{
	for(unsigned int i = 0; i < meshModelD3Ds_.size(); i++)
	{
		if(meshModelD3Ds_[i])
			delete meshModelD3Ds_[i];
	}
}

MeshModelD3D* MeshManagement::getMeshModelD3D(
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

HRESULT MeshManagement::createMeshModelD3D(
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
			new MeshModelD3D(vb, ibs));
		}
	}
	else
	{ /*Error*/
		std::string failed = "Nonexistant index in Mesh-Attributes. Index: " + index;
		SHOW_MESSAGEBOX(failed);
	}

	return hr;
}
HRESULT MeshManagement::createVertexBuffer(
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
HRESULT MeshManagement::createIndexBuffers(
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
HRESULT MeshManagement::createIndexBuffer(
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

void MeshManagement::pushMeshModelD3D(
	const unsigned int	index, 
	MeshModelD3D*		meshModelD3D)
{
	meshModelD3Ds_.push_back(meshModelD3D);

	unsigned int meshModelD3DIndex = meshModelD3Ds_.size() - 1;
	map.insert(std::pair<unsigned int, unsigned int>(index, meshModelD3DIndex));
}

bool MeshManagement::existingMeshModelD3DIndex(const int unsigned index)
{
	bool valExists = false;

	std::map<unsigned int, unsigned int>::iterator it = map.find(index);
	if(it != map.end())
		valExists = true;

	return valExists;
}

unsigned int MeshManagement::getMeshModelD3DIndex(const int unsigned index)
{
	std::map<unsigned int, unsigned int>::iterator it = map.find(index);

	return (*it).second;
}