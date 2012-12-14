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

static const char MDLDESC_SEPARATOR_DEFAULT	= ' ';

static const char MDLDESC_INDICATOR_COMMENT	= '#';
static const char MDLDESC_INDICATOR_HEADER	= 'h';
static const char MDLDESC_INDICATOR_MODEL	= 'm';

static const unsigned int MDLDESC_PARAM_NUM_HEADER	= 3;
static const unsigned int MDLDESC_PARAM_NUM_MODEL	= 4;

static const unsigned int MDLDESC_PARAM_INDEX_HEADER_NAME = 1;
static const unsigned int MDLDESC_PARAM_INDEX_HEADER_PATH = 2;

static const unsigned int MDLDESC_PARAM_INDEX_MODEL_ID			= 1;
static const unsigned int MDLDESC_PARAM_INDEX_MODEL_FILENAME	= 2;
static const unsigned int MDLDESC_PARAM_INDEX_MODEL_DYNAMIC		= 3;

/** Used to specify symbols read from .mdldesc */
enum MdlDescSymbol
{
	MDLDESC_SYMBOL_HEADER,
	MDLDESC_SYMBOL_MODEL,

	MDLDESC_SYMBOL_IGNORE,
	MDLDESC_SYMBOL_INVALID
};

//! Used to load .mdldesc-files, which are used to establish lists of models to be loaded into engine.
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

	//! Loads intermediate data into resulting MdlDesc-type.
	void loadMdlDesc();

	/*helper*/
	SimpleStringSplitter* sss_;				//!< Used to split lines read from file.

	/*intermediate*/
	MdlDescHeader				header_;	//!< Header read from .mdldesc.
	std::vector<MdlDescModel*>	models_;	//!< Collection of texture descriptions read from .mdldesc.

	/*result*/
	VarStatus<MdlDesc>* mdlDesc_;			//!< Resulting MdlDesc-type. Wrapped in VarStatus-class to indicate where memory management-responsibilities lie.
};

#endif //XKILL_IO_LOADERMDLDESC_H