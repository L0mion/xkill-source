#include <MeshLoaderObj.h>

MeshLoaderObj::MeshLoaderObj(LPCWSTR mlFilePath)
	: MeshLoader(mlFilePath)
{
	lineNum_		= 0;
	curLine_		= "";	
	curSymbol_		= SYMBOL_IGNORE;

	curLineSplit_.clear();
	mlPosition_.clear();
	mlNormal_.clear();
	mlTex_.clear();
	mlFaces_.clear();
	mlVertices_.clear();
	mlIndices_.clear();
}
MeshLoaderObj::~MeshLoaderObj()
{
	curLineSplit_.clear();
	mlPosition_.clear();
	mlNormal_.clear();
	mlTex_.clear();
	mlFaces_.clear();
	mlVertices_.clear();
	mlIndices_.clear();
}

bool MeshLoaderObj::init()
{
	bool sucessfulLoad = true;

	LPCWSTR filename = getMLFilePath();
	mlIFS_.open(filename);

	if(mlIFS_.is_open())
	{
		sucessfulLoad = mlParseSymbols();
		if(mlIFS_.is_open())
			mlIFS_.close();
	}
	else
		sucessfulLoad = false;

	return sucessfulLoad;
}

bool MeshLoaderObj::mlParseSymbols()
{
	bool sucessfulLoad = true;

	while(!mlIFS_.eof() && sucessfulLoad)
	{
		mlGetLine(curLine_);
		curLineSplit_ = sss_.splitString(OBJ_SEPARATOR_DEFAULT, curLine_);

		sucessfulLoad = mlParseSymbol();
		sucessfulLoad = mlParseParams();

		if(sucessfulLoad)
		{
			switch(curSymbol_)
			{
			case SYMBOL_VERTEX:
				mlLoadVertex();
				break;
			case SYMBOL_TEX:
				mlLoadTex();
				break;
			case SYMBOL_NORM:
				mlLoadNorm();
				break;
			case SYMBOL_FACE:
				mlLoadFaces();
			case SYMBOL_IGNORE:
				//Do nothing.
				break;
			}
		}
		
		if(!sucessfulLoad)
			mlPrintFail();
	}

	return sucessfulLoad;
}
bool MeshLoaderObj::mlParseSymbol()
{
	bool sucessfulLoad = true;

	curSymbol_ = SYMBOL_IGNORE;
	if(curLineSplit_.size() > 0)
	{
		if(curLineSplit_.front()		== OBJ_INDICATOR_VERTEX)
			curSymbol_ = SYMBOL_VERTEX;
		else if(curLineSplit_.front()	== OBJ_INDICATOR_TEX)
			curSymbol_ = SYMBOL_TEX;
		else if(curLineSplit_.front()	== OBJ_INDICATOR_NORM)
			curSymbol_ = SYMBOL_NORM;
		else if(curLineSplit_.front()	== OBJ_INDICATOR_FACE)
			curSymbol_ = SYMBOL_FACE;
	}

	/*
	else if(symbol == OBJ_INDICATOR_SMOOTH)
	else if(symbol == OBJ_INDICATOR_GROUP)
	else if(symbol == OBJ_INDICATOR_LIB)
	*/

	return sucessfulLoad;
}

bool MeshLoaderObj::mlParseParams()
{
	bool sucessfulParse = false;

	switch(curSymbol_)
	{
	case SYMBOL_VERTEX:
		sucessfulParse = mlParseParam();
		break;
	case SYMBOL_TEX:
		sucessfulParse = mlParseParam();
		break;
	case SYMBOL_NORM:
		sucessfulParse = mlParseParam();
		break;
	case SYMBOL_FACE:
		sucessfulParse = true; //Faces are special case and are not parsed here.
		break;
	case SYMBOL_IGNORE:
		sucessfulParse = true;
		break;
	}

	return sucessfulParse;
}
bool MeshLoaderObj::mlParseParam()
{
	bool sucessfulParse = true;

	unsigned int numParams = curLineSplit_.size();
	if(numParams)
	{
		for(unsigned int i = OBJ_PARAMS; i < numParams; i++)
			if(!mlIsNumeric(curLineSplit_[i]))
				sucessfulParse = false;
	}

	return sucessfulParse;
}

void MeshLoaderObj::mlLoadVertex()
{
	float x, y, z;
	float w = 1.0f; //optional
	x = (float)::atof(curLineSplit_[OBJ_PARAMS_INDEX_VERTEX_X].c_str());
	y = (float)::atof(curLineSplit_[OBJ_PARAMS_INDEX_VERTEX_Y].c_str());
	z = (float)::atof(curLineSplit_[OBJ_PARAMS_INDEX_VERTEX_Z].c_str());

	if(curLineSplit_.size() == OBJ_PARAMS_NUM_VERTEX_OPTIONAL)
		w = (float)::atof(curLineSplit_[OBJ_PARAMS_INDEX_VERTEX_W].c_str());

	//do what with w?

	DirectX::XMFLOAT3 mlPosition(x, y, z);
	mlPosition_.push_back(mlPosition);
}
void MeshLoaderObj::mlLoadNorm()
{
	float x, y, z;
	x = (float)::atof(curLineSplit_[OBJ_PARAMS_INDEX_VERTEX_X].c_str());
	y = (float)::atof(curLineSplit_[OBJ_PARAMS_INDEX_VERTEX_Y].c_str());
	z = (float)::atof(curLineSplit_[OBJ_PARAMS_INDEX_VERTEX_Z].c_str());

	DirectX::XMFLOAT3 mlNormal(x, y, z);
	mlNormal_.push_back(mlNormal);
}
void MeshLoaderObj::mlLoadTex()
{
	float u, v;
	float w = 0.0f; //optional
	u = (float)::atof(curLineSplit_[OBJ_PARAMS_INDEX_TEX_U].c_str());
	v = (float)::atof(curLineSplit_[OBJ_PARAMS_INDEX_TEX_V].c_str());

	if(curLineSplit_.size() > OBJ_PARAMS_NUM_TEX)
		w = (float)::atof(curLineSplit_[OBJ_PARAMS_INDEX_TEX_W].c_str());

	DirectX::XMFLOAT2 mlTexCoord(u, v);
	mlTex_.push_back(mlTexCoord);
}
bool MeshLoaderObj::mlLoadFaces()
{
	bool sucessfulLoad = true;

	std::string v;
	std::vector<std::string> vSplit;
	
	for(unsigned int i = 0; i < 3 && sucessfulLoad; i++)
	{
		v = curLineSplit_[1 + i];
		vSplit = sss_.splitString(OBJ_SEPARATOR_FACE, v);

		sucessfulLoad = mlParseFace(vSplit);
		if(sucessfulLoad)
			mlLoadFace(vSplit);
	}

	return sucessfulLoad;
}
void MeshLoaderObj::mlLoadFace(std::vector<std::string>& face)
{
	unsigned int iPos, iTex, iNorm;
	iPos	= ::atoi(face[OBJ_PARAMS_INDEX_FACE_X].c_str());
	iTex	= ::atoi(face[OBJ_PARAMS_INDEX_FACE_Y].c_str());
	iNorm	= ::atoi(face[OBJ_PARAMS_INDEX_FACE_Z].c_str());
	MeshFace newFace(iPos, iTex, iNorm, 0);

	MeshFace preFace;
	bool foundFace = false;
	for(unsigned int i = 0; i < mlFaces_.size(); i++)
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
		vertex.texcoord_	= mlTex_[iTex	- 1];
		vertex.normal_		= mlNormal_[iNorm	- 1];
		mlVertices_.push_back(vertex);

		/*Push new face*/
		index = mlVertices_.size() - 1;
		newFace.setIndex(index);
		mlFaces_.push_back(newFace);
	}

	mlIndices_.push_back(index);
}
bool MeshLoaderObj::mlParseFace(std::vector<std::string>& splitFaces)
{
	bool sucessfulParse = true;

	if(splitFaces.size() == OBJ_PARAMS_NUM_FACE_V)
	{
		for(unsigned int i = 0; i < OBJ_PARAMS_NUM_FACE_V; i++)
			if(!mlIsNumeric(splitFaces[i]))
				sucessfulParse = false;
	}
	else
		sucessfulParse = false;

	return sucessfulParse;
}

void MeshLoaderObj::mlGetLine(std::string& line)
{
	std::getline(mlIFS_, line);
	lineNum_++;
}
void MeshLoaderObj::mlPrintFail()
{
	std::stringstream ss;
	ss << lineNum_;
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