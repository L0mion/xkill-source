
#include "objLoaderBasic.h"

ObjLoaderBasic::ObjLoaderBasic()
{
}

ObjLoaderBasic::~ObjLoaderBasic()
{
}

void ObjLoaderBasic::parseObjectFile(std::string filename, std::vector<VertexPosNormTex>* vertices)
{
	std::fstream infile(filename);
	std::string temp;

	positions_.clear();
	normals_.clear();
	texcoords_.clear();

	if(infile)
	{
		while(!infile.eof())
		{
			temp = "unknown";
			infile >> temp;

			if(temp == "v")
				parsePosition(infile);
			if(temp == "vn")
				parseNormal(infile);
			if(temp == "vt")
				parseTexcoord(infile);
			if(temp == "f")
				parseFace(infile, vertices);
			if(temp == "mtllib")
				parseMaterial(infile);

		}
	}
}

void ObjLoaderBasic::parsePosition(std::fstream& infile)
{
	float x, y, z;
	infile >> x >> y >> z;
	positions_.push_back(Float3(x, y, z));
}

void ObjLoaderBasic::parseNormal(std::fstream& infile)
{
	float x, y, z;
	infile >> x >> y >> z;
	normals_.push_back(Float3(x, y, z));
}

void ObjLoaderBasic::parseTexcoord(std::fstream& infile)
{
	float u, v;
	infile >> u >> v;
	texcoords_.push_back(Float2(u, v));
}

void ObjLoaderBasic::parseFace(std::fstream& infile, std::vector<VertexPosNormTex>* vertices)
{
	unsigned int iPosition, iNormal, iTexcoord;
	VertexPosNormTex vertex;

	for(unsigned int i=0; i<3; i++)
	{
		infile >> iPosition;
		vertex.position_ = positions_[iPosition-1];

		if(infile.peek() == '/')
		{
			infile.ignore();
			if(infile.peek() != '/')
			{
				infile >> iTexcoord;
				vertex.texcoord_ = texcoords_[iTexcoord-1];
			}
			if(infile.peek() == '/')
			{
				infile.ignore();
				infile >> iNormal;
				vertex.normal_ = normals_[iNormal-1];
			}
		}
		vertices->push_back(vertex);
	}
}

void ObjLoaderBasic::parseMaterial(std::fstream& infile)
{
	std::string mtlFilename;
	infile >> mtlFilename;

	std::fstream mtlFile(mtlFilename);

	std::string temp;
	std::string str;

	if(mtlFile)
	{
		while(!mtlFile.eof())
		{
			temp = "unknown";

			mtlFile >> temp;
			if(temp == "map_Kd")
			{
				mtlFile >> str;
				std::wstring str2(str.length(), L'');
				copy(str.begin(), str.end(), str2.begin());
				std::wstring textureFilename = str2;
			}
		}
	}
}