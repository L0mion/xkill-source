#include <iostream>

#include "LoaderMD5.h"

LoaderMD5::LoaderMD5()
{
	fileLength_ = 0;

	md5Version_ = -1;
	numJoints_ = 0;
	numMeshes_ = 0;
	hasAnimation_ = false;

	DirectX::XMStoreFloat4x4(&localToWorldMatrix_, DirectX::XMMatrixIdentity());
}
LoaderMD5::~LoaderMD5()
{
	if(infile_.is_open())
		infile_.close();
}

bool LoaderMD5::loadModel(const std::string& filename, LoaderMD5ModelDesc* modelDesc)
{
	reset();

	fileLength_ = getFileLength(filename);

	std::string param;

	infile_.open(filename.c_str());

	infile_ >> param;
	while(!infile_.eof())
	{
		if(param == "MD5Version") 
			parseParamMD5Version();
		else if(param == "commandline")
			parseParamCommandline();
		else if(param == "numJoints")
			parseParamNumJoints();
		else if(param == "numMeshes")
			parseParamNumMeshes();
		else if(param == "joints")
			parseParamJoints();
		else if(param == "mesh")
			parseParamMesh();


		infile_ >> param;
	}

	modelDesc->joints_ = joints_;
	modelDesc->meshes_ = meshes_;
	
	return true;
}

void LoaderMD5::parseParamMD5Version()
{
	infile_ >> md5Version_;
}
void LoaderMD5::parseParamCommandline()
{
	//Comandline is uninteresting information and will therefor be ignored.
	ignoreLine(infile_, fileLength_);
}
void LoaderMD5::parseParamNumJoints()
{
	infile_ >> numJoints_;
	joints_.reserve(numJoints_);
}
void LoaderMD5::parseParamNumMeshes()
{
	infile_ >> numMeshes_;
	meshes_.reserve(numMeshes_);
}
void LoaderMD5::parseParamJoints()
{
	LoaderMD5JointDesc joint;
	std::string dummy;
	
	//Read the '{' character.
	infile_ >> dummy;

	for(int i=0; i<numJoints_; i++)
	{
		infile_ >> joint.name_ >> joint.parentID_ >> dummy
				>> joint.position_.x >> joint.position_.y >> joint.position_.z >> dummy >> dummy
				>> joint.orientationQuaternion_.x >> joint.orientationQuaternion_.y >> joint.orientationQuaternion_.z >> dummy;
		
		removeQuotes(joint.name_);
		computeQuaternionW(joint.orientationQuaternion_);
		joints_.push_back(joint);

		ignoreLine(infile_, fileLength_);
	}

	//Read the '}' character.
	infile_ >> dummy;
}
void LoaderMD5::parseParamMesh()
{
	LoaderMD5MeshDesc mesh;

	std::string param;
	std::string dummy;
	//Read the '{' character.
	infile_ >> dummy;
	infile_ >> param;

	while(param != "}")
	{
		if(param == "shader")
			parseParamShader(mesh);
		else if(param == "numverts")
			parseParamNumVerts(mesh);
		else if(param == "numtris")
			parseParamNumTris(mesh);
		else if(param == "numweights")
			parseParamNumWeights(mesh);
		else
			ignoreLine(infile_, fileLength_);

		infile_ >> param;
	}

	prepareMesh(mesh);
	prepareNormals(mesh);

	meshes_.push_back(mesh);
	
}
void LoaderMD5::parseParamShader(LoaderMD5MeshDesc& mesh)
{
	infile_ >> mesh.shader_;
	removeQuotes(mesh.shader_);
	ignoreLine(infile_, fileLength_);
}
void LoaderMD5::parseParamNumVerts(LoaderMD5MeshDesc& mesh)
{
	std::string dummy;
	int numVertices = 0;
	infile_ >> numVertices;
	ignoreLine(infile_, fileLength_);

	for(int i=0; i<numVertices; i++)
	{
		LoaderMD5VertexDesc vertex;

		infile_ >> dummy >> dummy >> dummy	//vert vertexIndex (
				>> vertex.texcoord_.x >> vertex.texcoord_.y >> dummy // u v )
				>> vertex.startWeight_ >> vertex.numWeights_; 

		ignoreLine(infile_, fileLength_);
		mesh.vertices_.push_back(vertex);
		mesh.texcoords_.push_back(vertex.texcoord_);
	}
}
void LoaderMD5::parseParamNumTris(LoaderMD5MeshDesc& mesh)
{
	std::string dummy;
	int numTriangles;
	infile_ >> numTriangles;

	ignoreLine(infile_, fileLength_);
	for(int i=0; i<numTriangles; i++)
	{
		LoaderMD5TriangleDesc triangle;
		infile_ >> dummy >> dummy >> triangle.indices_[0] >> triangle.indices_[1] >> triangle.indices_[2];

		ignoreLine(infile_, fileLength_);

		mesh.triangles_.push_back(triangle);
		mesh.indices_.push_back(static_cast<unsigned int>(triangle.indices_[2]));	//0
		mesh.indices_.push_back(static_cast<unsigned int>(triangle.indices_[1]));	//1
		mesh.indices_.push_back(static_cast<unsigned int>(triangle.indices_[0]));	//2
	}
}
void LoaderMD5::parseParamNumWeights(LoaderMD5MeshDesc& mesh)
{
	std::string dummy;
	int numWeights;
	infile_ >> numWeights;
	ignoreLine(infile_, fileLength_);
	for(int i=0; i<numWeights; i++)
	{
		LoaderMD5WeightDesc weight;
		infile_ >> dummy >> dummy >> weight.jointID_ >> weight.bias_ >> dummy					//weight weightIndex ..... (
				>> weight.position_.x >> weight.position_.y >> weight.position_.z >> dummy;		// )
		ignoreLine(infile_, fileLength_);
		mesh.weights_.push_back(weight);
	}
}


void LoaderMD5::prepareMesh(LoaderMD5MeshDesc& mesh)
{
	mesh.positions_.clear();
	mesh.texcoords_.clear();

	for(unsigned int vertexIndex=0; vertexIndex<mesh.vertices_.size(); vertexIndex++)
	{
		DirectX::XMFLOAT3 finalPosition(0.0f, 0.0f, 0.0f);
		LoaderMD5VertexDesc& vertex = mesh.vertices_[vertexIndex];

		vertex.position_ = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		vertex.normal_ = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);

		for(int weightIndex=0; weightIndex<vertex.numWeights_; weightIndex++)
		{
			LoaderMD5WeightDesc& weight = mesh.weights_[vertex.startWeight_ + weightIndex];
			LoaderMD5JointDesc& joint = joints_[weight.jointID_];

			//joint.orientationQuaternion_ * weight.position_;
			DirectX::XMFLOAT3 rotationPosition = rotateVector(weight.position_, joint.orientationQuaternion_);

			DirectX::XMFLOAT3 position;
			position.x = (joint.position_.x + rotationPosition.x) * weight.bias_;
			position.y = (joint.position_.y + rotationPosition.y) * weight.bias_;
			position.z = (joint.position_.z + rotationPosition.z) * weight.bias_;

			vertex.position_.x += position.x;
			vertex.position_.y += position.y;
			vertex.position_.z += position.z;
		}

		mesh.positions_.push_back(vertex.position_);
		mesh.texcoords_.push_back(vertex.texcoord_);
	}
	
}
void LoaderMD5::prepareNormals(LoaderMD5MeshDesc& mesh)
{
	mesh.normals_.clear();

	for(unsigned int triangleIndex=0; triangleIndex< mesh.triangles_.size(); triangleIndex++)
	{
		DirectX::XMFLOAT3 v0 = mesh.vertices_[mesh.triangles_[triangleIndex].indices_[0]].position_;
		DirectX::XMFLOAT3 v1 = mesh.vertices_[mesh.triangles_[triangleIndex].indices_[1]].position_;
		DirectX::XMFLOAT3 v2 = mesh.vertices_[mesh.triangles_[triangleIndex].indices_[2]].position_;

		DirectX::XMVECTOR xmV0 = DirectX::XMLoadFloat3(&v0);
		DirectX::XMVECTOR xmV1 = DirectX::XMLoadFloat3(&v1);
		DirectX::XMVECTOR xmV2 = DirectX::XMLoadFloat3(&v2);

		DirectX::XMVECTOR xmNormal = DirectX::XMVector3Cross(DirectX::XMVectorSubtract(xmV2, xmV0), DirectX::XMVectorSubtract(xmV1, xmV0)); 

		for(int i=0; i<3; i++)
		{
			DirectX::XMFLOAT3 normal = mesh.vertices_[mesh.triangles_[triangleIndex].indices_[i]].normal_;
			DirectX::XMVECTOR xmResult = DirectX::XMLoadFloat3(&normal);
			xmResult = DirectX::XMVectorAdd(xmResult, xmNormal);
			DirectX::XMStoreFloat3(&normal, xmResult);
			mesh.vertices_[mesh.triangles_[triangleIndex].indices_[i]].normal_ = normal;
		}
	}

	for(unsigned int vertexIndex=0; vertexIndex<mesh.vertices_.size(); vertexIndex++)
	{
		LoaderMD5VertexDesc& vertex = mesh.vertices_[vertexIndex];
		DirectX::XMFLOAT3 normal = vertex.normal_;
		
		normal = normalizeVector(normal);
		
		mesh.normals_.push_back(normal);

		vertex.normal_ = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		for(int weightIndex=0; weightIndex<vertex.numWeights_; weightIndex++)
		{
			const LoaderMD5WeightDesc& weight = mesh.weights_[vertex.startWeight_ + weightIndex];
			const LoaderMD5JointDesc& joint = joints_[weight.jointID_];
			normal = rotateVector(normal, joint.orientationQuaternion_);

			vertex.normal_.x += normal.x * weight.bias_;
			vertex.normal_.y += normal.y * weight.bias_;
			vertex.normal_.z += normal.z * weight.bias_;
		}
	}
}

void LoaderMD5::reset()
{
	//Make sure no data from previous files is present.
	if(infile_.is_open())
		infile_.close();
	
	fileLength_ = 0;
	
	joints_.clear();
	meshes_.clear();
}

int LoaderMD5::getFileLength(const std::string& filename)
{
	long begin, end;
	std::ifstream file(filename);
	begin = file.tellg();
	file.seekg(0, std::ios::end);
	end = file.tellg();
	file.close();

	int fileLength = end-begin;
	return fileLength;
}
void LoaderMD5::ignoreLine(std::ifstream& file, int length)
{
	file.ignore(length, '\n');
}
void LoaderMD5::removeQuotes(std::string& str)
{
	size_t n;
	while( (n = str.find('\"')) != std::string::npos)
		str.erase(n, 1);
}
void LoaderMD5::computeQuaternionW(DirectX::XMFLOAT4& quaternion)
{
	float t = 1.0f - ( quaternion.x * quaternion.x ) - ( quaternion.y * quaternion.y ) - ( quaternion.z * quaternion.z );
    if ( t < 0.0f )
    {
        quaternion.w = 0.0f;
    }
    else
    {
        quaternion.w = -sqrtf(t);
    }
}

DirectX::XMFLOAT3 LoaderMD5::normalizeVector(DirectX::XMFLOAT3 vec)
{
	DirectX::XMVECTOR xmVec = DirectX::XMLoadFloat3(&vec);
	xmVec = DirectX::XMVector3Normalize(xmVec);
	
	DirectX::XMFLOAT3 result;
	DirectX::XMStoreFloat3(&result, xmVec);

	return result;
}
DirectX::XMFLOAT3 LoaderMD5::rotateVector(DirectX::XMFLOAT3 vec, DirectX::XMFLOAT4 quaternion)
{
	DirectX::XMVECTOR xmQuaternion = DirectX::XMLoadFloat4(&quaternion);
	DirectX::XMVECTOR xmVec = DirectX::XMLoadFloat3(&vec);
	xmVec = DirectX::XMVector3Rotate(xmVec, xmQuaternion);
	
	DirectX::XMFLOAT3 result;
	DirectX::XMStoreFloat3(&result, xmVec);

	return result;
}