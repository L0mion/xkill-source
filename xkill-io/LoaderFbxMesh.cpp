#include <sstream>

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
	int errorCode = mesh->GetLastErrorID();
	if(errorCode != -1)
	{
		std::stringstream message;
		message << "LoaderFbx::parseMesh | Error code : " << errorCode
				<< " Error message: " << mesh->GetLastErrorString();
		SHOW_MESSAGEBOX(message.str());
	}

	int polygonVertexCount = mesh->GetPolygonVertexCount();
	int polygonCount = polygonVertexCount / POLYGON_SIZE;
	
	parseVertexPositions(mesh, polygonVertexCount);
	
	int vertexId = 0;

	for(int polygonIndex=0; polygonIndex<polygonCount; polygonIndex++)
	{
		for(int insidePolygonIndex=0; insidePolygonIndex<POLYGON_SIZE; insidePolygonIndex++)
		{
			parseVertexNormals(mesh, polygonIndex, insidePolygonIndex, vertexId);
			parseVertexUVs(mesh, polygonIndex, insidePolygonIndex);
			parseVertexTangents(mesh, vertexId);
			parseVertexBinormals(mesh, vertexId);
			vertexId++;
		}
	}

	createVerticesPosNormTex();

	//parseMaterial(mesh);
}
void LoaderFbxMesh::parseVertexPositions(FbxMesh* mesh, int polygonVertexCount)
{
	FbxVector4* controlPoints = mesh->GetControlPoints();
	int* indices = mesh->GetPolygonVertices();

	for(int i=0; i<polygonVertexCount; i++)
	{
		Float3 position;
		position.x = static_cast<float>(controlPoints[indices[i]].mData[0]);
		position.y = static_cast<float>(controlPoints[indices[i]].mData[1]);
		position.z = static_cast<float>(controlPoints[indices[i]].mData[2]);

		vertexPositions_.push_back(position);
	}
}
void LoaderFbxMesh::parseVertexNormals(FbxMesh* mesh, int polygonIndex, int insidePolygonIndex, int vertexId)
{
	FbxVector4 fbxNormal;
	mesh->GetPolygonVertexNormal(polygonIndex, insidePolygonIndex, fbxNormal);
	Float3 normal;
	normal.x = static_cast<float>(fbxNormal.mData[0]);
	normal.y = static_cast<float>(fbxNormal.mData[1]);
	normal.z = static_cast<float>(fbxNormal.mData[2]);

	vertexNormals_.push_back(normal);
}
void LoaderFbxMesh::parseVertexUVs(FbxMesh* mesh, int polygonIndex, int insidePolygonIndex)
{
	FbxStringList uvSetNameList;
	mesh->GetUVSetNames(uvSetNameList);

	for(int i=0; i<uvSetNameList.GetCount(); i++)
	{
		const char* uvSetName = uvSetNameList.GetStringAt(i);

		FbxVector2 fbxUV; 
		mesh->GetPolygonVertexUV(polygonIndex, insidePolygonIndex, uvSetName, fbxUV);
		Float2 uv;
		uv.x = static_cast<float>(fbxUV.mData[0]);
		uv.y = static_cast<float>(fbxUV.mData[1]);
		vertexUVs_.push_back(uv);
	}
}
void LoaderFbxMesh::parseVertexTangents(FbxMesh* mesh, int vertexId)
{
	for(int i=0; i<mesh->GetElementTangentCount(); i++)
	{
		FbxVector4 fbxTangent;

		FbxGeometryElementTangent* tangentElement = mesh->GetElementTangent(i);

		if( tangentElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			int id = 0;
			switch(tangentElement->GetReferenceMode())
			{
			case FbxGeometryElement::eDirect:
				fbxTangent = tangentElement->GetDirectArray().GetAt(vertexId);
				break;
			case FbxGeometryElement::eIndexToDirect:
				id 			= tangentElement->GetIndexArray().GetAt(vertexId);
				fbxTangent	= tangentElement->GetDirectArray().GetAt(id);
				break;
			default:
				break;
			}
		}

		Float4 tangent;
		tangent.x = static_cast<float>(fbxTangent.mData[0]);
		tangent.y =	static_cast<float>(fbxTangent.mData[1]);
		tangent.z =	static_cast<float>(fbxTangent.mData[2]);
		tangent.w =	static_cast<float>(fbxTangent.mData[3]);
		vertexTangents_.push_back(tangent);
	}
}
void LoaderFbxMesh::parseVertexBinormals(FbxMesh* mesh, int vertexId)
{
	for(int i=0; i<mesh->GetElementBinormalCount(); i++)
	{
		FbxVector4 fbxBinormal;
		FbxGeometryElementBinormal* binormalElement = mesh->GetElementBinormal(i);
		if(binormalElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
		{
			int id=0;
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
		}

		Float4 binormal;
		binormal.x = static_cast<float>(fbxBinormal.mData[0]);
		binormal.y = static_cast<float>(fbxBinormal.mData[1]);
		binormal.z = static_cast<float>(fbxBinormal.mData[2]);
		binormal.w = static_cast<float>(fbxBinormal.mData[3]);
		vertexBinormals_.push_back(binormal);
	}
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