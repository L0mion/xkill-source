#include <DirectXMath.h>

#include "vertices.h"
#include "ObjFace.h"
#include "ObjGroup.h"
#include "ObjGeometry.h"
#include "LoaderMTL.h"
#include "renderingUtilities.h"
#include "LoaderObj.h"

LoaderObj::LoaderObj(
	const std::string filePath,
	const std::string fileName)
	: Loader(filePath, fileName)
{
}
LoaderObj::~LoaderObj()
{
}

bool LoaderObj::init()
{
	bool sucessfulLoad = true;

	lineNum_ = 0;

	std::string fullPath = getFilePath() + getFileName();
	ifstream_.open(fullPath);

	if(ifstream_.is_open())
	{
		sucessfulLoad = parseObj();
		if(ifstream_.is_open())
			ifstream_.close();
	}
	else
		sucessfulLoad = false;

	if(sucessfulLoad)
		loadObj();

	return sucessfulLoad;
}

bool LoaderObj::parseObj()
{
	bool sucessfulLoad = true;

	ObjSymbol curSymbol = OBJSYMBOL_INVALID;
	std::string curLine = "";
	std::vector<std::string> curLineSplit;
	while(!ifstream_.eof() && sucessfulLoad)
	{
		getLine(curLine);
		curLineSplit = sss_.splitString(OBJ_SEPARATOR_DEFAULT, curLine);

		curSymbol = parseSymbol(curLineSplit);
		if(curSymbol != OBJSYMBOL_IGNORE)
		{
			sucessfulLoad	= parseParams(curSymbol, curLineSplit);
			if(sucessfulLoad)
				sucessfulLoad = loadSymbol(curSymbol, curLineSplit);
		}
		
		if(!sucessfulLoad)
			printFail(curLine);
	}

	return sucessfulLoad;
}
ObjSymbol LoaderObj::parseSymbol(const std::vector<std::string>& params)
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

bool LoaderObj::parseParams(
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
			sucessfulParse = parseParamsNumeric(params);
	}
	else
		sucessfulParse = false;

	return sucessfulParse;
}
bool LoaderObj::parseParamsNumeric(const std::vector<std::string>& params)
{
	bool sucessfulParse = true;

	unsigned int numParams = params.size();
	if(numParams)
	{
		for(unsigned int i = OBJ_PARAMS; i < numParams; i++)
			if(!isNumeric(params[i]))
				sucessfulParse = false;
	}

	return sucessfulParse;
}

bool LoaderObj::loadSymbol(
		const ObjSymbol symbol,
		const std::vector<std::string>& params)
{
	bool sucessfulLoad = true;

	switch(symbol)
	{
	case OBJSYMBOL_VERTEX:
		loadPos(params);
		break;
	case OBJSYMBOL_TEX:
		loadTex(params);
		break;
	case OBJSYMBOL_NORM:
		loadNorm(params);
		break;
	case OBJSYMBOL_FACE:
		loadFaces(params);
		break;
	case OBJSYMBOL_GROUP:
		loadGroup(params);
		break;
	case OBJSYMBOL_MATERIAL:
		loadMaterial(params);
		break;
	case OBJSYMBOL_MATERIAL_USE:
		sucessfulLoad = loadMaterialUse(params);
		break;
	}

	return sucessfulLoad;
}
void LoaderObj::loadPos(const std::vector<std::string>& params)
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
	position_.push_back(mlPosition);
}
void LoaderObj::loadNorm(const std::vector<std::string>& params)
{
	float x, y, z;
	x = (float)::atof(params[OBJ_PARAMS_INDEX_VERTEX_X].c_str());
	y = (float)::atof(params[OBJ_PARAMS_INDEX_VERTEX_Y].c_str());
	z = (float)::atof(params[OBJ_PARAMS_INDEX_VERTEX_Z].c_str());

	DirectX::XMFLOAT3 mlNormal(x, y, z);
	normal_.push_back(mlNormal);
}
void LoaderObj::loadTex(const std::vector<std::string>& params)
{
	float u, v;
	float w = 0.0f; //optional
	u = (float)::atof(params[OBJ_PARAMS_INDEX_TEX_U].c_str());
	v = (float)::atof(params[OBJ_PARAMS_INDEX_TEX_V].c_str());

	if(params.size() > OBJ_PARAMS_NUM_TEX)
		w = (float)::atof(params[OBJ_PARAMS_INDEX_TEX_W].c_str());

	DirectX::XMFLOAT2 mlTexCoord(u, v);
	tex_.push_back(mlTexCoord);
}
bool LoaderObj::loadFaces(const std::vector<std::string>& params)
{
	bool sucessfulLoad = true;

	std::string face;
	std::vector<std::string> faceSplit;
	
	for(unsigned int i = 0; i < 3 && sucessfulLoad; i++)
	{
		face = params[1 + i];
		faceSplit = sss_.splitString(OBJ_SEPARATOR_FACE, face);

		sucessfulLoad = parseFace(faceSplit);
		if(sucessfulLoad)
			sucessfulLoad = loadFace(faceSplit);
	}

	return sucessfulLoad;
}
bool LoaderObj::parseFace(const std::vector<std::string>& splitFaces)
{
	bool sucessfulParse = true;

	if(splitFaces.size() == OBJ_PARAMS_NUM_FACE)
	{
		for(unsigned int i = 0; i < OBJ_PARAMS_NUM_FACE; i++)
			if(!isNumeric(splitFaces[i]))
				sucessfulParse = false;
	}
	else
		sucessfulParse = false;

	return sucessfulParse;
}
bool LoaderObj::loadFace(const std::vector<std::string>& face)
{
	unsigned int iPos, iTex, iNorm;
	iPos	= ::atoi(face[OBJ_PARAMS_INDEX_FACE_X].c_str());
	iTex	= ::atoi(face[OBJ_PARAMS_INDEX_FACE_Y].c_str());
	iNorm	= ::atoi(face[OBJ_PARAMS_INDEX_FACE_Z].c_str());
	ObjFace newFace(iPos, iTex, iNorm, 0);

	ObjFace preFace;
	bool foundFace = false;
	for(int i = faces_.size() - 1; i >= 0; i--)
	{
		if(faces_[i] == newFace)
		{
			foundFace = true;
			preFace = faces_[i];
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
		index = loadVertex(iPos, iTex, iNorm);

		newFace.setIndex(index);
		faces_.push_back(newFace);
	}

	return loadIndex(index);
}
void LoaderObj::loadGroup(const std::vector<std::string>& params)
{
	std::string groupName;
	groupName = params[OBJ_PARAMS_INDEX_GROUP_NAME];
	
	ObjGroup newGroup(groupName);
	groups_.push_back(groupName);
}
void LoaderObj::loadMaterial(const std::vector<std::string>& params)
{
	std::string matFile = "";
	matFile = params[OBJ_PARAMS_INDEX_MATERIAL_NAME];

	mtlLib_.push_back(matFile); //store filename to read from later.
}
bool LoaderObj::loadMaterialUse(const std::vector<std::string>& params)
{
	bool sucessfulLoad = true;

	std::string mat = "";
	mat = params[OBJ_PARAMS_INDEX_MATERIAL_USE_NAME];

	if(groups_.size() > 0)
		groups_.back().setMaterialName(mat);

	return sucessfulLoad;
}
const unsigned int LoaderObj::loadVertex(
	const unsigned int iPos,
	const unsigned int iTex,
	const unsigned int iNorm)
{
	/*Create new vertex*/
	VertexPosNormTex vertex;
	vertex.position_	= position_[iPos	- 1];
	vertex.texcoord_	= tex_[iTex		- 1];
	vertex.normal_		= normal_[iNorm	- 1];
	vertices_.push_back(vertex);

	return vertices_.size() - 1;
}
bool LoaderObj::loadIndex(unsigned int index)
{
	bool sucessfulLoad = true;

	if(groups_.size() > 0)
		groups_.back().pushIndex(index);
	else
		sucessfulLoad = false; //no specified groups to push to

	return sucessfulLoad;
}

void LoaderObj::getLine(std::string& line)
{
	std::getline(ifstream_, line);
	lineNum_++;
}
void LoaderObj::printFail(const std::string curLine)
{
	std::string file	= getFilePath() + getFileName();
	std::string lineNum = std::to_string(lineNum_);
	std::string failed	= " parsing failed at line " + lineNum + ": " + curLine;

	ERROR_MSG(failed);
}
bool LoaderObj::isNumeric(const std::string value)
{
	std::stringstream conv;
	double tmp;
	conv << value;
	conv >> tmp;
	return conv.eof();
}

void LoaderObj::loadObj()
{
	ObjGeometry<VertexPosNormTex> mlGeometry(vertices_);
	for(unsigned int i = 0; i < groups_.size(); i++)
		mlGeometry.pushGroup(groups_[i]);

	ObjDependencies dependencies;
	for(unsigned int i = 0; i < mtlLib_.size(); i++)
		dependencies.pushDependencyMTL(mtlLib_[i]);
	obj_ = Obj(getFileName(), dependencies, mlGeometry);
}

Obj LoaderObj::getObj()
{
	return obj_;
}