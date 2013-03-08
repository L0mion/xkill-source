#include <d3d11.h>

#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/AttributeType.h>

#include "ModelD3D.h"
#include "SubsetD3D.h"
#include "VB.h"
#include "IB.h"
#include "renderingUtilities.h"
#include "ManagementModel.h"

ATTRIBUTES_DECLARE_ALL

ManagementModel::ManagementModel()
{
	ATTRIBUTES_INIT_ALL

}
ManagementModel::~ManagementModel()
{
	//Delete our models.
	for(unsigned int i = 0; i < modelD3Ds_.size(); i++)
	{
		if(modelD3Ds_[i])
			delete modelD3Ds_[i];
	}
}

void ManagementModel::unloadModels()
{
	//Delete our models.
	for(unsigned int i = 0; i < modelD3Ds_.size(); i++)
	{
		if(modelD3Ds_[i])
			delete modelD3Ds_[i];
	}
	modelD3Ds_.clear();

	modelIDtoIndex_.clear();
}

ModelD3D* ManagementModel::getModelD3D(
	const unsigned int	modelID, 
	ID3D11Device*		device)	
{
	HRESULT hr = S_OK;

	if(!existingModelD3D(modelID))
	{
		hr = createModelD3D(modelID, device);
	}

	ModelD3D* model = nullptr;
	if(hr == S_OK)
	{
		unsigned int modelD3DIndex = getModelD3DIndex(modelID);
		model = modelD3Ds_[modelD3DIndex];
	}

	return model;
}

HRESULT ManagementModel::createModelD3D(
	const unsigned int	modelID, 
	ID3D11Device*		device)
{
	HRESULT hr = S_OK;

	AttributePtr<Attribute_Mesh> ptr_mesh;
	if(getMeshAttribute(modelID, ptr_mesh))
	{
		MeshDesc model = ptr_mesh->mesh;
		VertexType vertexType = ptr_mesh->vertexType;

		ID3D11Buffer* vertexBuffer;
		hr = createVertexBuffer(
			modelID, 
			model.vertices_, 
			&vertexBuffer,
			ptr_mesh->vertexType,
			device);

		std::vector<SubsetD3D*>	subsetD3Ds;
		if(SUCCEEDED(hr))
		{
			hr = createIndexBuffers(
				modelID, 
				model.subsets_, 
				subsetD3Ds, 
				device);
		}
		if(SUCCEEDED(hr))
		{
			pushModelD3D(
				modelID,
				new ModelD3D(vertexType, vertexBuffer, subsetD3Ds, model.materials_));
		}
	}
	else
	{
		hr = S_FALSE;
		ERROR_MSG(L"ManagementModel::createModelD3D Could not find a MeshAttribute corresponding to modelID!" + modelID);
	}

	return hr;
}
bool ManagementModel::getMeshAttribute(unsigned int modelID, AttributePtr<Attribute_Mesh>& inout)
{
	bool foundAt = false;
	AttributePtr<Attribute_Mesh> attr_mesh;

	while(!foundAt && itrMesh.hasNext())
	{
		attr_mesh = itrMesh.getNext();

		if(attr_mesh->meshID == modelID)
		{
			inout = attr_mesh;
			foundAt = true;
		}
	}
	itrMesh.resetIndex();

	return foundAt;
}
HRESULT ManagementModel::createVertexBuffer(
		const unsigned int		modelID, 
		std::vector<VertexDesc>	vertices,
		ID3D11Buffer**			vertexBuffer,
		VertexType				vertexType,
		ID3D11Device*			device)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC vbd;
	vbd.Usage			= D3D11_USAGE_IMMUTABLE;
	vbd.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags	= 0;
	vbd.MiscFlags		= 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	switch(vertexType)
	{
	case VERTEX_INVALID:
		{
			std::string failed = "ManagementModel::CreateVertexBuffer Invalid vertex type for model: " + modelID;
			ERROR_MESSAGEBOX(failed);
			break;
		}
	case VERTEX_TYPE_POS_COLOR:
		{
			std::vector<VertexPosColor> convertedVertices = convertVertexPosColor(vertices);
			vbd.ByteWidth = sizeof(VertexPosColor) * convertedVertices.size();
			vinitData.pSysMem = &convertedVertices.at(0);
			hr = device->CreateBuffer(&vbd, &vinitData, vertexBuffer);
			break;
		}
	case VERTEX_TYPE_POS_NORM_TEX:
		{
			std::vector<VertexPosNormTex> convertedVertices = convertVertexPosNormTex(vertices);
			vbd.ByteWidth = sizeof(VertexPosNormTex) * convertedVertices.size();
			vinitData.pSysMem = &convertedVertices.at(0);
			hr = device->CreateBuffer(&vbd, &vinitData, vertexBuffer);
			break;
		}
		
		
	case VERTEX_TYPE_POS_NORM_SKINNED:
		{
			std::vector<VertexPosNormSkinned> convertedVertices = convertVertexPosNormSkinned(vertices);
			vbd.ByteWidth = sizeof(VertexPosNormSkinned) * convertedVertices.size();
			vinitData.pSysMem = &convertedVertices.at(0);
			hr = device->CreateBuffer(&vbd, &vinitData, vertexBuffer);
			break;
		}
		
		
	case VERTEX_TYPE_POS_NORM_TEX_SKINNED:
		{
			std::vector<VertexPosNormTexSkinned> convertedVertices = convertVertexPosNormTexSkinned(vertices);
			vbd.ByteWidth = sizeof(VertexPosNormTexSkinned) * convertedVertices.size();
			vinitData.pSysMem = &convertedVertices.at(0);
			hr = device->CreateBuffer(&vbd, &vinitData, vertexBuffer);
			break;
		}
		
	case VERTEX_TYPE_POS_NORM_TEX_TAN_SKINNED:
		{
			std::vector<VertexPosNormTexTanSkinned> convertedVertices = convertVertexPosNormTexTanSkinned(vertices);
			vbd.ByteWidth = sizeof(VertexPosNormTexTanSkinned) * convertedVertices.size();
			vinitData.pSysMem = &convertedVertices.at(0);
			hr = device->CreateBuffer(&vbd, &vinitData, vertexBuffer);
			break;
		}
	case VERTEX_TYPE_POS_NORM_TEX_TAN:
		{
			std::vector<VertexPosNormTexTan> convertedVertices = convertVertexPosNormTexTan(vertices);
			vbd.ByteWidth = sizeof(VertexPosNormTexTan) * convertedVertices.size();
			vinitData.pSysMem = &convertedVertices.at(0);
			hr = device->CreateBuffer(&vbd, &vinitData, vertexBuffer);
			break;
		}
	default:
		std::string failed = "ManagementModel::CreateVertexBuffer Unknown vertex type.";
		ERROR_MESSAGEBOX(failed);
		break;
	}

	if(FAILED(hr))
	{
		std::string failed = "ManagementModel::CreateVertexBuffer Failed to create Vertex Buffer from MeshModel ID: " + modelID;
		ERROR_MESSAGEBOX(failed);
	}

	return hr;
}
HRESULT ManagementModel::createIndexBuffers(
	const unsigned int			modelID, 
	std::vector<SubsetDesc>		subsets, 
	std::vector<SubsetD3D*>&	subsetD3Ds,
	ID3D11Device*				device)
{
	HRESULT hr = S_OK;

	for(unsigned int i = 0; i < subsets.size() && !FAILED(hr); i++)
	{
		ID3D11Buffer* indexBuffer;
		hr = createIndexBuffer(
			modelID,
			subsets[i],
			&indexBuffer,
			device);
		if(SUCCEEDED(hr))
		{
			SubsetD3D* subsetD3D = new SubsetD3D(
				subsets[i].materialIndex_, 
				subsets[i].indices_.size(), 
				indexBuffer);
			subsetD3Ds.push_back(subsetD3D);
		}
	}

	return hr;
}
HRESULT ManagementModel::createIndexBuffer(
	const unsigned int	modelID,
	SubsetDesc			subset,
	ID3D11Buffer**		indexBuffer,
	ID3D11Device*		device)
{
	HRESULT hr = S_OK;

	std::vector<unsigned int> indices = subset.indices_;
	if(indices.size() <= 0)
		hr = S_FALSE;

	D3D11_BUFFER_DESC ibd;
	ibd.Usage			= D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth		= sizeof(unsigned int) * indices.size();
	ibd.BindFlags		= D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags	= 0;
	ibd.MiscFlags		= 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &indices.at(0);
	hr = device->CreateBuffer(&ibd, &vinitData, indexBuffer);

	if(FAILED(hr))
	{
		std::string failed = "Failed to create Index Buffer from MeshModel at index: " + modelID;
		ERROR_MESSAGEBOX(failed);
	}

	return hr;
}

void ManagementModel::pushModelD3D(
	const unsigned int	modelID, 
	ModelD3D*		meshModelD3D)
{
	modelD3Ds_.push_back(meshModelD3D);

	unsigned int meshModelD3DIndex = modelD3Ds_.size() - 1;
	modelIDtoIndex_.insert(std::pair<unsigned int, unsigned int>(modelID, meshModelD3DIndex));
}

std::vector<VertexPosColor> ManagementModel::convertVertexPosColor(std::vector<VertexDesc>& vertices)
{
	std::vector<VertexPosColor> convertedVertices(vertices.size());
	for(unsigned int i = 0; i < vertices.size(); i++)
	{
		VertexPosColor vertex = VertexPosColor(vertices[i].position_, vertices[i].color_);
		convertedVertices[i] = vertex;
	}
	return convertedVertices;
}
std::vector<VertexPosNormTex> ManagementModel::convertVertexPosNormTex(std::vector<VertexDesc>& vertices)
{
	std::vector<VertexPosNormTex> convertedVertices(vertices.size());
	for(unsigned int i = 0; i < vertices.size(); i++)
	{
		VertexPosNormTex vertex = VertexPosNormTex(
			vertices[i].position_, 
			vertices[i].normal_,
			vertices[i].textureCoordinates_);
		convertedVertices[i] = vertex;
	}
	return convertedVertices;
}
std::vector<VertexPosNormTexSkinned> ManagementModel::convertVertexPosNormTexSkinned(std::vector<VertexDesc>& vertices)
{
	std::vector<VertexPosNormTexSkinned> convertedVertices(vertices.size());
	for(unsigned int i = 0; i < vertices.size(); i++)
	{
		VertexPosNormTexSkinned vertex = VertexPosNormTexSkinned(
			vertices[i].position_, 
			vertices[i].normal_,
			vertices[i].textureCoordinates_,
			vertices[i].weights_,
			vertices[i].boneIndices_);
		convertedVertices[i] = vertex;
	}
	return convertedVertices;
}
std::vector<VertexPosNormSkinned> ManagementModel::convertVertexPosNormSkinned(std::vector<VertexDesc>& vertices)
{
	std::vector<VertexPosNormSkinned> convertedVertices(vertices.size());
	for(unsigned int i = 0; i < vertices.size(); i++)
	{
		VertexPosNormSkinned vertex = VertexPosNormSkinned(
			vertices[i].position_, 
			vertices[i].normal_,
			vertices[i].weights_,
			vertices[i].boneIndices_);
		convertedVertices[i] = vertex;
	}
	return convertedVertices;
}
std::vector<VertexPosNormTexTanSkinned> ManagementModel::convertVertexPosNormTexTanSkinned(std::vector<VertexDesc>& vertices)
{
	std::vector<VertexPosNormTexTanSkinned> convertedVertices(vertices.size());
	for(unsigned int i = 0; i < vertices.size(); i++)
	{
		VertexPosNormTexTanSkinned vertex = VertexPosNormTexTanSkinned(
			vertices[i].position_, 
			vertices[i].normal_,
			vertices[i].textureCoordinates_,
			vertices[i].tangent_,
			vertices[i].weights_,
			vertices[i].boneIndices_);
		convertedVertices[i] = vertex;
	}
	return convertedVertices;
}
std::vector<VertexPosNormTexTan> ManagementModel::convertVertexPosNormTexTan(std::vector<VertexDesc>& vertices)
{
	std::vector<VertexPosNormTexTan> convertedVertices(vertices.size());
	for(unsigned int i = 0; i < vertices.size(); i++)
	{
		VertexPosNormTexTan vertex = VertexPosNormTexTan(
			vertices[i].position_, 
			vertices[i].normal_,
			vertices[i].textureCoordinates_,
			vertices[i].tangent_);
		convertedVertices[i] = vertex;
	}
	return convertedVertices;
}

bool ManagementModel::existingModelD3D(const int unsigned modelID)
{
	bool valExists = false;

	std::map<unsigned int, unsigned int>::iterator it = modelIDtoIndex_.find(modelID);
	if(it != modelIDtoIndex_.end())
		valExists = true;

	return valExists;
}
unsigned int ManagementModel::getModelD3DIndex(const int unsigned modelID)
{
	unsigned int index = 0;
	std::map<unsigned int, unsigned int>::iterator it = modelIDtoIndex_.find(modelID);
	if(it != modelIDtoIndex_.end())
		index = (*it).second;

	return index;
}