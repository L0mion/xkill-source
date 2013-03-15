#include "LoaderMD5Model.h"
#include "LoaderMD5Helper.h"



LoaderMD5Model::LoaderMD5Model()
{
	fileLength_ = 0;

	md5Version_ = -1;
	numJoints_ = 0;
	numMeshes_ = 0;
	hasAnimation_ = false;
}
LoaderMD5Model::~LoaderMD5Model()
{
	if(infile_.is_open())
		infile_.close();
}

bool LoaderMD5Model::loadModel(const std::string& filename, LoaderMD5ModelDesc* modelDesc)
{
	reset();

	fileLength_ = MD5Helper::getFileLength(filename);

	std::string param;

	infile_.open(filename.c_str());

	if(!infile_.is_open())
		return false;

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

	infile_.close();

	modelDesc->joints_		= joints_;
	modelDesc->meshes_		= meshes_;
	modelDesc->boneOffsets_ = bindPoseInverse_;
	
	return true;
}

void LoaderMD5Model::parseParamMD5Version()
{
	infile_ >> md5Version_;
}
void LoaderMD5Model::parseParamCommandline()
{
	//Comandline is uninteresting information and will therefor be ignored.
	MD5Helper::ignoreLine(infile_, fileLength_);
}
void LoaderMD5Model::parseParamNumJoints()
{
	infile_ >> numJoints_;
	joints_.reserve(numJoints_);
}
void LoaderMD5Model::parseParamNumMeshes()
{
	infile_ >> numMeshes_;
	meshes_.reserve(numMeshes_);
}
void LoaderMD5Model::parseParamJoints()
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
		
		MD5Helper::removeQuotes(joint.name_);
		MD5Helper::computeQuaternionW(joint.orientationQuaternion_);
		joints_.push_back(joint);

		MD5Helper::ignoreLine(infile_, fileLength_);
	}

	//Read the '}' character.
	infile_ >> dummy;

	buildBindPose(joints_);
}
void LoaderMD5Model::parseParamMesh()
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
			MD5Helper::ignoreLine(infile_, fileLength_);

		infile_ >> param;
	}

	prepareMesh(mesh);
	prepareNormals(mesh);

	meshes_.push_back(mesh);
	
}
void LoaderMD5Model::parseParamShader(LoaderMD5MeshDesc& mesh)
{
	infile_ >> mesh.shader_;
	MD5Helper::removeQuotes(mesh.shader_);
	MD5Helper::ignoreLine(infile_, fileLength_);
}
void LoaderMD5Model::parseParamNumVerts(LoaderMD5MeshDesc& mesh)
{
	std::string dummy;
	int numVertices = 0;
	infile_ >> numVertices;
	MD5Helper::ignoreLine(infile_, fileLength_);

	for(int i=0; i<numVertices; i++)
	{
		LoaderMD5VertexDesc vertex;

		infile_ >> dummy >> dummy >> dummy	//vert vertexIndex (
				>> vertex.texcoord_.x >> vertex.texcoord_.y >> dummy // u v )
				>> vertex.startWeight_ >> vertex.numWeights_; 

		//Texture coordinates gets fliped in the shader because of obj and fbx texcoord needs that.
		//Md5 however is correct and must be fliped before the shader to be correct in the shader.
		vertex.texcoord_.y = 1 - vertex.texcoord_.y;

		MD5Helper::ignoreLine(infile_, fileLength_);
		mesh.vertices_.push_back(vertex);
		mesh.texcoords_.push_back(vertex.texcoord_);
	}
}
void LoaderMD5Model::parseParamNumTris(LoaderMD5MeshDesc& mesh)
{
	std::string dummy;
	int numTriangles;
	infile_ >> numTriangles;

	MD5Helper::ignoreLine(infile_, fileLength_);
	for(int i=0; i<numTriangles; i++)
	{
		LoaderMD5TriangleDesc triangle;
		infile_ >> dummy >> dummy >> triangle.indices_[0] >> triangle.indices_[1] >> triangle.indices_[2];

		MD5Helper::ignoreLine(infile_, fileLength_);

		mesh.triangles_.push_back(triangle);
		mesh.indices_.push_back(static_cast<unsigned int>(triangle.indices_[2]));	//0
		mesh.indices_.push_back(static_cast<unsigned int>(triangle.indices_[1]));	//1
		mesh.indices_.push_back(static_cast<unsigned int>(triangle.indices_[0]));	//2
	}
}
void LoaderMD5Model::parseParamNumWeights(LoaderMD5MeshDesc& mesh)
{
	std::string dummy;
	int numWeights;
	infile_ >> numWeights;
	MD5Helper::ignoreLine(infile_, fileLength_);
	for(int i=0; i<numWeights; i++)
	{
		LoaderMD5WeightDesc weight;
		infile_ >> dummy >> dummy >> weight.jointID_ >> weight.bias_ >> dummy					//weight weightIndex ..... (
				>> weight.position_.x >> weight.position_.y >> weight.position_.z >> dummy;		// )
		MD5Helper::ignoreLine(infile_, fileLength_);
		mesh.weights_.push_back(weight);
	}
}

void LoaderMD5Model::prepareMesh(LoaderMD5MeshDesc& mesh)
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
			DirectX::XMFLOAT3 rotationPosition = MD5Helper::rotateVector(weight.position_, joint.orientationQuaternion_);

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
void LoaderMD5Model::prepareNormals(LoaderMD5MeshDesc& mesh)
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
		
		normal = MD5Helper::normalizeVector(normal);
		
		//normal.x *= -1.0f;
		//normal.y *= -1.0f;
		//normal.z *= -1.0f;

		mesh.normals_.push_back(normal);

		vertex.normal_ = normal;

	//	vertex.normal_ = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	//	for(int weightIndex=0; weightIndex<vertex.numWeights_; weightIndex++)
	//	{
	//		const LoaderMD5WeightDesc& weight = mesh.weights_[vertex.startWeight_ + weightIndex];
	//		const LoaderMD5JointDesc& joint = joints_[weight.jointID_];
	//		normal = MD5Helper::rotateVector(normal, joint.orientationQuaternion_);
	//
	//		vertex.normal_.x += normal.x * weight.bias_;
	//		vertex.normal_.y += normal.y * weight.bias_;
	//		vertex.normal_.z += normal.z * weight.bias_;
	//	}
	}
}

void LoaderMD5Model::buildBindPose(std::vector<LoaderMD5JointDesc> joints)
{
	bindPose_.clear();
	bindPoseInverse_.clear();

	std::vector<LoaderMD5JointDesc>::const_iterator iter = joints.begin();
	while(iter != joints.end())
	{
		const LoaderMD5JointDesc& joint = (*iter);
		DirectX::XMVECTOR xmRotation = DirectX::XMLoadFloat4(&joint.orientationQuaternion_);

		DirectX::XMMATRIX xmTranslationMatrix	 = DirectX::XMMatrixTranslation(joint.position_.x, joint.position_.y, joint.position_.z);
		DirectX::XMMATRIX xmRotationMatrix		 = DirectX::XMMatrixRotationQuaternion(xmRotation);
		DirectX::XMMATRIX xmBoneTransform		 = xmRotationMatrix * xmTranslationMatrix;
		DirectX::XMVECTOR xmBoneDeterminant		 = DirectX::XMMatrixDeterminant(xmBoneTransform);
		DirectX::XMMATRIX xmBoneTransformInverse = DirectX::XMMatrixInverse(&xmBoneDeterminant, xmBoneTransform);

		DirectX::XMFLOAT4X4 boneTransform, boneTransformInverse;
		DirectX::XMStoreFloat4x4(&boneTransform, xmBoneTransform);
		DirectX::XMStoreFloat4x4(&boneTransformInverse, xmBoneTransformInverse);

		bindPose_.push_back(boneTransform);
		bindPoseInverse_.push_back(boneTransformInverse);

		++iter;
	}
}

void LoaderMD5Model::reset()
{
	//Make sure no data from previous files is present.
	if(infile_.is_open())
		infile_.close();
	
	fileLength_ = 0;
	
	joints_.clear();
	meshes_.clear();
}