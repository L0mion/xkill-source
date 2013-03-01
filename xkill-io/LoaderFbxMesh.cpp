#include <sstream>

#include<DirectXMath.h>

#include <fbxsdk.h>

#include <xkill-utilities/Util.h>
#include <xkill-utilities/MeshVertices.h>

#include "LoaderFbxMesh.h"
#include "LoaderFbxMeshDesc.h"

LoaderFbxMesh::LoaderFbxMesh()
{
}
LoaderFbxMesh::~LoaderFbxMesh()
{
}

void LoaderFbxMesh::reset()
{
	polygonGroupIds_.clear();
	vertexPositions_.clear();
	vertexColors_.clear();	 
	vertexNormals_.clear();	 
	vertexUVs_.clear();		 
	vertexTangents_.clear(); 
	vertexBinormals_.clear();
}

void LoaderFbxMesh::parseMesh(FbxMesh* mesh, FbxPose* fbxPose, LoaderFbxMeshDesc* meshDesc)
{
	reset();

	meshDesc->setFbxMesh(mesh);

	int polygonVertexCount = mesh->GetPolygonVertexCount();
	int polygonCount = mesh->GetPolygonCount();
	int numControlPonts = mesh->GetControlPointsCount();

	FbxVector4* controlPoints = mesh->GetControlPoints();
	
	int vertexId = 0;

	for(int polygonIndex=0; polygonIndex<polygonCount; polygonIndex++)
	{
		parsePolygonGroup(mesh, polygonIndex);

		int polygonSize = mesh->GetPolygonSize(polygonIndex);

		for(int insidePolygonIndex=0; insidePolygonIndex<polygonSize; insidePolygonIndex++)
		{
			int controlPointIndex = mesh->GetPolygonVertex(polygonIndex, insidePolygonIndex);

			parseVertexPositions(mesh, controlPoints, controlPointIndex);
			parseVertexColors(mesh, controlPointIndex, vertexId);
			parseVertexNormals(mesh, controlPointIndex, vertexId);
			parseVertexUVs(mesh, polygonIndex, insidePolygonIndex, controlPointIndex);
			parseVertexTangents(mesh, controlPointIndex, vertexId);
			parseVertexBinormals(mesh, controlPointIndex, vertexId);
			vertexId++;
		}
	}


//	transformVertices(mesh);

	meshDesc->setPolygonGroupIds(polygonGroupIds_);
	meshDesc->setVertexPositions(vertexPositions_);
	meshDesc->setVertexNormals(vertexNormals_);
	meshDesc->setVertexUVs(vertexUVs_);
	meshDesc->setVertexTangents(vertexTangents_);
	meshDesc->setVertexBinormals(vertexBinormals_);

	parseVertexLinkData(mesh, fbxPose, meshDesc);
	meshDesc->fillBoneData();
}

void LoaderFbxMesh::parsePolygonGroup(FbxMesh* mesh, int polygonIndex)
{
	for(int i=0; i<mesh->GetElementPolygonGroupCount(); i++)
	{
		FbxGeometryElementPolygonGroup* polyGroupElement = mesh->GetElementPolygonGroup(i);
		switch(polyGroupElement->GetMappingMode())
		{
		case FbxGeometryElement::eByPolygon:
			if(polyGroupElement->GetReferenceMode() == FbxGeometryElement::eIndex)
			{
				int polyGroupId = polyGroupElement->GetIndexArray().GetAt(polygonIndex);
				polygonGroupIds_.push_back(polyGroupId);
			}
			break;
		default:
			break;
		}
	}
}
void LoaderFbxMesh::parseVertexPositions(FbxMesh* mesh, FbxVector4* controlPoints, int controlPointIndex)
{
	FbxVector4  controlPoint = controlPoints[controlPointIndex];

	Float3 position;
	position.x = static_cast<float>(controlPoint.mData[0]);
	position.y = static_cast<float>(controlPoint.mData[1]);
	position.z = static_cast<float>(controlPoint.mData[2]);

	vertexPositions_.push_back(position);
}

void	 LoaderFbxMesh::parseVertexColors(FbxMesh* mesh, int controlPointIndex, int vertexId)
{
	for(int i=0; i<mesh->GetElementVertexColorCount(); i++)
	{
		FbxGeometryElementVertexColor* vertexColorElement = mesh->GetElementVertexColor(i);
		FbxColor fbxColor;
		switch(vertexColorElement->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
			fbxColor = parseVertexColorsByControlPoint(vertexColorElement, controlPointIndex);
			break;
		case FbxGeometryElement::eByPolygonVertex:
			fbxColor = parseVertexColorsByPolygonVertex(vertexColorElement, vertexId);
			break;
		default:
			break;
		}

		Float4 color;
		color.x = static_cast<float>(fbxColor.mRed);
		color.y = static_cast<float>(fbxColor.mGreen);
		color.z = static_cast<float>(fbxColor.mBlue);
		color.w = static_cast<float>(fbxColor.mAlpha);

		vertexColors_.push_back(color);
	}

}
FbxColor LoaderFbxMesh::parseVertexColorsByControlPoint(FbxGeometryElementVertexColor* vertexColorElement, int controlPointIndex)
{
	FbxColor fbxColor(0, 0, 0);
	int id;
	switch(vertexColorElement->GetReferenceMode())
	{
	case FbxGeometryElement::eDirect:
		 fbxColor = vertexColorElement->GetDirectArray().GetAt(controlPointIndex);
		break;
	case FbxGeometryElement::eIndexToDirect:
		id = vertexColorElement->GetIndexArray().GetAt(controlPointIndex);
		fbxColor = vertexColorElement->GetDirectArray().GetAt(id);
		break;
	default:
		break;
	}

	return fbxColor;
}
FbxColor LoaderFbxMesh::parseVertexColorsByPolygonVertex(FbxGeometryElementVertexColor* vertexColorElement, int vertexId)
{
	FbxColor fbxColor(0, 0, 0);
	int id;
	switch(vertexColorElement->GetReferenceMode())
	{
	case FbxGeometryElement::eDirect:
		fbxColor = vertexColorElement->GetDirectArray().GetAt(vertexId);
		break;
	case FbxGeometryElement::eIndexToDirect:
		id = vertexColorElement->GetIndexArray().GetAt(vertexId);
		fbxColor = vertexColorElement->GetDirectArray().GetAt(id);
		break;
	default:
		break;
	}
	
	return fbxColor;
}

void		LoaderFbxMesh::parseVertexNormals(FbxMesh* mesh, int controlPointIndex, int vertexId)
{
	for(int i=0; i<mesh->GetElementNormalCount(); i++)
	{
		FbxVector4 fbxNormal(0, 0, 0, 0);
		FbxGeometryElementNormal* normalElement = mesh->GetElementNormal(i);
		switch(normalElement->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
			fbxNormal = parseVertexNormalsByControlPoint(normalElement, controlPointIndex);
			break;
		case FbxGeometryElement::eByPolygonVertex:
			fbxNormal = parseVertexNormalsByPolygonVertex(normalElement, vertexId);
			break;
		default:
			break;
		}
		Float3 normal;
		normal.x = static_cast<float>(fbxNormal.mData[0]);
		normal.y = static_cast<float>(fbxNormal.mData[1]);
		normal.z = static_cast<float>(fbxNormal.mData[2]);
		vertexNormals_.push_back(normal);
	}
}
FbxVector4	LoaderFbxMesh::parseVertexNormalsByControlPoint(FbxGeometryElementNormal* normalElement, int controlPointIndex)
{
	FbxVector4 fbxNormal(0, 0, 0, 0);
	int id;
	switch(normalElement->GetReferenceMode())
	{
	case FbxGeometryElement::eDirect:
		fbxNormal = normalElement->GetDirectArray().GetAt(controlPointIndex);
		break;
	case FbxGeometryElement::eIndexToDirect:
		id = normalElement->GetIndexArray().GetAt(controlPointIndex);
		fbxNormal = normalElement->GetDirectArray().GetAt(id);
		break;
	default:
		break;
	}

	return fbxNormal;
}
FbxVector4	LoaderFbxMesh::parseVertexNormalsByPolygonVertex(FbxGeometryElementNormal* normalElement, int vertexId)
{
	FbxVector4 fbxNormal(0, 0, 0, 0);
	int id;
	switch(normalElement->GetReferenceMode())
	{
	case FbxGeometryElement::eDirect:
		fbxNormal = normalElement->GetDirectArray().GetAt(vertexId);
		break;
	case FbxGeometryElement::eIndexToDirect:
		id= normalElement->GetIndexArray().GetAt(vertexId);
		fbxNormal = normalElement->GetDirectArray().GetAt(id);
		break;
	default:
		break;
	}

	return fbxNormal;
}

void		LoaderFbxMesh::parseVertexUVs(FbxMesh* mesh, int polygonIndex, int insidePolygonIndex, int controlPointIndex)
{
	for(int i=0; i<mesh->GetElementUVCount(); i++)
	{
		FbxVector2 fbxUV;
		FbxGeometryElementUV* uvElement = mesh->GetElementUV(i);
		switch(uvElement->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
			fbxUV = parseVertexUVsByControlPoint(uvElement, controlPointIndex);
			break;
		case FbxGeometryElement::eByPolygonVertex:
			fbxUV = parseVertexUVsByPolygonVertex(mesh, uvElement, polygonIndex, insidePolygonIndex);
			break;
		default:
			break;
		}

		Float2 uv;
		uv.x = static_cast<float>(fbxUV.mData[0]);
		uv.y = static_cast<float>(fbxUV.mData[1]);
		vertexUVs_.push_back(uv);
	}
}
FbxVector2	LoaderFbxMesh::parseVertexUVsByControlPoint(FbxGeometryElementUV* uvElement, int controlPointIndex)
{
	FbxVector2 fbxUV(0, 0);
	int id;
	switch(uvElement->GetReferenceMode())
	{
	case FbxGeometryElement::eDirect:
		fbxUV = uvElement->GetDirectArray().GetAt(controlPointIndex);
		break;
	case FbxGeometryElement::eIndexToDirect:
		id = uvElement->GetIndexArray().GetAt(controlPointIndex);
		fbxUV = uvElement->GetDirectArray().GetAt(id);
		break;
	default:
		break;
	}

	return fbxUV;
}
FbxVector2	LoaderFbxMesh::parseVertexUVsByPolygonVertex(FbxMesh* mesh, FbxGeometryElementUV* uvElement, int polygonIndex, int insidePolygonIndex)
{
	FbxVector2 fbxUV;
	int uvIndex = mesh->GetTextureUVIndex(polygonIndex, insidePolygonIndex);
	switch(uvElement->GetReferenceMode())
	{
	case FbxGeometryElement::eDirect:
	case FbxGeometryElement::eIndexToDirect:
		fbxUV = uvElement->GetDirectArray().GetAt(uvIndex);
		break;
	default:
		break;
	}

	return fbxUV;
}

void		LoaderFbxMesh::parseVertexTangents(FbxMesh* mesh, int controlPointIndex, int vertexId)
{
	for(int i=0; i<mesh->GetElementTangentCount(); i++)
	{
		FbxVector4 fbxTangent(0, 0, 0, 0);
		FbxGeometryElementTangent* tangentElement = mesh->GetElementTangent(i);
		switch(tangentElement->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
			fbxTangent = parseVertexTangentsByControlPoint(tangentElement, controlPointIndex);
			break;
		case FbxGeometryElement::eByPolygonVertex:
			fbxTangent = parseVertexTangentsByPolygonVertex(tangentElement, vertexId);
			break;
		default:
			break;
		}

		Float4 tangent;
		tangent.x = static_cast<float>(fbxTangent.mData[0]);
		tangent.y = static_cast<float>(fbxTangent.mData[1]);
		tangent.z = static_cast<float>(fbxTangent.mData[2]);
		tangent.w = static_cast<float>(fbxTangent.mData[3]);
		vertexTangents_.push_back(tangent);
	}
}
FbxVector4	LoaderFbxMesh::parseVertexTangentsByControlPoint(FbxGeometryElementTangent* tangentElement, int controlPointIndex)
{
	FbxVector4 fbxTangent(0, 0, 0, 0);
	int id;
	switch(tangentElement->GetReferenceMode())
	{
	case FbxGeometryElement::eDirect:
		fbxTangent = tangentElement->GetDirectArray().GetAt(controlPointIndex);
		break;
	case FbxGeometryElement::eIndexToDirect:
		id = tangentElement->GetIndexArray().GetAt(controlPointIndex);
		fbxTangent = tangentElement->GetDirectArray().GetAt(id);
		break;
	default:
		break;
	}

	return fbxTangent;
}
FbxVector4	LoaderFbxMesh::parseVertexTangentsByPolygonVertex(FbxGeometryElementTangent* tangentElement, int vertexId)
{
	FbxVector4 fbxTangent(0, 0, 0, 0);
	int id;
	switch(tangentElement->GetReferenceMode())
	{
	case FbxGeometryElement::eDirect:
		fbxTangent = tangentElement->GetDirectArray().GetAt(vertexId);
		break;
	case FbxGeometryElement::eIndexToDirect:
		id = tangentElement->GetIndexArray().GetAt(vertexId);
		fbxTangent = tangentElement->GetDirectArray().GetAt(id);
		break;
	default:
		break;
	}

	return fbxTangent;
}

void		LoaderFbxMesh::parseVertexBinormals(FbxMesh* mesh, int controlPointIndex, int vertexId)
{
	for(int i=0; i<mesh->GetElementBinormalCount(); i++)
	{
		FbxVector4 fbxBinormal(0, 0, 0, 0);
		FbxGeometryElementBinormal* binormalElement = mesh->GetElementBinormal(i);
		switch(binormalElement->GetMappingMode())
		{
		case FbxGeometryElement::eByControlPoint:
			fbxBinormal = parseVertexBinormalsByControlPoint(binormalElement, controlPointIndex);
			break;
		case FbxGeometryElement::eByPolygonVertex:
			fbxBinormal = parseVertexBinormalsByPolygonVertex(binormalElement, vertexId);
			break;
		default:
			break;
		}
		Float4 binormal;
		binormal.x = static_cast<float>(fbxBinormal.mData[0]);
		binormal.y = static_cast<float>(fbxBinormal.mData[1]);
		binormal.z = static_cast<float>(fbxBinormal.mData[2]);
		binormal.w = static_cast<float>(fbxBinormal.mData[3]);
		vertexBinormals_.push_back(binormal);
	}
}
FbxVector4	LoaderFbxMesh::parseVertexBinormalsByControlPoint(FbxGeometryElementBinormal* binormalElement, int controlPointIndex)
{
	FbxVector4 fbxBinormal(0, 0, 0, 0);
	int id;
	switch(binormalElement->GetReferenceMode())
	{
	case FbxGeometryElement::eDirect:
		fbxBinormal = binormalElement->GetDirectArray().GetAt(controlPointIndex);
		break;
	case FbxGeometryElement::eIndexToDirect:
		id = binormalElement->GetIndexArray().GetAt(controlPointIndex);
		fbxBinormal = binormalElement->GetDirectArray().GetAt(id);
		break;
	default:
		break;
	}

	return fbxBinormal;
}
FbxVector4	LoaderFbxMesh::parseVertexBinormalsByPolygonVertex(FbxGeometryElementBinormal* binormalElement, int vertexId)
{
	FbxVector4 fbxBinormal(0, 0, 0, 0);
	int id;
	switch(binormalElement->GetReferenceMode())
	{
	case FbxGeometryElement::eDirect:
		fbxBinormal = binormalElement->GetDirectArray().GetAt(vertexId);
		break;
	case FbxGeometryElement::eIndexToDirect:
		id = binormalElement->GetIndexArray().GetAt(vertexId);
		fbxBinormal = binormalElement->GetDirectArray().GetAt(id);
		break;
	default:
		break;
	}

	return fbxBinormal;
}

void		LoaderFbxMesh::parseVertexLinkData(FbxMesh* mesh, FbxPose* fbxPose, LoaderFbxMeshDesc* meshDesc)
{
	int debug = mesh->GetControlPointsCount();
	meshDesc->prepareBoneData(mesh->GetControlPointsCount());
	FbxCluster* cluster = nullptr;
	
	int	numDeformers = mesh->GetDeformerCount(FbxDeformer::eSkin);
	int	numClusters	 = 0;
	int linkMode	 = 0;

	for(int deformerIndex=0; deformerIndex<numDeformers; deformerIndex++)
	{
		numClusters = static_cast<FbxSkin*>(mesh->GetDeformer(deformerIndex, FbxDeformer::eSkin))->GetClusterCount();
		if(numClusters > 0)
		{
			cluster = static_cast<FbxSkin*>(mesh->GetDeformer(deformerIndex, FbxDeformer::eSkin))->GetCluster(0);
		}
		if(cluster)
		{
			FbxNode* node = cluster->GetLink();
		
			node = findRoot(node);
			std::vector<FbxNode*> nodes;
			std::vector<int> parentIndices;
			parseLinkHierarchy(node, &nodes, &parentIndices);

			meshDesc->setBoneNodes(nodes);
			meshDesc->setBoneParentIndices(parentIndices);

			Float4x4 identityMatrix;
			identityMatrix._11 = 1.0f; identityMatrix._12 = 0.0f; identityMatrix._13 = 0.0f; identityMatrix._14 = 0.0f;
			identityMatrix._21 = 0.0f; identityMatrix._22 = 1.0f; identityMatrix._23 = 0.0f; identityMatrix._24 = 0.0f;
			identityMatrix._31 = 0.0f; identityMatrix._32 = 0.0f; identityMatrix._33 = 1.0f; identityMatrix._34 = 0.0f;
			identityMatrix._41 = 0.0f; identityMatrix._42 = 0.0f; identityMatrix._43 = 0.0f; identityMatrix._44 = 1.0f;

			for(unsigned int i=0; i<nodes.size(); i++)
			{
				//FbxAMatrix matrixTransform;
				//matrixTransform = nodes[i]->EvaluateGlobalTransform(FbxTime(0));
				//matrixTransform = getGlobalPosition(nodes[i], FbxTime(0), fbxPose, &matrixTransform);
				//
				//Float4x4 offsetMatrix;
				//for(int x=0; x<4; x++)
				//{
				//	for(int y=0; y<4; y++)
				//		offsetMatrix.m[x][y] = static_cast<float>(matrixTransform.mData[x][y]);
				//}

				meshDesc->addOffsetMatrix(identityMatrix);
			}

			for(int clusterIndex=0; clusterIndex<numClusters; clusterIndex++)
			{
				cluster = static_cast<FbxSkin*>(mesh->GetDeformer(deformerIndex, FbxDeformer::eSkin))->GetCluster(clusterIndex);
				unsigned int nodeIndex=0; 
				bool equal = false;
				while(!equal && nodeIndex<nodes.size())
				{
					if(nodes[nodeIndex]->GetUniqueID() == cluster->GetLink()->GetUniqueID())
						equal = true;
					else
						nodeIndex++;
				}
				if(equal)
				{
					parseIndicesAndWeights(cluster, meshDesc, nodeIndex);
					parseTransformMatrix(cluster, mesh, fbxPose, meshDesc, nodeIndex);
				}
			}
		}
	}
}
FbxNode*	LoaderFbxMesh::findRoot(FbxNode* node)
{
	bool done = false;
	while(!done)
	{
		if(node->GetParent()->GetNodeAttribute())
		{
			if(node->GetParent()->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eSkeleton)
			{
				node = node->GetParent();
			}
			else
				done = true;
		}
		else
			done = true;
	}
	
	return node;
}
void		LoaderFbxMesh::parseLinkHierarchy(FbxNode* rootNode, std::vector<FbxNode*>* nodes, std::vector<int>* parentIndices)
{
	nodes->push_back(rootNode);
	parentIndices->push_back(-1);
	for(unsigned int nodeIndex=0; nodeIndex<nodes->size(); nodeIndex++)
	{
		for(int childIndex=0; childIndex<nodes->at(nodeIndex)->GetChildCount(); childIndex++)
		{
			nodes->push_back(nodes->at(nodeIndex)->GetChild(childIndex));
			parentIndices->push_back(nodeIndex);
		}
	}
}
void		LoaderFbxMesh::parseIndicesAndWeights(FbxCluster* cluster, LoaderFbxMeshDesc* meshDesc, int nodeIndex)
{
	int numIndices = cluster->GetControlPointIndicesCount();
				
	int* indices = cluster->GetControlPointIndices();
	double* weights = cluster->GetControlPointWeights();
	
	for(int i =0; i<numIndices; i++)
	{
		int index = indices[i];
		double weight = weights[i];
		meshDesc->addVertexBoneIndex(indices[i], nodeIndex);
		meshDesc->addVertexBoneWeight(indices[i], static_cast<float>(weights[i]));
	}
}
void		LoaderFbxMesh::parseTransformMatrix(FbxCluster* cluster, FbxMesh* mesh, FbxPose* fbxPose, LoaderFbxMeshDesc* meshDesc, int index)
{
	FbxCluster::ELinkMode clusterMode = cluster->GetLinkMode();

	FbxAMatrix globalPosition = getGlobalPosition(mesh->GetNode(), FbxTime(0), fbxPose, &globalPosition);
	globalPosition *= getGeometry(mesh->GetNode());

	FbxAMatrix fbxMatrix;
//	if(clusterMode == FbxCluster::eAdditive && cluster->GetAssociateModel())
//		fbxMatrix = parseTransformMatrixAssociateModel(cluster, mesh, fbxPose, globalPosition);
//	else
//		fbxMatrix = parseTransformMatrixOther(cluster, mesh, fbxPose, globalPosition);

	fbxMatrix = parseTransformMatrix_debug(cluster, mesh, fbxPose, globalPosition);

//	fbxMatrix = convertToLeftHanded(fbxMatrix);

	Float4x4 offsetMatrix = translateMatrixToFloat4x4(fbxMatrix);

	meshDesc->setOffsetMatrix(index, offsetMatrix);
}
FbxAMatrix	LoaderFbxMesh::parseTransformMatrixAssociateModel(FbxCluster* cluster, FbxMesh* mesh, FbxPose* fbxPose, FbxAMatrix globalPosition)
{
	FbxAMatrix associateGeometry;
	FbxAMatrix associateGlobalInitPosition;
	FbxAMatrix associateGlobalCurrentPosition;

	FbxAMatrix referenceGeometry;
	FbxAMatrix referenceGlobalInitPosition;
	FbxAMatrix referenceGlobalCurrentPosition;

	FbxAMatrix clusterGeometry;
	FbxAMatrix clusterGlobalInitPosition;
	FbxAMatrix clusterGlobalCurrentPosition;

	FbxTime time(0);

	cluster->GetTransformAssociateModelMatrix(associateGlobalInitPosition);
	associateGeometry = getGeometry(cluster->GetAssociateModel());
	associateGlobalInitPosition *= associateGeometry;
	associateGlobalCurrentPosition = getGlobalPosition(cluster->GetAssociateModel(), time, fbxPose, nullptr);

	cluster->GetTransformMatrix(referenceGlobalInitPosition);
	referenceGeometry = getGeometry(mesh->GetNode());
	referenceGlobalInitPosition *= referenceGeometry;
	referenceGlobalCurrentPosition = globalPosition;

	cluster->GetTransformLinkMatrix(clusterGlobalInitPosition);
	clusterGeometry = getGeometry(cluster->GetLink());
	clusterGlobalInitPosition *= clusterGeometry;
	clusterGlobalCurrentPosition = getGlobalPosition(cluster->GetLink(), time, fbxPose, nullptr);

	FbxAMatrix offsetMatrix;
	offsetMatrix = referenceGlobalInitPosition.Inverse() * associateGlobalInitPosition * associateGlobalCurrentPosition.Inverse() *
			clusterGlobalCurrentPosition * clusterGlobalInitPosition.Inverse() * referenceGlobalInitPosition;

	return offsetMatrix;
}
FbxAMatrix	LoaderFbxMesh::parseTransformMatrixOther(FbxCluster* cluster, FbxMesh* mesh, FbxPose* fbxPose, FbxAMatrix globalPosition)
{
	FbxAMatrix referenceGeometry;
	FbxAMatrix referenceGlobalInitPosition;
	FbxAMatrix referenceGlobalCurrentPosition;

	FbxAMatrix clusterGeometry;
	FbxAMatrix clusterGlobalInitPosition;
	FbxAMatrix clusterGlobalCurrentPosition;

	FbxAMatrix clusterRelativeInitPosition;
	FbxAMatrix clusterRelativeCurrentPositionInverse;

	FbxTime time(0);

	FbxAMatrix fbxMatrixIdentity;
	fbxMatrixIdentity.SetIdentity();

	cluster->GetTransformMatrix(referenceGlobalInitPosition);
	referenceGlobalCurrentPosition = globalPosition;
	referenceGeometry = getGeometry(mesh->GetNode());
	referenceGlobalInitPosition *= referenceGeometry;

	cluster->GetTransformLinkMatrix(clusterGlobalInitPosition);
	clusterGlobalCurrentPosition = getGlobalPosition(cluster->GetLink(), time, fbxPose, nullptr);

	clusterRelativeInitPosition = clusterGlobalInitPosition.Inverse() * referenceGlobalInitPosition;
	clusterRelativeCurrentPositionInverse = referenceGlobalCurrentPosition.Inverse() * clusterGlobalCurrentPosition;
	
	FbxAMatrix offsetMatrix;
	offsetMatrix = clusterRelativeCurrentPositionInverse * clusterRelativeInitPosition;
	
	return offsetMatrix;
}

FbxAMatrix  LoaderFbxMesh::parseTransformMatrix_debug(FbxCluster* cluster, FbxMesh* mesh, FbxPose* fbxPose, FbxAMatrix globalPosition)
{
	FbxAMatrix transform;
	FbxAMatrix bindPoseTransform;
	FbxAMatrix bindPoseTransformInverse;
	FbxAMatrix boneReferenceTransform;
	FbxAMatrix boneReferenceTransformInverse;
	FbxAMatrix meshGeometryMatrix;

	cluster->GetTransformLinkMatrix(bindPoseTransform);
	cluster->GetTransformMatrix(boneReferenceTransform);

	bindPoseTransformInverse		= bindPoseTransform.Inverse();
	boneReferenceTransformInverse	= boneReferenceTransform.Inverse(); 

	meshGeometryMatrix = getGeometry(mesh->GetNode());

	transform = bindPoseTransformInverse * boneReferenceTransform;

	return transform;
}

FbxAMatrix	LoaderFbxMesh::getGeometry(FbxNode* node)
{
	FbxAMatrix fbxGeometry;
	FbxVector4 fbxTranslation, fbxRotation, fbxScaling;

	fbxTranslation	= node->GetGeometricTranslation(FbxNode::eSourcePivot);
	fbxRotation		= node->GetGeometricRotation(FbxNode::eSourcePivot);
	fbxScaling		= node->GetGeometricScaling(FbxNode::eSourcePivot);
	fbxGeometry.SetTRS(fbxTranslation, fbxRotation, fbxScaling);

	return fbxGeometry;
}
FbxAMatrix	LoaderFbxMesh::getGlobalPosition(FbxNode* node, FbxTime time, FbxPose* pose, FbxAMatrix* parentGlobalPosition)
{
	FbxAMatrix globalPosition;

	bool positionFound = false;

	if(pose)
	{
		int nodeIndex = pose->Find(node);
		if(nodeIndex > -1)
		{
			if(pose->IsBindPose() || !pose->IsLocalMatrix(nodeIndex))
				globalPosition = getPoseMatrix(pose, nodeIndex);
			else
			{
				FbxAMatrix localParentGlobalPosition;
				if(parentGlobalPosition)
					localParentGlobalPosition = *parentGlobalPosition;
				else
				{
					if(node->GetParent())
						localParentGlobalPosition = getGlobalPosition(node->GetParent(), time, pose, nullptr);
				}

				FbxAMatrix localPosition = getPoseMatrix(pose, nodeIndex);
				globalPosition = localParentGlobalPosition * localPosition;
			}
			positionFound = true;
		}
	}

	if(!positionFound)
		globalPosition = node->EvaluateGlobalTransform(time);

	return globalPosition;
}
FbxAMatrix	LoaderFbxMesh::getPoseMatrix(FbxPose* pose, int nodeIndex)
{
	FbxAMatrix poseMatrix;
	FbxMatrix matrix = pose->GetMatrix(nodeIndex);
	
	memcpy(static_cast<double*>(poseMatrix), static_cast<double*>(matrix), sizeof(matrix.mData));

	return poseMatrix;
}

FbxAMatrix LoaderFbxMesh::convertToLeftHanded(FbxAMatrix fbxMatrix)
{
	FbxAMatrix convertionMatrix;
	FbxVector4 rowX(1.0, 0.0, 0.0, 0.0);
	FbxVector4 rowY(0.0, 1.0, 0.0, 0.0);
	FbxVector4 rowZ(0.0, 0.0, -1.0, 0.0);
	FbxVector4 rowW(0.0, 0.0, 0.0, 1.0);

	convertionMatrix.SetRow(0, rowX);
	convertionMatrix.SetRow(1, rowY);
	convertionMatrix.SetRow(2, rowZ);
	convertionMatrix.SetRow(3, rowW);

	FbxAMatrix convertedMatrix = fbxMatrix * convertionMatrix;
	return convertedMatrix;
}
Float4x4	LoaderFbxMesh::translateMatrixToFloat4x4(FbxAMatrix fbxMatrix)
{
	Float4x4 matrix;
	for(int x=0; x<4; x++)
	{
		for(int y=0; y<4; y++)
			matrix.m[x][y] = static_cast<float>(fbxMatrix.mData[x][y]);
	}
	return matrix;
}

void LoaderFbxMesh::transformVertices(FbxMesh* mesh)
{
	FbxNode*	node		= mesh->GetNode();
	FbxAMatrix	fbxMatrix	= node->EvaluateGlobalTransform(FbxTime(0));

	DirectX::XMMATRIX xmTransform;
	xmTransform = DirectX::XMMATRIX(static_cast<float>(fbxMatrix.mData[0][0]), static_cast<float>(fbxMatrix.mData[0][1]), static_cast<float>(fbxMatrix.mData[0][2]), static_cast<float>(fbxMatrix.mData[0][3]),
									static_cast<float>(fbxMatrix.mData[1][0]), static_cast<float>(fbxMatrix.mData[1][1]), static_cast<float>(fbxMatrix.mData[1][2]), static_cast<float>(fbxMatrix.mData[1][3]),
									static_cast<float>(fbxMatrix.mData[2][0]), static_cast<float>(fbxMatrix.mData[2][1]), static_cast<float>(fbxMatrix.mData[2][2]), static_cast<float>(fbxMatrix.mData[2][3]),
									static_cast<float>(fbxMatrix.mData[3][0]), static_cast<float>(fbxMatrix.mData[3][1]), static_cast<float>(fbxMatrix.mData[3][2]), static_cast<float>(fbxMatrix.mData[3][3]));

	xmTransform = DirectX::XMMatrixTranspose(xmTransform);

	DirectX::XMFLOAT3 position;
	DirectX::XMVECTOR xmPosition;
	for(unsigned int i=0; i<vertexPositions_.size(); i++)
	{
		position.x = vertexPositions_[i].x;
		position.y = vertexPositions_[i].y;
		position.z = vertexPositions_[i].z;
	
		xmPosition = DirectX::XMLoadFloat3(&position);
		xmPosition = DirectX::XMVector3TransformCoord(xmPosition, xmTransform);
		
		DirectX::XMStoreFloat3(&position, xmPosition);
		vertexPositions_[i].x = position.x;
		vertexPositions_[i].y = position.y;
		vertexPositions_[i].z = position.z;
	}

	DirectX::XMFLOAT3 normal;
	DirectX::XMVECTOR xmNormal;
	for(unsigned int i=0; i<vertexNormals_.size(); i++)
	{
		normal.x = vertexNormals_[i].x;
		normal.y = vertexNormals_[i].y;
		normal.z = vertexNormals_[i].z;

		xmNormal = DirectX::XMLoadFloat3(&normal);
		xmNormal = DirectX::XMVector3TransformNormal(xmNormal, xmTransform);

		DirectX::XMStoreFloat3(&normal, xmNormal);
		vertexNormals_[i].x = normal.x;
		vertexNormals_[i].y = normal.y;
		vertexNormals_[i].z = normal.z;
	}

	DirectX::XMFLOAT4 tangent;
	DirectX::XMVECTOR xmTangent;
	for(unsigned int i=0; i<vertexTangents_.size(); i++)
	{
		tangent.x = vertexTangents_[i].x;
		tangent.y = vertexTangents_[i].y;
		tangent.z = vertexTangents_[i].z;
		tangent.w = vertexTangents_[i].w;

		xmTangent = DirectX::XMLoadFloat4(&tangent);
		xmTangent = DirectX::XMVector4Transform(xmTangent, xmTransform);

		DirectX::XMStoreFloat4(&tangent, xmTangent);
		vertexTangents_[i].x = tangent.x;
		vertexTangents_[i].y = tangent.y;
		vertexTangents_[i].z = tangent.z;
		vertexTangents_[i].w = tangent.w;
	}
}
