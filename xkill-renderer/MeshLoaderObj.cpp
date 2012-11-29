#include <DirectXMath.h>

#include "vertices.h"
#include "ObjFace.h"
#include "MeshGroup.h"
#include "MeshGeometry.h"
#include "MeshLoaderMTL.h"
#include "renderingUtilities.h"
#include "MeshLoaderObj.h"

MeshLoaderObj::MeshLoaderObj(
	const LPCWSTR mlFileName,
	const LPCWSTR mlFilePath)
	: MeshLoader(mlFilePath)
{
	mlFileName_		= mlFileName;
}
MeshLoaderObj::~MeshLoaderObj()
{
}

bool MeshLoaderObj::init()
{
	bool sucessfulLoad = true;

	mlLineNum_ = 0;

	LPCWSTR filename = getMLFilePath();
	mlIFS_.open(filename);

	if(mlIFS_.is_open())
	{
		sucessfulLoad = mlParseObj();
		if(mlIFS_.is_open())
			mlIFS_.close();
	}
	else
		sucessfulLoad = false;

	if(sucessfulLoad)
		mlLoadObj();

	return sucessfulLoad;
}

bool MeshLoaderObj::mlParseObj()
{
	bool sucessfulLoad = true;

	ObjSymbol curSymbol = OBJSYMBOL_INVALID;
	std::string curLine = "";
	std::vector<std::string> curLineSplit;
	while(!mlIFS_.eof() && sucessfulLoad)
	{
		mlGetLine(curLine);
		curLineSplit = mlSSS_.splitString(OBJ_SEPARATOR_DEFAULT, curLine);

		curSymbol = mlParseSymbol(curLineSplit);
		if(curSymbol != OBJSYMBOL_IGNORE)
		{
			sucessfulLoad	= mlParseParams(curSymbol, curLineSplit);
			if(sucessfulLoad)
				sucessfulLoad = mlLoadSymbol(curSymbol, curLineSplit);
		}
		
		if(!sucessfulLoad)
			mlPrintFail();
	}

	return sucessfulLoad;
}
ObjSymbol MeshLoaderObj::mlParseSymbol(const std::vector<std::string>& params)
{
	ObjSymbol symbol = OBJSYMBOL_IGNORE;
	if(params.size() > 0)
	{
		if(params.front()		== OBJ_INDICATOR_VERTEX)
			symbol = OBJSYMBOL_VERTEX;
		else if(params.front()	== OBJ_INDICATOR_TEX)
			symbol = OBJSYMBOL_TEX;
		else if(params.front()	== OBJ_INDICATOR_NORM)
			symbol = OBJSYMBOL_NORM;
		else if(params.front()	== OBJ_INDICATOR_FACE)
			symbol = OBJSYMBOL_FACE;
		else if(params.front()	== OBJ_INDICATOR_GROUP)
			symbol = OBJSYMBOL_GROUP;
		else if(params.front()	== OBJ_INDICATOR_MATERIAL)
			symbol = OBJSYMBOL_MATERIAL;
		else if(params.front()	== OBJ_INDICATOR_MATERIAL_USE)
			symbol = OBJSYMBOL_MATERIAL_USE;
	}

	return symbol;
}

bool MeshLoaderObj::mlParseParams(
	const ObjSymbol symbol,
	const std::vector<std::string>& params)
{
	bool sucessfulParse = true;

	int		numExpectedParams	= 0;
	int		numOptionalParams	= 0;
	bool	expectedNumeric		= true;
	switch(symbol)
	{
	case OBJSYMBOL_VERTEX:
		numExpectedParams = OBJ_PARAMS_NUM_VERTEX;
		numOptionalParams = OBJ_PARAMS_NUM_VERTEX_OPTIONAL;
		break;
	case OBJSYMBOL_TEX:
		numExpectedParams = OBJ_PARAMS_NUM_TEX; //at least NUM_TEX params
		numOptionalParams = OBJ_PARAMS_NUM_TEX_OPTIONAL;
		break;
	case OBJSYMBOL_NORM:
		numExpectedParams = OBJ_PARAMS_NUM_NORM;
		break;
	case OBJSYMBOL_FACE:
		numExpectedParams	= OBJ_PARAMS_NUM_FACES;
		expectedNumeric		= false;
		break;
	case OBJSYMBOL_GROUP:
		numExpectedParams	= OBJ_PARAMS_NUM_GROUP;
		expectedNumeric		= false;
		break;
	case OBJSYMBOL_MATERIAL:
		numExpectedParams	= OBJ_PARAMS_NUM_MATERIAL;
		expectedNumeric		= false;
		break;
	case OBJSYMBOL_MATERIAL_USE:
		numExpectedParams	= OBJ_PARAMS_NUM_MATERIAL_USE;
		expectedNumeric		= false;
		break;
	case OBJSYMBOL_IGNORE:
		//Do nothing.
		break;
	}

	unsigned int numParams = params.size();
	if(
		numParams == numExpectedParams ||
		numParams == numExpectedParams + numOptionalParams)
	{
		if(expectedNumeric)
			sucessfulParse = mlParseParamsNumeric(params);
	}
	else
		sucessfulParse = false;

	return sucessfulParse;
}
bool MeshLoaderObj::mlParseParamsNumeric(const std::vector<std::string>& params)
{
	bool sucessfulParse = true;

	unsigned int numParams = params.size();
	if(numParams)
	{
		for(unsigned int i = OBJ_PARAMS; i < numParams; i++)
			if(!mlIsNumeric(params[i]))
				sucessfulParse = false;
	}

	return sucessfulParse;
}

bool MeshLoaderObj::mlLoadSymbol(
		const ObjSymbol symbol,
		const std::vector<std::string>& params)
{
	bool sucessfulLoad = true;

	switch(symbol)
	{
	case OBJSYMBOL_VERTEX:
		mlLoadPos(params);
		break;
	case OBJSYMBOL_TEX:
		mlLoadTex(params);
		break;
	case OBJSYMBOL_NORM:
		mlLoadNorm(params);
		break;
	case OBJSYMBOL_FACE:
		mlLoadFaces(params);
		break;
	case OBJSYMBOL_GROUP:
		mlLoadGroup(params);
		break;
	case OBJSYMBOL_MATERIAL:
		mlLoadMaterial(params);
		break;
	case OBJSYMBOL_MATERIAL_USE:
		sucessfulLoad = mlLoadMaterialUse(params);
		break;
	}

	return sucessfulLoad;
}
void MeshLoaderObj::mlLoadPos(const std::vector<std::string>& params)
{
	float x, y, z;
	float w = 1.0f; //optional
	x = (float)::atof(params[OBJ_PARAMS_INDEX_VERTEX_X].c_str());
	y = (float)::atof(params[OBJ_PARAMS_INDEX_VERTEX_Y].c_str());
	z = (float)::atof(params[OBJ_PARAMS_INDEX_VERTEX_Z].c_str());

	if(params.size() == OBJ_PARAMS_NUM_VERTEX_OPTIONAL)
		w = (float)::atof(params[OBJ_PARAMS_INDEX_VERTEX_W].c_str());

	//do what with w?

	DirectX::XMFLOAT3 mlPosition(x, y, z);
	mlPosition_.push_back(mlPosition);
}
void MeshLoaderObj::mlLoadNorm(const std::vector<std::string>& params)
{
	float x, y, z;
	x = (float)::atof(params[OBJ_PARAMS_INDEX_VERTEX_X].c_str());
	y = (float)::atof(params[OBJ_PARAMS_INDEX_VERTEX_Y].c_str());
	z = (float)::atof(params[OBJ_PARAMS_INDEX_VERTEX_Z].c_str());

	DirectX::XMFLOAT3 mlNormal(x, y, z);
	mlNormal_.push_back(mlNormal);
}
void MeshLoaderObj::mlLoadTex(const std::vector<std::string>& params)
{
	float u, v;
	float w = 0.0f; //optional
	u = (float)::atof(params[OBJ_PARAMS_INDEX_TEX_U].c_str());
	v = (float)::atof(params[OBJ_PARAMS_INDEX_TEX_V].c_str());

	if(params.size() > OBJ_PARAMS_NUM_TEX)
		w = (float)::atof(params[OBJ_PARAMS_INDEX_TEX_W].c_str());

	DirectX::XMFLOAT2 mlTexCoord(u, v);
	mlTex_.push_back(mlTexCoord);
}
bool MeshLoaderObj::mlLoadFaces(const std::vector<std::string>& params)
{
	bool sucessfulLoad = true;

	std::string face;
	std::vector<std::string> faceSplit;
	
	for(unsigned int i = 0; i < 3 && sucessfulLoad; i++)
	{
		face = params[1 + i];
		faceSplit = mlSSS_.splitString(OBJ_SEPARATOR_FACE, face);

		sucessfulLoad = mlParseFace(faceSplit);
		if(sucessfulLoad)
			sucessfulLoad = mlLoadFace(faceSplit);
	}

	return sucessfulLoad;
}
bool MeshLoaderObj::mlParseFace(const std::vector<std::string>& splitFaces)
{
	bool sucessfulParse = true;

	if(splitFaces.size() == OBJ_PARAMS_NUM_FACE)
	{
		for(unsigned int i = 0; i < OBJ_PARAMS_NUM_FACE; i++)
			if(!mlIsNumeric(splitFaces[i]))
				sucessfulParse = false;
	}
	else
		sucessfulParse = false;

	return sucessfulParse;
}
bool MeshLoaderObj::mlLoadFace(const std::vector<std::string>& face)
{
	unsigned int iPos, iTex, iNorm;
	iPos	= ::atoi(face[OBJ_PARAMS_INDEX_FACE_X].c_str());
	iTex	= ::atoi(face[OBJ_PARAMS_INDEX_FACE_Y].c_str());
	iNorm	= ::atoi(face[OBJ_PARAMS_INDEX_FACE_Z].c_str());
	ObjFace newFace(iPos, iTex, iNorm, 0);

	ObjFace preFace;
	bool foundFace = false;
	for(int i = mlFaces_.size() - 1; i >= 0; i--)
	{
		if(mlFaces_[i] == newFace)
		{
			foundFace = true;
			preFace = mlFaces_[i];
			break;
		}
	}

	unsigned int index;
	if(foundFace)
	{ 
		index = preFace.getIndex();
	}
	else
	{
		index = mlLoadVertex(iPos, iTex, iNorm);

		newFace.setIndex(index);
		mlFaces_.push_back(newFace);
	}

	return mlLoadIndex(index);
}
void MeshLoaderObj::mlLoadGroup(const std::vector<std::string>& params)
{
	std::string groupName;
	groupName = params[OBJ_PARAMS_INDEX_GROUP_NAME];
	
	MeshGroup newGroup(groupName);
	mlGroups_.push_back(groupName);
}
void MeshLoaderObj::mlLoadMaterial(const std::vector<std::string>& params)
{
	std::string matFile = "";
	matFile = params[OBJ_PARAMS_INDEX_MATERIAL_NAME];

	mlMtlLib_.push_back(matFile); //store filename to read from later.
}
bool MeshLoaderObj::mlLoadMaterialUse(const std::vector<std::string>& params)
{
	bool sucessfulLoad = true;

	std::string mat = "";
	mat = params[OBJ_PARAMS_INDEX_MATERIAL_USE_NAME];

	if(mlGroups_.size() > 0)
		mlGroups_.back().setMGMaterial(mat);

	return sucessfulLoad;
}
const unsigned int MeshLoaderObj::mlLoadVertex(
	const unsigned int iPos,
	const unsigned int iTex,
	const unsigned int iNorm)
{
	/*Create new vertex*/
	VertexPosNormTex vertex;
	vertex.position_	= mlPosition_[iPos	- 1];
	vertex.texcoord_	= mlTex_[iTex		- 1];
	vertex.normal_		= mlNormal_[iNorm	- 1];
	mlVertices_.push_back(vertex);

	return mlVertices_.size() - 1;
}
bool MeshLoaderObj::mlLoadIndex(unsigned int index)
{
	bool sucessfulLoad = true;

	if(mlGroups_.size() > 0)
		mlGroups_.back().mgPushIndex(index);
	else
		sucessfulLoad = false; //no specified groups to push to

	return sucessfulLoad;
}

void MeshLoaderObj::mlGetLine(std::string& line)
{
	std::getline(mlIFS_, line);
	mlLineNum_++;
}
void MeshLoaderObj::mlPrintFail()
{
	std::stringstream ss;
	ss << mlLineNum_;
	std::string line = ss.str();
	std::wstring lineW;
	lineW.assign(line.begin(), line.end());

	std::wstring file		= getMLFilePath();
	std::wstring failed		= L" parsing failed at line ";
	std::wstring errorMsg	= file + failed + lineW;
	ERROR_MSG(errorMsg.c_str());
}
bool MeshLoaderObj::mlIsNumeric(std::string value)
{
	std::stringstream conv;
	double tmp;
	conv << value;
	conv >> tmp;
	return conv.eof();
}

void MeshLoaderObj::mlLoadObj()
{
	MeshGeometry<VertexPosNormTex> mlGeometry(mlVertices_);
	for(unsigned int i = 0; i < mlGroups_.size(); i++)
		mlGeometry.mgPushGroup(mlGroups_[i]);

	ObjDependencies dependencies;
	for(unsigned int i = 0; i < mlMtlLib_.size(); i++)
		dependencies.pushDependencyMTL(mlMtlLib_[i]);
	mlObj_ = Obj(mlFileName_, dependencies, mlGeometry);
}

Obj MeshLoaderObj::getMLObj()
{
	return mlObj_;
}