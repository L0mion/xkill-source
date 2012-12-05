#include <d3d11.h>
#include <Windows.h>

#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/AttributeType.h>
#include <xkill-utilities/MeshModel.h>

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

	return meshModelD3Ds_[getMeshModelD3DIndex(index)];
}

HRESULT MeshManagement::createMeshModelD3D(
	const unsigned int	index, 
	ID3D11Device*		device)
{
	HRESULT hr = S_OK;

	std::vector<MeshAttribute>*	allMesh; GET_ATTRIBUTES(allMesh, MeshAttribute,	ATTRIBUTE_MESH);
	if(index < allMesh->size())
	{
		MeshAttribute* meshAt	= &allMesh->at(index);
		MeshModel* model		= meshAt->mesh;

		ID3D11Buffer* vertexBuffer = nullptr;
		std::vector<ID3D11Buffer*> indexBuffers;
		hr = createVertexBuffer(
			index, 
			model->getGeometry(), 
			vertexBuffer, 
			device);
		if(SUCCEEDED(hr))
		{
			hr = createIndexBuffers(
				index, 
				model->getGeometry(), 
				indexBuffers, 
				device);
		}
		if(SUCCEEDED(hr))
		{
			pushMeshModelD3D(index,
			new MeshModelD3D(vertexBuffer, indexBuffers));
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
		ID3D11Buffer*		vertexBuffer,
		ID3D11Device*		device)
{
	HRESULT hr = S_OK;

	std::vector<VertexPosNormTex> vertices = geometry.getVertices();

	D3D11_BUFFER_DESC vbd;
	vbd.Usage			= D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth		= sizeof(VertexPosNormTex) * vertices.size();
	vbd.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags		= 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices.at(0);
	hr = device->CreateBuffer(&vbd, &vinitData, &vertexBuffer);
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
	std::vector<ID3D11Buffer*>&		indexBuffers,
	ID3D11Device*					device)
{
	HRESULT hr = S_OK;

	std::vector<MeshSubset> subsets = geometry.getSubsets();
	for(unsigned int i = 0; i < subsets.size() && !FAILED(hr); i++)
	{
		ID3D11Buffer* indexBuffer = nullptr;
		hr = createIndexBuffer(
			index,
			subsets[i],
			indexBuffer,
			device);
		if(SUCCEEDED(hr))
			indexBuffers.push_back(indexBuffer);
	}

	return hr;
}
HRESULT MeshManagement::createIndexBuffer(
	const unsigned int	index,
	MeshSubset&			subset,
	ID3D11Buffer*		indexBuffer,
	ID3D11Device*		device)
{
	HRESULT hr = S_OK;

	std::vector<unsigned int> indices = subset.getIndices();

	D3D11_BUFFER_DESC ibd;
	ibd.Usage			= D3D11_USAGE_DYNAMIC;
	ibd.ByteWidth		= sizeof(unsigned int) * indices.size();
	ibd.BindFlags		= D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
	ibd.MiscFlags		= 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &indices.at(0);
	hr = device->CreateBuffer(&ibd, &vinitData, &indexBuffer);
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
	map.insert(std::pair<unsigned int, unsigned int>(index, meshModelD3Ds_.size()));
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