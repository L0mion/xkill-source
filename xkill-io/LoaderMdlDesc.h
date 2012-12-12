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

enum MdlDescSymbol
{
	MDLDESC_SYMBOL_HEADER,
	MDLDESC_SYMBOL_MODEL,

	MDLDESC_SYMBOL_IGNORE,
	MDLDESC_SYMBOL_INVALID
};

class LoaderMdlDesc : public Loader
{
public:
	LoaderMdlDesc(
		std::string fileName,
		std::string filePath);
	~LoaderMdlDesc();

	bool init();

	MdlDesc* claimMdlDesc();
protected:
private:
	bool parseMdlDesc();
	MdlDescSymbol parseSymbol(const std::vector<std::string>& params);
	bool parseParams(
		const MdlDescSymbol symbol,
		const std::vector<std::string>& params);
	bool loadSymbol(
		const MdlDescSymbol symbol,
		const std::vector<std::string>& params);
	
	void loadHeader(const std::vector<std::string>& params);
	void loadModel(const std::vector<std::string>& params);

	void loadTexDesc();

	/*helper*/
	SimpleStringSplitter* sss_;

	/*intermediate*/
	MdlDescHeader				header_;
	std::vector<MdlDescModel*>	models_;

	/*result*/
	VarStatus<MdlDesc>* mdlDesc_;
};

#endif //XKILL_IO_LOADERMDLDESC_H