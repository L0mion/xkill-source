#ifndef XKILL_IO_LOADERMDLDESC_H
#define XKILL_IO_LOADERMDLDESC_H

#include <string>
#include <vector>

#include "VarStatus.h"
#include "Loader.h"
#include "MdlDescHeader.h"
#include "MdlDescModel.h"



class SimpleStringSplitter;
class MdlDesc;
class Event;

static const char MDLDESC_SEPARATOR_DEFAULT	= ' ';

static const char MDLDESC_INDICATOR_COMMENT	= '#';
static const char MDLDESC_INDICATOR_HEADER	= 'h';
static const char MDLDESC_INDICATOR_MODEL	= 'm';
static const char MDLDESC_INDICATOR_WORLD	= 'w';
static const char MDLDESC_INDICATOR_SPAWN	= 's';
static const char MDLDESC_INDICATOR_HACK	= 'c';
static const char MDLDESC_INDICATOR_AMMO	= 'a';
static const char MDLDESC_INDICATOR_LIGHT	= 'l';

static const unsigned int MDLDESC_PARAM_NUM_HEADER	= 3;
static const unsigned int MDLDESC_PARAM_NUM_MODEL	= 5;
static const unsigned int MDLDESC_PARAM_NUM_WORLD = 9;
static const unsigned int MDLDESC_PARAM_NUM_SPAWN = 5;
static const unsigned int MDLDESC_PARAM_NUM_HACK = 5;
static const unsigned int MDLDESC_PARAM_NUM_AMMO = 5;
static const unsigned int MDLDESC_PARAM_NUM_LIGHT = 22;


static const unsigned int MDLDESC_PARAM_INDEX_HEADER_NAME = 1;
static const unsigned int MDLDESC_PARAM_INDEX_HEADER_PATH = 2;

static const unsigned int MDLDESC_PARAM_INDEX_MODEL_ID			= 1;
static const unsigned int MDLDESC_PARAM_INDEX_MODEL_FILENAME	= 2;
static const unsigned int MDLDESC_PARAM_INDEX_MODEL_DYNAMIC		= 3;
static const unsigned int MDLDESC_PARAM_INDEX_MODEL_VERTEX_TYPE = 4;

static const unsigned int MDLDESC_PARAM_INDEX_WORLD_ID = 1;
static const unsigned int MDLDESC_PARAM_INDEX_WORLD_POSX = 2;
static const unsigned int MDLDESC_PARAM_INDEX_WORLD_POSY = 3;
static const unsigned int MDLDESC_PARAM_INDEX_WORLD_POSZ = 4;
static const unsigned int MDLDESC_PARAM_INDEX_WORLD_ROTX = 5;
static const unsigned int MDLDESC_PARAM_INDEX_WORLD_ROTY = 6;
static const unsigned int MDLDESC_PARAM_INDEX_WORLD_ROTZ = 7;
static const unsigned int MDLDESC_PARAM_INDEX_WORLD_ROTW = 8;

static const unsigned int MDLDESC_PARAM_INDEX_SPAWN_POSX = 1;
static const unsigned int MDLDESC_PARAM_INDEX_SPAWN_POSY = 2;
static const unsigned int MDLDESC_PARAM_INDEX_SPAWN_POSZ = 3;
static const unsigned int MDLDESC_PARAM_INDEX_SPAWN_RADIUS = 4;

static const unsigned int MDLDESC_PARAM_INDEX_HACK_TYPE = 1;
static const unsigned int MDLDESC_PARAM_INDEX_HACK_POSX = 2;
static const unsigned int MDLDESC_PARAM_INDEX_HACK_POSY = 3;
static const unsigned int MDLDESC_PARAM_INDEX_HACK_POSZ = 4;

static const unsigned int MDLDESC_PARAM_INDEX_AMMO_TYPE = 1;
static const unsigned int MDLDESC_PARAM_INDEX_AMMO_POSX = 2;
static const unsigned int MDLDESC_PARAM_INDEX_AMMO_POSY = 3;
static const unsigned int MDLDESC_PARAM_INDEX_AMMO_POSZ = 4;

static const unsigned int MDLDESC_PARAM_INDEX_LIGHT_TYPE	= 1;
static const unsigned int MDLDESC_PARAM_INDEX_LIGHT_AMBR	= 2;
static const unsigned int MDLDESC_PARAM_INDEX_LIGHT_AMBG	= 3;
static const unsigned int MDLDESC_PARAM_INDEX_LIGHT_AMBB	= 4;
static const unsigned int MDLDESC_PARAM_INDEX_LIGHT_DIFR	= 5;
static const unsigned int MDLDESC_PARAM_INDEX_LIGHT_DIFG	= 6;
static const unsigned int MDLDESC_PARAM_INDEX_LIGHT_DIFB	= 7;
static const unsigned int MDLDESC_PARAM_INDEX_LIGHT_SPER	= 8;
static const unsigned int MDLDESC_PARAM_INDEX_LIGHT_SPEG	= 9;
static const unsigned int MDLDESC_PARAM_INDEX_LIGHT_SPEB	= 10;
static const unsigned int MDLDESC_PARAM_INDEX_LIGHT_ATTX	= 11;
static const unsigned int MDLDESC_PARAM_INDEX_LIGHT_ATTY	= 12;
static const unsigned int MDLDESC_PARAM_INDEX_LIGHT_ATTZ	= 13;
static const unsigned int MDLDESC_PARAM_INDEX_LIGHT_POSX	= 14;
static const unsigned int MDLDESC_PARAM_INDEX_LIGHT_POSY	= 15;
static const unsigned int MDLDESC_PARAM_INDEX_LIGHT_POSZ	= 16;
static const unsigned int MDLDESC_PARAM_INDEX_LIGHT_DIRX	= 17;
static const unsigned int MDLDESC_PARAM_INDEX_LIGHT_DIRY	= 18;
static const unsigned int MDLDESC_PARAM_INDEX_LIGHT_DIRZ	= 19;
static const unsigned int MDLDESC_PARAM_INDEX_LIGHT_RANGE	= 20;
static const unsigned int MDLDESC_PARAM_INDEX_LIGHT_SPOTPOW = 21;

/** Used to specify symbols read from .mdldesc */
enum MdlDescSymbol
{
	MDLDESC_SYMBOL_HEADER,
	MDLDESC_SYMBOL_MODEL,
	MDLDESC_SYMBOL_WORLD,
	MDLDESC_SYMBOL_SPAWN,
	MDLDESC_SYMBOL_HACK,
	MDLDESC_SYMBOL_AMMO,
	MDLDESC_SYMBOL_LIGHT,

	MDLDESC_SYMBOL_IGNORE,
	MDLDESC_SYMBOL_INVALID
};

//! Used to load .mdldesc-files, which are used to establish lists of models to be loaded into engine.
/*!
\ingroup xkill-io
*/
class LoaderMdlDesc : public Loader
{
public:
	//! Passes standard values to Loader-object and initializes VarStatus-object to true.
	LoaderMdlDesc(
		std::string fileName,
		std::string filePath);
	//! Clears memory allocated by mdlDesc_ and sss_.
	~LoaderMdlDesc();

	//! Starts parsing of .mdldesc-file.
	bool init();

	//! Used to retrieve resulting MdlDesc-type from LoaderMdlDesc. Sets VarStatus to false, to indicate that memory management respnsibilities has been handed to whomever called the function.
	MdlDesc* claimMdlDesc();
	//! 
	void transferEventsToGame();

protected:
private:
	//! Retrieves lines from file, interprets what symbol corresponds to read indicator, parses parameters based on expected number of parameters and then proceeds to load corresponding symbol.
	bool parseMdlDesc();
	//! Selects a symbol based on indicator read from file.
	MdlDescSymbol parseSymbol(const std::vector<std::string>& params);
	//! Checks if number of parameters corresponds with said symbol.
	bool parseParams(
		const MdlDescSymbol symbol,
		const std::vector<std::string>& params);
	//! Based on symbol, function calls corresponding loading-function.
	bool loadSymbol(
		const MdlDescSymbol symbol,
		const std::vector<std::string>& params);
	
	//! Loads a .mdldesc-header from file.
	void loadHeader(const std::vector<std::string>& params);
	//! Loads a .mdldesc-texture from file.
	void loadModel(const std::vector<std::string>& params);
	//! Loads a .mdldesc-world from file.
	void loadWorld(const std::vector<std::string>& params);
	//! Loads a .mdldesc-spawn from file.
	void loadSpawn(const std::vector<std::string>& params);
	//! Loads a .mdldesc-hack from file.
	void loadHack(const std::vector<std::string>& params);
	//! Loads a .mdldesc-ammo from file.
	void loadAmmo(const std::vector<std::string>& params);
	//! Loads a .mdldesc-light from file.
	void loadLight(const std::vector<std::string>& params);



	//! Loads intermediate data into resulting MdlDesc-type.
	void loadMdlDesc();

	/*helper*/
	SimpleStringSplitter* sss_;				//!< Used to split lines read from file.

	/*intermediate*/
	MdlDescHeader				header_;	//!< Header read from .mdldesc.
	std::vector<MdlDescModel*>	models_;	//!< Collection of texture descriptions read from .mdldesc.
	std::vector<Event*>			events_;	//!< Collection of events needed to load set level objects

	/*result*/
	VarStatus<MdlDesc>* mdlDesc_;			//!< Resulting MdlDesc-type. Wrapped in VarStatus-class to indicate where memory management-responsibilities lie.
};

#endif //XKILL_IO_LOADERMDLDESC_H