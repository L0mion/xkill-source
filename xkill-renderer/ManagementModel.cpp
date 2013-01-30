#include <d3d11.h>
#include <Windows.h>

#include <xkill-utilities/Util.h>
#include <xkill-utilities/MeshModel.h>
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
		MeshModel* model = meshAt.mesh;

		VB<VertexPosNormTex>* vb = new VB<VertexPosNormTex>();
		std::vector<SubsetD3D*>	subsetD3Ds;

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
				subsetD3Ds, 
				device);
		}
		if(SUCCEEDED(hr))
		{
			pushModelD3D(
				modelID,
				new ModelD3D(vb, subsetD3Ds, model->getMaterials()));
		}
	}
	else
	{
		std::stringstream ss;
		ss << "ManagementModel::createModelD3D Could not find model with id ";
		ss << modelID;

		std::string str;
		str = ss.str();
		
		ERROR_MSG(str);
		hr = S_FALSE;
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
		MeshGeometry&			geometry,
		VB<VertexPosNormTex>*	vb,
		ID3D11Device*			device)
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
HRESULT ManagementModel::createIndexBuffers(
	const unsigned int			modelID, 
	MeshGeometry&				geometry, 
	std::vector<SubsetD3D*>&	subsetD3Ds,
	ID3D11Device*				device)
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
		{
			SubsetD3D* subsetD3D = new SubsetD3D(subsets[i].getMaterialIndex(), ib);
			subsetD3Ds.push_back(subsetD3D);
		}
	}

	return hr;
}
HRESULT ManagementModel::createIndexBuffer(
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