#include <sstream>

#include <fbxsdk.h>

#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/MeshVertices.h>

#include "LoaderFbxMesh.h"

LoaderFbxMesh::LoaderFbxMesh()
{
}
LoaderFbxMesh::~LoaderFbxMesh()
{
}
void LoaderFbxMesh::reset()
{
	indices_.clear();

	verticesPosNormTex_.clear();

	vertexPositions_.clear();
	vertexNormals_.clear();
	vertexUVs_.clear();
	vertexTangents_.clear();
	vertexBinormals_.clear();
}

void LoaderFbxMesh::parseMesh(FbxMesh* mesh)
{
//	int errorCode = mesh->GetLastErrorID();
//	if(errorCode != -1)
//	{
//		std::stringstream message;
//		message << "LoaderFbx::parseMesh | Error code : " << errorCode
//				<< " Error message: " << mesh->GetLastErrorString();
//		SHOW_MESSAGEBOX(message.str());
//	}

	int polygonVertexCount = mesh->GetPolygonVertexCount();
	int polygonCount = mesh->GetPolygonCount();
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

//	createVerticesPosNormTex();
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

void LoaderFbxMesh::parseVertexColors(FbxMesh* mesh, int controlPointIndex, int vertexId)
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

void LoaderFbxMesh::parseVertexNormals(FbxMesh* mesh, int controlPointIndex, int vertexId)
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
FbxVector4 LoaderFbxMesh::parseVertexNormalsByControlPoint(FbxGeometryElementNormal* normalElement, int controlPointIndex)
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
FbxVector4 LoaderFbxMesh::parseVertexNormalsByPolygonVertex(FbxGeometryElementNormal* normalElement, int vertexId)
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

void LoaderFbxMesh::parseVertexUVs(FbxMesh* mesh, int polygonIndex, int insidePolygonIndex, int controlPointIndex)
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
FbxVector2 LoaderFbxMesh::parseVertexUVsByControlPoint(FbxGeometryElementUV* uvElement, int controlPointIndex)
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
FbxVector2 LoaderFbxMesh::parseVertexUVsByPolygonVertex(FbxMesh* mesh, FbxGeometryElementUV* uvElement, int polygonIndex, int insidePolygonIndex)
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

void LoaderFbxMesh::parseVertexTangents(FbxMesh* mesh, int controlPointIndex, int vertexId)
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
FbxVector4 LoaderFbxMesh::parseVertexTangentsByControlPoint(FbxGeometryElementTangent* tangentElement, int controlPointIndex)
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
FbxVector4 LoaderFbxMesh::parseVertexTangentsByPolygonVertex(FbxGeometryElementTangent* tangentElement, int vertexId)
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

void LoaderFbxMesh::parseVertexBinormals(FbxMesh* mesh, int controlPointIndex, int vertexId)
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
FbxVector4 LoaderFbxMesh::parseVertexBinormalsByControlPoint(FbxGeometryElementBinormal* binormalElement, int controlPointIndex)
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
FbxVector4 LoaderFbxMesh::parseVertexBinormalsByPolygonVertex(FbxGeometryElementBinormal* binormalElement, int vertexId)
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

bool LoaderFbxMesh::float2Equal(Float2 f1, Float2 f2)
{
	bool equal = false;
	if(f1.x == f2.x && f1.y == f2.y)
		equal = true;

	return equal;
}
bool LoaderFbxMesh::float3Equal(Float3 f1, Float3 f2)
{
	bool equal = false;
	if(f1.x == f2.x && f1.y == f2.y && f1.z == f2.z)
		equal = true;

	return equal;
}
bool LoaderFbxMesh::float4Equal(Float4 f1, Float4 f2)
{
	bool equal = false;
	if(f1.x == f2.x && f1.y == f2.y && f1.z == f2.z && f1.w == f2.w)
		equal = true;

	return equal;
}

std::vector<VertexPosNormTex> LoaderFbxMesh::assemblePosNormTex()
{
	std::vector<VertexPosNormTex> vertices;
	for(unsigned int i=0; i<vertexPositions_.size(); i++)
	{
		VertexPosNormTex vertex;
		vertex.position_	= vertexPositions_[i];
		vertex.normal_		= vertexNormals_[i];
		vertex.texcoord_	= vertexUVs_[i];

		vertices.push_back(vertex);
	}
	return vertices;
}

void LoaderFbxMesh::indexPosNormTex(std::vector<VertexPosNormTex> posNormTex)
{
	indices_.clear();
	verticesPosNormTex_.clear();

	for(unsigned int i=0; i<posNormTex.size(); i++)
	{
		bool equal			= false;
		unsigned int index	= 0;
		while(index<verticesPosNormTex_.size() && !equal)
		{
			equal = float3Equal(posNormTex[i].position_, verticesPosNormTex_[index].position_);
			if(equal)
				equal = float3Equal(posNormTex[i].normal_, verticesPosNormTex_[index].normal_);
			if(equal)
				equal = float2Equal(posNormTex[i].texcoord_, verticesPosNormTex_[index].texcoord_);
			if(!equal)
				index++;
		}
		if(equal)
			indices_.push_back(index);
		else
		{
			verticesPosNormTex_.push_back(posNormTex[i]);
			indices_.push_back(verticesPosNormTex_.size()-1);
		}
	}
}

void LoaderFbxMesh::createVerticesPosNormTex()
{
	std::vector<VertexPosNormTex> vertices;
	vertices = assemblePosNormTex();
	indexPosNormTex(vertices);
}

std::vector<unsigned int> LoaderFbxMesh::getIndices()
{
	return indices_;
}
std::vector<VertexPosNormTex> LoaderFbxMesh::getVerticesPosNormTex()
{
	return verticesPosNormTex_;
}