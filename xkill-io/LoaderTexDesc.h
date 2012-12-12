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
	LoaderTexDesc(
		std::string fileName,
		std::string filePath);
	~LoaderTexDesc();

	bool init();

	TexDesc* claimTexDesc();
protected:
private:
	bool parseTexDesc();
	TexDescSymbol parseSymbol(const std::vector<std::string>& params);
	bool parseParams(
		const TexDescSymbol symbol,
		const std::vector<std::string>& params);
	bool loadSymbol(
		const TexDescSymbol symbol,
		const std::vector<std::string>& params);
	
	void loadHeader(const std::vector<std::string>& params);
	void loadTex(const std::vector<std::string>& params);

	void loadTexDesc();

	/*helper*/
	SimpleStringSplitter* sss_;

	/*intermediate*/
	TexDescHeader			header_;
	std::vector<TexDescTex>	texs_;

	/*result*/
	VarStatus<TexDesc>* texDesc_;
};

#endif //XKILL_IO_LOADERTEXDESC_H