#include "SimpleStringSplitter.h"
#include "LoaderTexDesc.h"

LoaderTexDesc::LoaderTexDesc(
	std::string fileName,
	std::string filePath) : Loader(filePath, fileName)
{
	texDesc_ = new VarStatus<TexDesc>(true);
}
LoaderTexDesc::~LoaderTexDesc()
{
	if(texDesc_)
		delete texDesc_;

	if(sss_)
		delete sss_;
}

bool LoaderTexDesc::init()
{
	bool sucessfulLoad = true;
	
	lineNum_ = 0;
	sss_ = new SimpleStringSplitter();
	
	std::string fullPath = getFilePath() + getFileName();
	ifstream_.open(fullPath);
	
	if(ifstream_.is_open())
	{
		sucessfulLoad = parseTexDesc();
		if(ifstream_.is_open())
			ifstream_.close();
	}
	else
		sucessfulLoad = false;
	
	if(sucessfulLoad)
		loadTexDesc();
	
	return sucessfulLoad;
}

TexDesc* LoaderTexDesc::claimTexDesc()
{
	texDesc_->setStatus(false);
	return texDesc_->getVar();
}

bool LoaderTexDesc::parseTexDesc()
{
	bool sucessfulLoad = true;

	TexDescSymbol curSymbol = TEXDESC_SYMBOL_INVALID;
	std::string curLine = "";
	std::vector<std::string> curLineSplit;
	while(!ifstream_.eof() && sucessfulLoad)
	{
		getLine(curLine);
		curLineSplit = sss_->splitString(TEXDESC_SEPARATOR_DEFAULT, curLine);

		curSymbol = parseSymbol(curLineSplit);
		if(curSymbol != TEXDESC_SYMBOL_IGNORE)
		{
			sucessfulLoad	= parseParams(curSymbol, curLineSplit);
			if(sucessfulLoad)
				sucessfulLoad = loadSymbol(curSymbol, curLineSplit);
		}

		//if(!sucessfulLoad)
		//	printFail(curLine);
	}

	return sucessfulLoad;
}
TexDescSymbol LoaderTexDesc::parseSymbol(const std::vector<std::string>& params)
{
	TexDescSymbol symbol = TEXDESC_SYMBOL_IGNORE;

	if(params.size() > 0)
	{
		char indicator = params.front().front();
		if(params.size() > 0)
		{
			if(indicator == TEXDESC_INDICATOR_HEADER)
				symbol = TEXDESC_SYMBOL_HEADER;
			if(indicator == TEXDESC_INDICATOR_TEXTURE)
				symbol = TEXDESC_SYMBOL_TEX;
		}
	}
	
	return symbol;
}
bool LoaderTexDesc::parseParams(
	const TexDescSymbol symbol, 
	const std::vector<std::string>& params)
{
	bool sucessfulParse = true;

	int		numExpectedParams	= 0;
	bool	expectedNumeric		= true;
	switch(symbol)
	{
	case TEXDESC_SYMBOL_HEADER:
		numExpectedParams = TEXDESC_PARAM_NUM_HEADER;
		break;
	case TEXDESC_SYMBOL_TEX:
		numExpectedParams = TEXDESC_PARAM_NUM_TEXTURE;
		break;
	case TEXDESC_SYMBOL_IGNORE:
		//Do nothing.
		break;
	}

	unsigned int numParams = params.size();
	if(numParams != numExpectedParams)
		sucessfulParse = false;

	return sucessfulParse;
}
bool LoaderTexDesc::loadSymbol(
	const TexDescSymbol symbol,
	const std::vector<std::string>& params)
{
	bool sucessfulLoad = true;

	switch(symbol)
	{
	case TEXDESC_SYMBOL_HEADER:
		loadHeader(params);
		break;
	case TEXDESC_SYMBOL_TEX:
		loadTex(params);
		break;
	}

	return sucessfulLoad;
}

void LoaderTexDesc::loadHeader(const std::vector<std::string>& params)
{
	std::string headerName;
	std::string texPath;

	headerName	= params[TEXDESC_PARAM_INDEX_HEADER_NAME].c_str();
	texPath		= params[TEXDESC_PARAM_INDEX_HEADER_PATH].c_str();

	header_ = TexDescHeader(headerName, texPath);
}
void LoaderTexDesc::loadTex(const std::vector<std::string>& params)
{
	unsigned int	texID;
	std::string		texPath;

	texID 	= static_cast<unsigned int>(::atof(params[TEXDESC_PARAM_INDEX_TEXTURE_ID].c_str()));
	texPath	= params[TEXDESC_PARAM_INDEX_TEXTURE_FILENAME].c_str();

	texs_.push_back(TexDescTex(texID, texPath));
}

void LoaderTexDesc::loadTexDesc()
{
	TexDesc* texDesc = new TexDesc(header_, texs_);
	texDesc_->setVar(texDesc);
}