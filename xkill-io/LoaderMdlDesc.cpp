#include "MdlDesc.h"
#include "SimpleStringSplitter.h"

#include "LoaderMdlDesc.h"

LoaderMdlDesc::LoaderMdlDesc(
	std::string fileName,
	std::string filePath) : Loader(filePath, fileName)
{
	mdlDesc_ = new VarStatus<MdlDesc>(true);
}
LoaderMdlDesc::~LoaderMdlDesc()
{
	if(mdlDesc_)
		delete mdlDesc_;

	if(sss_)
		delete sss_;

	//for(unsigned int i = 0; i < models_.size(); i++)
	//	if(models_[i])
	//		delete models_[i];
}

bool LoaderMdlDesc::init()
{
	bool sucessfulLoad = true;
	
	lineNum_ = 0;
	sss_ = new SimpleStringSplitter();
	
	std::string fullPath = getFilePath() + getFileName();
	ifstream_.open(fullPath);
	
	if(ifstream_.is_open())
	{
		sucessfulLoad = parseMdlDesc();
		if(ifstream_.is_open())
			ifstream_.close();
	}
	else
		sucessfulLoad = false;
	
	if(sucessfulLoad)
		loadMdlDesc();
	
	return sucessfulLoad;
}

MdlDesc* LoaderMdlDesc::claimMdlDesc()
{
	mdlDesc_->setStatus(false);
	return mdlDesc_->getVar();
}
bool LoaderMdlDesc::parseMdlDesc()
{
	bool sucessfulLoad = true;

	MdlDescSymbol curSymbol = MDLDESC_SYMBOL_INVALID;
	std::string curLine = "";
	std::vector<std::string> curLineSplit;
	while(!ifstream_.eof() && sucessfulLoad)
	{
		getLine(curLine);
		curLineSplit = sss_->splitString(MDLDESC_SEPARATOR_DEFAULT, curLine);

		curSymbol = parseSymbol(curLineSplit);
		if(curSymbol != MDLDESC_SYMBOL_IGNORE)
		{
			sucessfulLoad	= parseParams(curSymbol, curLineSplit);
			if(sucessfulLoad)
				sucessfulLoad = loadSymbol(curSymbol, curLineSplit);
		}
	}

	return sucessfulLoad;
}
MdlDescSymbol LoaderMdlDesc::parseSymbol(const std::vector<std::string>& params)
{
	MdlDescSymbol symbol = MDLDESC_SYMBOL_IGNORE;

	if(params.size() > 0)
	{
		char indicator = params.front().front();
		if(params.size() > 0)
		{
			if(indicator == MDLDESC_INDICATOR_HEADER)
				symbol = MDLDESC_SYMBOL_HEADER;
			if(indicator == MDLDESC_INDICATOR_MODEL)
				symbol = MDLDESC_SYMBOL_MODEL;
		}
	}
	
	return symbol;
}
bool LoaderMdlDesc::parseParams(
	const MdlDescSymbol symbol,
	const std::vector<std::string>& params)
{
	bool sucessfulParse = true;

	int		numExpectedParams	= 0;
	bool	expectedNumeric		= true;
	switch(symbol)
	{
	case MDLDESC_SYMBOL_HEADER:
		numExpectedParams = MDLDESC_PARAM_NUM_HEADER;
		break;
	case MDLDESC_SYMBOL_MODEL:
		numExpectedParams = MDLDESC_PARAM_NUM_MODEL;
		break;
	case MDLDESC_SYMBOL_IGNORE:
		//Do nothing.
		break;
	}

	unsigned int numParams = params.size();
	if(numParams != numExpectedParams)
		sucessfulParse = false;

	return sucessfulParse;
}
bool LoaderMdlDesc::loadSymbol(
	const MdlDescSymbol symbol,
	const std::vector<std::string>& params)
{
	bool sucessfulLoad = true;

	switch(symbol)
	{
	case MDLDESC_SYMBOL_HEADER:
		loadHeader(params);
		break;
	case MDLDESC_SYMBOL_MODEL:
		loadModel(params);
		break;
	}

	return sucessfulLoad;
}

void LoaderMdlDesc::loadHeader(const std::vector<std::string>& params)
{
	std::string headerName;
	std::string modelPath;

	headerName	= params[MDLDESC_PARAM_INDEX_HEADER_NAME].c_str();
	modelPath	= params[MDLDESC_PARAM_INDEX_HEADER_PATH].c_str();

	header_ = MdlDescHeader(headerName, modelPath);
}
void LoaderMdlDesc::loadModel(const std::vector<std::string>& params)
{
	unsigned int	modelID;
	std::string		modelName;
	std::string		modelDynamic;

	modelID 		= static_cast<unsigned int>(::atof(params[MDLDESC_PARAM_INDEX_MODEL_ID].c_str()));
	modelName		= params[MDLDESC_PARAM_INDEX_MODEL_FILENAME].c_str();
	modelDynamic	= params[MDLDESC_PARAM_INDEX_MODEL_DYNAMIC].c_str();

	models_.push_back(new MdlDescModel(
		modelID, 
		modelName, 
		modelDynamic != "0"));
}

void LoaderMdlDesc::loadMdlDesc()
{
	std::string path = this->getFilePath();
	
	MdlDesc* mdlDesc = new MdlDesc(
		path,
		header_,
		models_);
	mdlDesc_->setVar(mdlDesc);
}