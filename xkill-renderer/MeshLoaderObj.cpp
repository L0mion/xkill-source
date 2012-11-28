#include <MeshLoaderObj.h>

MeshLoaderObj::MeshLoaderObj(const LPCWSTR mlFilePath)
	: MeshLoader(mlFilePath)
{
	mlLineNum_		= 0;
}
MeshLoaderObj::~MeshLoaderObj()
{
}

bool MeshLoaderObj::init()
{
	bool sucessfulLoad = true;

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

	return sucessfulLoad;
}

bool MeshLoaderObj::mlParseObj()
{
	bool sucessfulLoad = true;

	ObjSymbol curSymbol = SYMBOL_INVALID;
	std::string curLine = "";
	std::vector<std::string> curLineSplit;
	while(!mlIFS_.eof() && sucessfulLoad)
	{
		mlGetLine(curLine);
		curLineSplit = mlSSS_.splitString(OBJ_SEPARATOR_DEFAULT, curLine);

		curSymbol = mlParseSymbol(curLineSplit);
		if(curSymbol != SYMBOL_IGNORE)
		{
			sucessfulLoad	= mlParseParams(curSymbol, curLineSplit);
			if(sucessfulLoad)
				mlLoadSymbol(curSymbol, curLineSplit);
		}
		
		if(!sucessfulLoad)
			mlPrintFail();
	}

	return sucessfulLoad;
}
ObjSymbol MeshLoaderObj::mlParseSymbol(const std::vector<std::string>& params)
{
	ObjSymbol symbol = SYMBOL_IGNORE;
	if(params.size() > 0)
	{
		if(params.front()		== OBJ_INDICATOR_VERTEX)
			symbol = SYMBOL_VERTEX;
		else if(params.front()	== OBJ_INDICATOR_TEX)
			symbol = SYMBOL_TEX;
		else if(params.front()	== OBJ_INDICATOR_NORM)
			symbol = SYMBOL_NORM;
		else if(params.front()	== OBJ_INDICATOR_FACE)
			symbol = SYMBOL_FACE;
		else if(params.front()	== OBJ_INDICATOR_GROUP)
			symbol = SYMBOL_GROUP;
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
	case SYMBOL_VERTEX:
		numExpectedParams = OBJ_PARAMS_NUM_VERTEX;
		numOptionalParams = OBJ_PARAMS_NUM_VERTEX_OPTIONAL;
		break;
	case SYMBOL_TEX:
		numExpectedParams = OBJ_PARAMS_NUM_TEX; //at least NUM_TEX params
		numOptionalParams = OBJ_PARAMS_NUM_TEX_OPTIONAL;
		break;
	case SYMBOL_NORM:
		numExpectedParams = OBJ_PARAMS_NUM_NORM;
		break;
	case SYMBOL_FACE:
		numExpectedParams	= OBJ_PARAMS_NUM_FACES;
		expectedNumeric		= false;
		break;
	case SYMBOL_GROUP:
		numExpectedParams	= OBJ_PARAMS_NUM_GROUP;
		expectedNumeric		= false;
		break;
	case SYMBOL_IGNORE:
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

void MeshLoaderObj::mlLoadSymbol(
		const ObjSymbol symbol,
		const std::vector<std::string>& params)
{
	switch(symbol)
	{
	case SYMBOL_VERTEX:
		mlLoadVertex(params);
		break;
	case SYMBOL_TEX:
		mlLoadTex(params);
		break;
	case SYMBOL_NORM:
		mlLoadNorm(params);
		break;
	case SYMBOL_FACE:
		mlLoadFaces(params);
		break;
	case SYMBOL_GROUP:
		mlLoadGroup(params);
		break;
	}
}
void MeshLoaderObj::mlLoadVertex(const std::vector<std::string>& params)
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
			mlLoadFace(faceSplit);
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
void MeshLoaderObj::mlLoadFace(const std::vector<std::string>& face)
{
	unsigned int iPos, iTex, iNorm;
	iPos	= ::atoi(face[OBJ_PARAMS_INDEX_FACE_X].c_str());
	iTex	= ::atoi(face[OBJ_PARAMS_INDEX_FACE_Y].c_str());
	iNorm	= ::atoi(face[OBJ_PARAMS_INDEX_FACE_Z].c_str());
	MeshFace newFace(iPos, iTex, iNorm, 0);

	MeshFace preFace;
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
		/*Create new vertex*/
		VertexPosNormTex vertex;
		vertex.position_	= mlPosition_[iPos	- 1];
		vertex.texcoord_	= mlTex_[iTex		- 1];
		vertex.normal_		= mlNormal_[iNorm	- 1];
		mlVertices_.push_back(vertex);

		/*Push new face*/
		index = mlVertices_.size() - 1;
		newFace.setIndex(index);
		mlFaces_.push_back(newFace);
	}

	mlIndices_.push_back(index);
}
void MeshLoaderObj::mlLoadGroup(const std::vector<std::string>& params)
{
	std::string groupName;
	groupName = params[OBJ_PARAMS_INDEX_GROUP_NAME];
	
	MeshGroup newGroup(groupName);
	mlGroups_.push_back(groupName);
}

bool MeshLoaderObj::mlLoadIndex(unsigned int)
{
	return true; //temp
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

std::vector<VertexPosNormTex> MeshLoaderObj::getMLVertices()
{
	return mlVertices_;
}
std::vector<unsigned int> MeshLoaderObj::getMLIndices()
{
	return mlIndices_;
}