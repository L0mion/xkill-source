#ifndef XKILL_IO_LOADERTEXDESC_H
#define XKILL_IO_LOADERTEXDESC_H

#include <xkill-utilities/TexDescHeader.h>
#include <xkill-utilities/TexDescTex.h>
#include <xkill-utilities/TexDesc.h>

#include "VarStatus.h"
#include "Loader.h"

class SimpleStringSplitter;

static const char TEXDESC_SEPARATOR_DEFAULT	= ' ';

static const char TEXDESC_INDICATOR_COMMENT	= '#';
static const char TEXDESC_INDICATOR_HEADER	= 'h';
static const char TEXDESC_INDICATOR_TEXTURE	= 't';

static const unsigned int TEXDESC_PARAM_NUM_HEADER	= 3;
static const unsigned int TEXDESC_PARAM_NUM_TEXTURE	= 3;

static const unsigned int TEXDESC_PARAM_INDEX_HEADER_NAME = 1;
static const unsigned int TEXDESC_PARAM_INDEX_HEADER_PATH = 2;

static const unsigned int TEXDESC_PARAM_INDEX_TEXTURE_ID		= 1;
static const unsigned int TEXDESC_PARAM_INDEX_TEXTURE_FILENAME	= 2;

/** Describes symbols read from file. */
enum TexDescSymbol
{
	TEXDESC_SYMBOL_HEADER,
	TEXDESC_SYMBOL_TEX,

	TEXDESC_SYMBOL_IGNORE,
	TEXDESC_SYMBOL_INVALID
};

//! Used to load .texdesc-files, which are used to establish a list of textures which will then be loaded.
class LoaderTexDesc : public Loader
{
public:
	//! Initializes VarStatus-type to true and passes arguments to Loader-type.
	LoaderTexDesc(
		std::string fileName,
		std::string filePath);
	//! Clears memory allocated by SimpleStringSplitter and VarStatus-object. Observe that memory allocated by VarStatus will only be cleared if VarStatus == true.
	~LoaderTexDesc();

	//! Begins loading of .texdesc-format. Will return false if method failed, and true otherwize.
	bool init();

	//! Sets TexDesc to false and returns var hold by TexDesc VarStatus-object.
	TexDesc* claimTexDesc();
protected:
private:
	//! Parses the .texdesc-format. Reads a line from file, splits said line, interprets first symbol, parses parameters based on symbol and loads into corresponding data-type. 
	bool parseTexDesc();
	//! Based on indicator, returns a symbol specifying correct action to be takne.
	TexDescSymbol parseSymbol(const std::vector<std::string>& params);
	//! Checks whether or not number of parameters read is corresponding to symbol.
	bool parseParams(
		const TexDescSymbol symbol,
		const std::vector<std::string>& params);
	//! Based on symbol; correct function is called.
	bool loadSymbol(
		const TexDescSymbol symbol,
		const std::vector<std::string>& params);
	
	//! Loads a TexDescHeader-object from file.
	void loadHeader(const std::vector<std::string>& params);
	//! Loads a TexDescTex-object from file.
	void loadTex(const std::vector<std::string>& params);

	//! Loads intermediate data into resulting TexDesc-format.
	void loadTexDesc();

	/*helper*/
	SimpleStringSplitter* sss_;			//!< 

	/*intermediate*/
	TexDescHeader			header_;	//!< 
	std::vector<TexDescTex>	texs_;		//!< 

	/*result*/
	VarStatus<TexDesc>* texDesc_;		//!< 
};

#endif //XKILL_IO_LOADERTEXDESC_H