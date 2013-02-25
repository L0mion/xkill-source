#include "MdlDesc.h"
#include "SimpleStringSplitter.h"

#include <xkill-utilities/Util.h>

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
	{
		std::string errorMsg = "Failed to open .mdldesc-file: " + getFileName();
		ERROR_MESSAGEBOX(errorMsg);
		sucessfulLoad = false;
	}

	if(sucessfulLoad)
		loadMdlDesc();
	
	return sucessfulLoad;
}

MdlDesc* LoaderMdlDesc::claimMdlDesc()
{
	mdlDesc_->setStatus(false);
	return mdlDesc_->getVar();
}

void LoaderMdlDesc::transferEventsToGame()
{
	Event_TransferEventsToGame e(events_);
	SEND_EVENT(&e);
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
		switch(indicator)
		{
		case MDLDESC_INDICATOR_HEADER:
			return MDLDESC_SYMBOL_HEADER;
		case MDLDESC_INDICATOR_MODEL:
			return MDLDESC_SYMBOL_MODEL;
		case MDLDESC_INDICATOR_WORLD:
			return MDLDESC_SYMBOL_WORLD;
		case MDLDESC_INDICATOR_SPAWN:
			return MDLDESC_SYMBOL_SPAWN;
		case MDLDESC_INDICATOR_HACK:
			return MDLDESC_SYMBOL_HACK;
		case MDLDESC_INDICATOR_AMMO:
			return MDLDESC_SYMBOL_AMMO;
		case MDLDESC_INDICATOR_LIGHT:
			return MDLDESC_SYMBOL_LIGHT;
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
	case MDLDESC_SYMBOL_WORLD:
		numExpectedParams = MDLDESC_PARAM_NUM_WORLD;
		break;
	case MDLDESC_SYMBOL_SPAWN:
		numExpectedParams = MDLDESC_PARAM_NUM_SPAWN;
		break;
	case MDLDESC_SYMBOL_HACK:
		numExpectedParams = MDLDESC_PARAM_NUM_HACK;
		break;
	case MDLDESC_SYMBOL_AMMO:
		numExpectedParams = MDLDESC_PARAM_NUM_AMMO;
		break;
	case MDLDESC_SYMBOL_LIGHT:
		numExpectedParams = MDLDESC_PARAM_NUM_LIGHT;
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
	case MDLDESC_SYMBOL_WORLD:
		loadWorld(params);
		break;
	case MDLDESC_SYMBOL_SPAWN:
		loadSpawn(params);
		break;
	case MDLDESC_SYMBOL_HACK:
		loadHack(params);
		break;
	case MDLDESC_SYMBOL_AMMO:
		loadAmmo(params);
		break;
	case MDLDESC_SYMBOL_LIGHT:
		loadLight(params);
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
	unsigned int	modelVertexType;

	modelID 		= static_cast<unsigned int>(::atof(params[MDLDESC_PARAM_INDEX_MODEL_ID].c_str()));
	modelName		= params[MDLDESC_PARAM_INDEX_MODEL_FILENAME].c_str();
	modelDynamic	= params[MDLDESC_PARAM_INDEX_MODEL_DYNAMIC].c_str();
	modelVertexType = static_cast<unsigned int>(::atof(params[MDLDESC_PARAM_INDEX_MODEL_VERTEX_TYPE].c_str()));

	models_.push_back(new MdlDescModel(
		modelID, 
		modelName, 
		modelDynamic != "0",
		modelVertexType));
}
void LoaderMdlDesc::loadWorld(const std::vector<std::string>& params)
{
	Float3 position;
	Float4 rotation;
	unsigned int meshID;

	position = Float3(static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_WORLD_POSX].c_str())),
					  static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_WORLD_POSY].c_str())),
					  static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_WORLD_POSZ].c_str())));
	rotation = Float4(static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_WORLD_ROTX].c_str())),
					  static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_WORLD_ROTY].c_str())),
					  static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_WORLD_ROTZ].c_str())),
					  static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_WORLD_ROTW].c_str())));
	meshID = atoi(params[MDLDESC_PARAM_INDEX_WORLD_ID].c_str());

	events_.push_back( new Event_CreateWorld(position,rotation,meshID));
}
void LoaderMdlDesc::loadSpawn(const std::vector<std::string>& params)
{
	Float3 position;
	float radius;

	position = Float3(static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_SPAWN_POSX].c_str())),
					  static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_SPAWN_POSY].c_str())),
					  static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_SPAWN_POSZ].c_str())));
	radius =  static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_SPAWN_RADIUS].c_str()));

	events_.push_back( new Event_CreatePlayerSpawnPoint(position,radius));
}
void LoaderMdlDesc::loadHack(const std::vector<std::string>& params)
{
	Float3 position;
	std::string type;

	position = Float3(static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_HACK_POSX].c_str())),
					  static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_HACK_POSY].c_str())),
					  static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_HACK_POSZ].c_str())));
	type = params[MDLDESC_PARAM_INDEX_HACK_TYPE].c_str();
	
	if( type == "HEL")
	{
		events_.push_back( new Event_CreatePickupablesSpawnPoint(position,XKILL_Enums::PickupableType::MEDKIT));
	}
	else if( type == "EXP")
	{
		events_.push_back( new Event_CreatePickupablesSpawnPoint(position,XKILL_Enums::PickupableType::AMMUNITION_EXPLOSIVE));
	}
	else if( type == "SHA")
	{
		events_.push_back( new Event_CreatePickupablesSpawnPoint(position,XKILL_Enums::PickupableType::AMMUNITION_SCATTER));
	}
	else if( type == "SIN")
	{
		events_.push_back( new Event_CreatePickupablesSpawnPoint(position,XKILL_Enums::PickupableType::AMMUNITION_BULLET));
	}
}
void LoaderMdlDesc::loadAmmo(const std::vector<std::string>& params)
{
	Float3 position;
	std::string type;

	position = Float3( static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_AMMO_POSX].c_str())),
					   static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_AMMO_POSY].c_str())),
					   static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_AMMO_POSZ].c_str())));
	type = params[MDLDESC_PARAM_INDEX_AMMO_TYPE].c_str();

	
}
void LoaderMdlDesc::loadLight(const std::vector<std::string>& params)
{
	Float3 position;
	Float3 direction;
	Float3 ambient;
	Float3 diffuse;
	Float3 specular;
	Float3 attenuation;
	float range;
	float spotPow;
	unsigned int type;
	
	position = Float3(	 static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_LIGHT_POSX].c_str())),
						 static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_LIGHT_POSY].c_str())),
						 static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_LIGHT_POSZ].c_str())));
	direction = Float3(	 static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_LIGHT_DIRX].c_str())),
						 static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_LIGHT_DIRY].c_str())),
						 static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_LIGHT_DIRZ].c_str())));
	ambient = Float3(	 static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_LIGHT_AMBR].c_str())),
						 static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_LIGHT_AMBG].c_str())),
						 static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_LIGHT_AMBB].c_str())));
	diffuse = Float3(	 static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_LIGHT_DIFR].c_str())),
						 static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_LIGHT_DIFG].c_str())),
						 static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_LIGHT_DIFB].c_str())));
	specular = Float3(	 static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_LIGHT_SPER].c_str())),
						 static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_LIGHT_SPEG].c_str())),
						 static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_LIGHT_SPEB].c_str())));
	attenuation = Float3(static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_LIGHT_ATTX].c_str())),
						 static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_LIGHT_ATTY].c_str())),
						 static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_LIGHT_ATTZ].c_str())));
	range =	  static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_LIGHT_RANGE].c_str()));
	spotPow = static_cast<float>(atof(params[MDLDESC_PARAM_INDEX_LIGHT_SPOTPOW].c_str()));
	type = atoi(params[MDLDESC_PARAM_INDEX_LIGHT_TYPE].c_str());

	events_.push_back( new Event_CreateLight(position,direction,ambient,diffuse,specular,attenuation,range,spotPow,type));
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