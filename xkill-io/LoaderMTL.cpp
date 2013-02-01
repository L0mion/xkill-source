#include <xkill-utilities/Util.h>
#include "MTLMaterial.h"
#include "LoaderMTL.h"
#include "SimpleStringSplitter.h"

LoaderMTL::LoaderMTL(
	const std::string pathMTL,
	const std::string fileNameMTL) : Loader(pathMTL, fileNameMTL)
{
	sss_ = nullptr;
}
LoaderMTL::~LoaderMTL()
{
	if(sss_)
		delete sss_;
}

bool LoaderMTL::init()
{
	bool sucessfulLoad = true;

	lineNum_ = 0;
	sss_ = new SimpleStringSplitter();

	std::string fullPath = getFilePath() + getFileName();
	ifstream_.open(fullPath);

	if(ifstream_.is_open())
	{
		sucessfulLoad = parseMTL();
		if(ifstream_.is_open())
			ifstream_.close();
	}
	else
		sucessfulLoad = false;

	if(sucessfulLoad)
		loadMTL();

	return sucessfulLoad;
}

bool LoaderMTL::parseMTL()
{
	bool sucessfulLoad = true;

	MTLSymbol curSymbol = MTLSYMBOL_INVALID;
	std::string curLine = "";
	std::vector<std::string> curLineSplit;
	while(!ifstream_.eof() && sucessfulLoad)
	{
		getLine(curLine);
		curLineSplit = sss_->splitString(MTL_SEPARATOR_DEFAULT, curLine);

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
			printFail(curLine);
	}

	return sucessfulLoad;
}
MTLSymbol LoaderMTL::parseSymbol(std::vector<std::string>& params)
{
	MTLSymbol symbol = MTLSYMBOL_IGNORE;
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
bool LoaderMTL::parseParams(
	MTLSymbol symbol, 
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
bool LoaderMTL::parseParamsNumeric(const std::vector<std::string>& params)
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
bool LoaderMTL::parsePrerequisites(
	MTLSymbol symbol, 
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

void LoaderMTL::loadSymbol(
	const MTLSymbol symbol, 
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

void LoaderMTL::loadNewMTL(const std::vector<std::string>& params)
{
	std::string newMat;
	newMat = params[MTL_PARAMS_INDEX_NEWMTL_NAME];

	MTLMaterial mtl(newMat);
	materials_.push_back(newMat);
}
void LoaderMTL::loadIllum(const std::vector<std::string>& params)
{
	unsigned int illum;
	illum = ::atoi(params[MTL_PARAMS_INDEX_ILLUM_ID].c_str());
	
	materials_.back().addIllum(integerToIllum(illum));
}
void LoaderMTL::loadColor(
	const MTLSymbol symbol,
	const std::vector<std::string>& params)
{
	Float3 color = loadVector(params);
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
Float3 LoaderMTL::loadVector(const std::vector<std::string>& params)
{
	float x, y, z;
	x = (float)::atof(params[MTL_PARAMS_INDEX_COLOR_X].c_str());
	y = (float)::atof(params[MTL_PARAMS_INDEX_COLOR_Y].c_str());
	z = (float)::atof(params[MTL_PARAMS_INDEX_COLOR_Z].c_str());

	return Float3(x, y, z);
}
void LoaderMTL::loadTex(
		const MTLSymbol symbol,
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
std::string LoaderMTL::loadFilename(const std::vector<std::string>& params)
{
	std::string filename;
	filename = params[MTL_PARAMS_INDEX_TEX_FILENAME];

	return filename;
}
void LoaderMTL::loadOptical(const std::vector<std::string>& params)
{
	float opticalDensity = (float)::atof(params[MTL_PARAMS_INDEX_OPTICAL_DENSITY].c_str());

	materials_.back().setOpticalDensity(opticalDensity);
}

void LoaderMTL::loadMTL()
{
	mtl_ = MTL(materials_);
}

void LoaderMTL::printFail(const std::string curLine)
{
	std::string file	= getFilePath() + getFileName();
	std::string lineNum = std::to_string(lineNum_);
	std::string failed	= " parsing failed at line " + lineNum + ": " + curLine;

	SHOW_MESSAGEBOX(failed);
}

MTL LoaderMTL::getMTL()
{
	return mtl_;
}