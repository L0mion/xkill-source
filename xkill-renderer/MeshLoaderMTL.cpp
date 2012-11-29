#include "MTLMaterial.h"
#include "MeshLoaderMTL.h"

MeshLoaderMTL::MeshLoaderMTL(LPCWSTR mtlPath) : MeshLoader(mtlPath)
{
	//Do nothing.
}
MeshLoaderMTL::~MeshLoaderMTL()
{
	//Do nothing.
}

bool MeshLoaderMTL::init()
{
	bool sucessfulLoad = true;

	lineNum_ = 0;

	LPCWSTR filename = getMLFilePath();
	mlIFS_.open(filename);

	if(mlIFS_.is_open())
	{
		sucessfulLoad = parseMTL();
		if(mlIFS_.is_open())
			mlIFS_.close();
	}
	else
		sucessfulLoad = false;

	if(sucessfulLoad)
		loadMTL();

	return sucessfulLoad;
}

bool MeshLoaderMTL::parseMTL()
{
	bool sucessfulLoad = true;

	MtlSymbol curSymbol = MTLSYMBOL_INVALID;
	std::string curLine = "";
	std::vector<std::string> curLineSplit;
	while(!mlIFS_.eof() && sucessfulLoad)
	{
		getLine(curLine);
		curLineSplit = sss_.splitString(MTL_SEPARATOR_DEFAULT, curLine);

		curSymbol = parseSymbol(curLineSplit);
		if(curSymbol != MTLSYMBOL_IGNORE)
		{
			sucessfulLoad = parseParams(curSymbol, curLineSplit);
			if(sucessfulLoad)
				sucessfulLoad = parsePrerequisites(curSymbol, curLineSplit);
			if(sucessfulLoad)
				loadSymbol(curSymbol, curLineSplit);
		}
		
		if(!sucessfulLoad)
			printFail();
	}

	if(sucessfulLoad)
		loadMTL();

	return sucessfulLoad;
}
MtlSymbol MeshLoaderMTL::parseSymbol(std::vector<std::string>& params)
{
	MtlSymbol symbol = MTLSYMBOL_IGNORE;
	if(params.size() > 0)
	{
		std::string front = params.front();
		if(front == MTL_INDICATOR_NEWMTL)
			symbol = MTLSYMBOL_NEWMTL;
		if(front == MTL_INDICATOR_ILLUM)
			symbol = MTLSYMBOL_ILLUM;
		
		if(front == MTL_INDICATOR_COLOR_DIFFUSE)
			symbol = MTLSYMBOL_COLOR_DIFFUSE;
		if(front == MTL_INDICATOR_COLOR_AMBIENT)
			symbol = MTLSYMBOL_COLOR_AMBIENT;
		if(front == MTL_INDICATOR_COLOR_SPECULAR)
			symbol = MTLSYMBOL_COLOR_SPECULAR;
		if(front == MTL_INDICATOR_COLOR_TF)
			symbol = MTLSYMBOL_COLOR_TF;

		if(front == MTL_INDICATOR_TEX_DIFFUSE)
			symbol = MTLSYMBOL_TEX_DIFFUSE;
		if(front == MTL_INDICATOR_TEX_AMBIENT)
			symbol = MTLSYMBOL_TEX_AMBIENT;
		if(front == MTL_INDICATOR_TEX_SPECULAR)
			symbol = MTLSYMBOL_TEX_SPECULAR;
		if(front == MTL_INDICATOR_TEX_SPECPOW)
			symbol = MTLSYMBOL_TEX_SPECPOW;
		if(front == MTL_INDICATOR_TEX_ALPHA)
			symbol = MTLSYMBOL_TEX_ALPHA;

		if(front == MTL_INDICATOR_OPTICAL)
			symbol = MTLSYMBOL_OPTICAL;
	}

	return symbol;
}
bool MeshLoaderMTL::parseParams(
	MtlSymbol symbol, 
	std::vector<std::string>& params)
{
	bool sucessfulParse = true;

	int		numExpectedParams	= 0;
	int		numOptionalParams	= 0;
	bool	expectedNumeric		= true;

	if(symbol == MTLSYMBOL_NEWMTL)
	{
		numExpectedParams	= MTL_PARAMS_NUM_NEWMTL;
		expectedNumeric		= false;
	}
	if(symbol == MTLSYMBOL_ILLUM)
	{
		numExpectedParams	= MTL_PARAMS_NUM_ILLUM;
	}
	if(symbol == MTLSYMBOL_OPTICAL)
	{
		numExpectedParams	= MTL_PARAMS_NUM_OPTICAL;
	}
	if(
		symbol == MTLSYMBOL_COLOR_DIFFUSE	||
		symbol == MTLSYMBOL_COLOR_AMBIENT	||
		symbol == MTLSYMBOL_COLOR_SPECULAR	||
		symbol == MTLSYMBOL_COLOR_TF)
	{
		numExpectedParams	= MTL_PARAMS_NUM_COLOR;
	}
	if(
		symbol == MTLSYMBOL_TEX_DIFFUSE		||
		symbol == MTLSYMBOL_TEX_AMBIENT		||
		symbol == MTLSYMBOL_TEX_SPECULAR	||
		symbol == MTLSYMBOL_TEX_SPECPOW		||
		symbol == MTLSYMBOL_TEX_ALPHA)
	{
		numExpectedParams = MTL_PARAMS_NUM_TEX;
		expectedNumeric = false;
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
bool MeshLoaderMTL::parseParamsNumeric(const std::vector<std::string>& params)
{
	bool sucessfulParse = true;

	unsigned int numParams = params.size();
	if(numParams)
	{
		for(unsigned int i = MTL_PARAMS; i < numParams; i++)
			if(!isNumeric(params[i]))
				sucessfulParse = false;
	}

	return sucessfulParse;
}
bool MeshLoaderMTL::parsePrerequisites(
	MtlSymbol symbol, 
	const std::vector<std::string>& params)
{
	bool sucessfulParse = true;

	bool requiresExistingMTL = true;
	switch(symbol)
	{
	case MTLSYMBOL_NEWMTL:
		requiresExistingMTL = false;
		break;
	};

	if(requiresExistingMTL)
		if(materials_.size() <= 0)
			sucessfulParse = false;

	return sucessfulParse;
}

void MeshLoaderMTL::loadSymbol(
	const MtlSymbol symbol, 
	const std::vector<std::string>& params)
{
	if(symbol == MTLSYMBOL_NEWMTL)
		loadNewMTL(params);
	if(symbol == MTLSYMBOL_ILLUM)
		loadIllum(params);
	if(symbol == MTLSYMBOL_OPTICAL)
		loadOptical(params);
	if(
		symbol == MTLSYMBOL_COLOR_DIFFUSE	||
		symbol == MTLSYMBOL_COLOR_AMBIENT	||
		symbol == MTLSYMBOL_COLOR_SPECULAR	||
		symbol == MTLSYMBOL_COLOR_TF)
	{
		loadColor(symbol, params);
	}
	if(
		symbol == MTLSYMBOL_TEX_DIFFUSE		||
		symbol == MTLSYMBOL_TEX_AMBIENT		||
		symbol == MTLSYMBOL_TEX_SPECULAR	||
		symbol == MTLSYMBOL_TEX_SPECPOW		||
		symbol == MTLSYMBOL_TEX_ALPHA)
	{
		loadTex(symbol, params);
	}
}

void MeshLoaderMTL::loadNewMTL(const std::vector<std::string>& params)
{
	std::string newMat;
	newMat = params[MTL_PARAMS_INDEX_NEWMTL_NAME];

	MTLMaterial mtl(newMat);
	materials_.push_back(newMat);
}
void MeshLoaderMTL::loadIllum(const std::vector<std::string>& params)
{
	unsigned int illum;
	illum = ::atoi(params[MTL_PARAMS_INDEX_ILLUM_ID].c_str());
	
	materials_.back().addIllum(integerToIllum(illum));
}
void MeshLoaderMTL::loadColor(
	const MtlSymbol symbol,
	const std::vector<std::string>& params)
{
	DirectX::XMFLOAT3 color = loadVector(params);
	switch(symbol)
	{
	case MTLSYMBOL_COLOR_DIFFUSE:
		materials_.back().setDiffuseColor(color);
		break;
	case MTLSYMBOL_COLOR_AMBIENT:
		materials_.back().setAmbientColor(color);
		break;
	case MTLSYMBOL_COLOR_SPECULAR:
		materials_.back().setSpecularColor(color);
		break;
	case MTLSYMBOL_COLOR_TF:
		materials_.back().setTF(color);
		break;
	};
	
}
DirectX::XMFLOAT3 MeshLoaderMTL::loadVector(const std::vector<std::string>& params)
{
	float x, y, z;
	x = (float)::atof(params[MTL_PARAMS_INDEX_COLOR_X].c_str());
	y = (float)::atof(params[MTL_PARAMS_INDEX_COLOR_Y].c_str());
	z = (float)::atof(params[MTL_PARAMS_INDEX_COLOR_Z].c_str());

	return DirectX::XMFLOAT3(x, y, z);
}
void MeshLoaderMTL::loadTex(
		const MtlSymbol symbol,
		const std::vector<std::string>& params)
{
	std::string filename = loadFilename(params);
	switch(symbol)
	{
	case MTLSYMBOL_TEX_DIFFUSE:
		materials_.back().setTexDiffuse(filename);
		break;
	case MTLSYMBOL_TEX_AMBIENT:
		materials_.back().setTexAmbient(filename);
		break;
	case MTLSYMBOL_TEX_SPECULAR:
		materials_.back().setTexSpecular(filename);
		break;
	case MTLSYMBOL_TEX_SPECPOW:
		materials_.back().setTexSpecularPow(filename);
		break;
	case MTLSYMBOL_TEX_ALPHA:
		materials_.back().setTexAlpha(filename);
		break;
	};
}
std::string MeshLoaderMTL::loadFilename(const std::vector<std::string>& params)
{
	std::string filename;
	filename = params[MTL_PARAMS_INDEX_TEX_FILENAME];

	return filename;
}
void MeshLoaderMTL::loadOptical(const std::vector<std::string>& params)
{
	float opticalDensity = (float)::atof(params[MTL_PARAMS_INDEX_OPTICAL_DENSITY].c_str());

	materials_.back().setOpticalDensity(opticalDensity);
}

void MeshLoaderMTL::loadMTL()
{
	mtl_ = MTL(materials_);
}

void MeshLoaderMTL::getLine(std::string& line)
{
	std::getline(mlIFS_, line);
	lineNum_++;
}
void MeshLoaderMTL::printFail()
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
bool MeshLoaderMTL::isNumeric(std::string value)
{
	std::stringstream conv;
	double tmp;
	conv << value;
	conv >> tmp;
	return conv.eof();
}

MTL MeshLoaderMTL::getMTL()
{
	return mtl_;
}