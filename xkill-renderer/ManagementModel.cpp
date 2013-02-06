#include <d3d11.h>
#include <Windows.h>

#include <xkill-utilities/Util.h>
#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/AttributeType.h>
#include <xkill-utilities/DebugShape.h>

#include "ModelD3D.h"
#include "SubsetD3D.h"
#include "DebugShapeD3D.h"
#include "DebugShapes.h"
#include "VB.h"
#include "IB.h"
#include "renderingUtilities.h"
#include "ManagementModel.h"

#include <sstream>

ATTRIBUTES_DECLARE_ALL

ManagementModel::ManagementModel()
{
	ATTRIBUTES_INIT_ALL

	debugShapes_ = nullptr;
}
ManagementModel::~ManagementModel()
{
	if(debugShapes_)
		delete debugShapes_;

	//Delete our models.
	for(unsigned int i = 0; i < modelD3Ds_.size(); i++)
	{
		if(modelD3Ds_[i])
			delete modelD3Ds_[i];
	}

	//Delete our debug-shapes.
	for(unsigned int i = 0; i < debugShapeD3Ds_.size(); i++)
	{
		if(debugShapeD3Ds_[i])
			delete debugShapeD3Ds_[i];
	}
}

HRESULT ManagementModel::init()
{
	HRESULT hr = S_OK;

	debugShapes_ = new DebugShapes();

	return hr;
}

ModelD3D* ManagementModel::getModelD3D(
	const unsigned int	modelID, 
	ID3D11Device*		device)	
{
	if(!existingModelD3D(modelID))
	{
		HRESULT hr = S_OK;
		hr = createModelD3D(modelID, device);
	}

	unsigned int modelD3DIndex = getModelD3DIndex(modelID);
	return modelD3Ds_[modelD3DIndex];
}
DebugShapeD3D* ManagementModel::getDebugShapeD3D(
	const unsigned int	shapeIndex,
	ID3D11Device*		device)
{
	if(!existingDebugShapeD3D(shapeIndex))
		createDebugShapeD3D(shapeIndex, device);

	unsigned int debugShapeD3DIndex = getDebugShapeD3DIndex(shapeIndex);
	return debugShapeD3Ds_[debugShapeD3DIndex];
}

HRESULT ManagementModel::createModelD3D(
	const unsigned int	modelID, 
	ID3D11Device*		device)
{
	HRESULT hr = S_OK;

	Attribute_Mesh meshAt;
	if(getMeshAttribute(modelID, meshAt))
	{
		MeshDesc model = meshAt.mesh;

		ID3D11Buffer* vertexBuffer;
		hr = createVertexBuffer(
			modelID, 
			model.vertices_, 
			&vertexBuffer,
			meshAt.vertexType,
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
				new ModelD3D(vertexBuffer, subsetD3Ds, model.materials_));
		}
	}
	else
		hr = S_FALSE;
	
	if(FAILED(hr))
	{
		ERROR_MSG(L"ManagementModel::createModelD3D An error occured!");
	}

	return hr;
}
bool ManagementModel::getMeshAttribute(unsigned int modelID, Attribute_Mesh& inout)
{
	bool foundAt = false;
	Attribute_Mesh* attr_mesh;

	while(!foundAt && itrMesh.hasNext())
	{
		attr_mesh = itrMesh.getNext();

		if(attr_mesh->meshID == modelID)
		{
			inout = *attr_mesh;
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
	vbd.CPUAccessFlags	= 0; //D3D11_CPU_ACCESS_WRITE
	vbd.MiscFlags		= 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	switch(vertexType)
	{
	case VERTEX_INVALID:
		{
			std::string failed = "ManagementModel::CreateVertexBuffer Invalid vertex type for model: " + modelID;
			SHOW_MESSAGEBOX(failed);
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
	default:
		std::string failed = "ManagementModel::CreateVertexBuffer Unknown vertex type.";
		SHOW_MESSAGEBOX(failed);
		break;
	}

	if(FAILED(hr))
	{
		std::string failed = "ManagementModel::CreateVertexBuffer Failed to create Vertex Buffer from MeshModel ID: " + modelID;
		SHOW_MESSAGEBOX(failed);
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
	ibd.CPUAccessFlags	= 0; //D3D11_CPU_ACCESS_WRITE
	ibd.MiscFlags		= 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &indices.at(0);
	hr = device->CreateBuffer(&ibd, &vinitData, indexBuffer);

	if(FAILED(hr))
	{
		std::string failed = "Failed to create Index Buffer from MeshModel at index: " + modelID;
		SHOW_MESSAGEBOX(failed);
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
void ManagementModel::pushDebugShapeD3D(
	const unsigned int shapeIndex, 
	DebugShapeD3D* shapeD3D)
{
	debugShapeD3Ds_.push_back(shapeD3D);

	unsigned int debugShapeD3DIndex = debugShapeD3Ds_.size() - 1;
	shapeIndextoD3DIndex_.insert(std::pair<unsigned int, unsigned int>(shapeIndex, debugShapeD3DIndex));
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

void ManagementModel::createDebugShapeD3D(unsigned int shapeIndex, ID3D11Device* device)
{
	Attribute_DebugShape* debugShapeAt = itrDebugShape.at(shapeIndex);
	DebugShape* shape = debugShapeAt->shape;

	switch(shape->shapeType_)
	{
	case DEBUG_SHAPE_SPHERE:
		pushDebugShapeD3D(shapeIndex, createSphere((DebugShapeSphere*)shape, device));
		break;
	case DEBUG_SHAPE_BB:
		pushDebugShapeD3D(shapeIndex, createBB((DebugShapeBB*)shape, device));
		break;
	case DEBUG_SHAPE_FRUSTUM:
		pushDebugShapeD3D(shapeIndex, createFrustum((DebugShapeFrustum*)shape, device));
		break;
	case DEBUG_SHAPE_NA:
		//Log warning
		break;
	default:
		SHOW_MESSAGEBOX("ManagementModel::createDebugShapeD3D: Failed to load DebugShapeD3D, no regognized DEBUG_SHAPE-enum.");
		break;
	}
}
DebugShapeD3D* ManagementModel::createSphere(DebugShapeSphere* sphere, ID3D11Device* device)
{
	DebugShapeD3D* debugSphere = nullptr;

	float sphereRadius = sphere->radius_;
	std::vector<VertexPosColor> sphereVertices = debugShapes_->getSphere(sphereRadius);

	VB<VertexPosColor>* vb = new VB<VertexPosColor>();
	vb->init(sphereVertices, device);

	debugSphere = new DebugShapeD3D(vb);
	return debugSphere;
}
DebugShapeD3D* ManagementModel::createBB(DebugShapeBB* bb, ID3D11Device* device)
{
	DebugShapeD3D* debugBB = nullptr;

	Float3 bbMin = bb->bbMin_;
	Float3 bbMax = bb->bbMax_;
	std::vector<VertexPosColor> bbVertices = debugShapes_->getBB(bbMin, bbMax);

	VB<VertexPosColor>* vb = new VB<VertexPosColor>();
	vb->init(bbVertices, device);

	debugBB = new DebugShapeD3D(vb);
	return debugBB;
}
DebugShapeD3D* ManagementModel::createFrustum(DebugShapeFrustum* frustum, ID3D11Device* device)
{
	DebugShapeD3D* debugFrustum = nullptr;

	std::vector<VertexPosColor> bbVertices = debugShapes_->getFrustum(
		frustum->p[0],
		frustum->p[1],
		frustum->p[2],
		frustum->p[3],
		frustum->p[4],
		frustum->p[5],
		frustum->p[6],
		frustum->p[7]);

	VB<VertexPosColor>* vb = new VB<VertexPosColor>();
	vb->init(bbVertices, device);

	debugFrustum = new DebugShapeD3D(vb);
	return debugFrustum;
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
	std::map<unsigned int, unsigned int>::iterator it = modelIDtoIndex_.find(modelID);
	return (*it).second;
}

bool ManagementModel::existingDebugShapeD3D(const unsigned int shapeIndex)
{
	bool valExists = false;

	std::map<unsigned int, unsigned int>::iterator it = shapeIndextoD3DIndex_.find(shapeIndex);
	if(it != shapeIndextoD3DIndex_.end())
		valExists = true;

	return valExists;
}
unsigned int ManagementModel::getDebugShapeD3DIndex(const int unsigned shapeIndex)
{
	std::map<unsigned int, unsigned int>::iterator it = shapeIndextoD3DIndex_.find(shapeIndex);
	return (*it).second;
}