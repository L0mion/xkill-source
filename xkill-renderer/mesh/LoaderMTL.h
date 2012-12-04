#ifndef XKILL_RENDERER_MESHLOADERMTL_H
#define XKILL_RENDERER_MESHLOADERMTL_H

#include <Windows.h>

#include "Loader.h"
#include "renderingUtilities.h"
#include "MTLIllum.h"
#include "SimpleStringSplitter.h"
#include "MTL.h"

namespace DirectX
{
	struct XMFLOAT3;
}

class MTLMaterial;

static const char MTL_SEPARATOR_DEFAULT = ' ';

static const std::string MTL_INDICATOR_NEWMTL			= "newmtl"; //newmtl material123
static const std::string MTL_INDICATOR_ILLUM			= "illum";	//illum 0-10
static const std::string MTL_INDICATOR_COLOR_DIFFUSE	= "Kd";		//Kd 1.0 2.0 3.0
static const std::string MTL_INDICATOR_COLOR_AMBIENT	= "Ka";		//Ka 1.0 2.0 3.0
static const std::string MTL_INDICATOR_COLOR_SPECULAR	= "Ks";		//Ks 1.0 2.0 3.0
static const std::string MTL_INDICATOR_COLOR_TF			= "Tf";		//Tf 1.0 2.0 3.0
static const std::string MTL_INDICATOR_TEX_DIFFUSE		= "map_Kd";	//map_Kd diffuseTex.tga
static const std::string MTL_INDICATOR_TEX_AMBIENT		= "map_Ka";	//map_Ka ambientTex.tga
static const std::string MTL_INDICATOR_TEX_SPECULAR		= "map_Ks";	//map_Ks specularTex.tga
static const std::string MTL_INDICATOR_TEX_SPECPOW		= "map_Ns";	//map_Kd specPowtex.tga
static const std::string MTL_INDICATOR_TEX_ALPHA		= "map_d";	//map_d alphaTex.tga
static const std::string MTL_INDICATOR_OPTICAL			= "Ni";		//Ni 1.0

static const unsigned int MTL_PARAMS					= 1;
static const unsigned int MTL_PARAMS_NUM_NEWMTL			= 1 + MTL_PARAMS;
static const unsigned int MTL_PARAMS_NUM_ILLUM			= 1 + MTL_PARAMS;
static const unsigned int MTL_PARAMS_NUM_COLOR			= 3 + MTL_PARAMS;
static const unsigned int MTL_PARAMS_NUM_TEX			= 1 + MTL_PARAMS;
static const unsigned int MTL_PARAMS_NUM_OPTICAL		= 1 + MTL_PARAMS;

static const unsigned int MTL_PARAMS_INDEX_NEWMTL_NAME	= 0 + MTL_PARAMS;
static const unsigned int MTL_PARAMS_INDEX_ILLUM_ID		= 0 + MTL_PARAMS;

static const unsigned int MTL_PARAMS_INDEX_COLOR_X	= 0 + MTL_PARAMS;
static const unsigned int MTL_PARAMS_INDEX_COLOR_Y	= 1 + MTL_PARAMS;
static const unsigned int MTL_PARAMS_INDEX_COLOR_Z	= 2 + MTL_PARAMS;

static const unsigned int MTL_PARAMS_INDEX_TEX_FILENAME = 0 + MTL_PARAMS;

static const unsigned int MTL_PARAMS_INDEX_OPTICAL_DENSITY = 0 + MTL_PARAMS;

 /** Describes read symbol so that pretty switch-cases may be used.
* An action is based of this Enum, and then proceeds to select an appropriate function.
*/
enum MTLSymbol
{
	MTLSYMBOL_NEWMTL,			/**< newmtl */
	MTLSYMBOL_ILLUM,			/**< illum */
	MTLSYMBOL_COLOR_DIFFUSE,	/**< Kd */
	MTLSYMBOL_COLOR_AMBIENT,	/**< Ka */
	MTLSYMBOL_COLOR_SPECULAR,	/**< Ks */
	MTLSYMBOL_COLOR_TF,			/**< Tf */

	MTLSYMBOL_TEX_DIFFUSE,		/**< map_Kd */
	MTLSYMBOL_TEX_AMBIENT,		/**< map_Ka */
	MTLSYMBOL_TEX_SPECULAR,		/**< map_Ks */
	MTLSYMBOL_TEX_SPECPOW,		/**< map_Ns */
	MTLSYMBOL_TEX_ALPHA,		/**< map_d */

	MTLSYMBOL_OPTICAL,			/**< Ni */

	MTLSYMBOL_IGNORE,			/**< ...if not any of the above. */
	MTLSYMBOL_INVALID			/**< This value should never, ever - except occationally - occur. (No, seriously, never.) */
};

//! MeshLoader with functionality to load .mtl-files.
/*!
\ingroup xkill-mesh-io-mtl
*/
class LoaderMTL : public Loader
{
public:
	//! Forwards path to .mtl to parent Loader-class.
	/*!
	\param pathMTL Path up to desired .mtl-file.
	\param fileNameMTL Name of desired .mtl-file.
	*/
	LoaderMTL(
		const std::string pathMTL,
		const std::string fileNameMTL);
	//! Does nothing.
	~LoaderMTL();

	//! Function initializing object and loading specified .mtl-file.
	/*!
	Method attempts to open specified .mtl-file and proceeds to parse this file if suceeded. If suceeded, the method will then call loading of this .obj.
	\sa parseMTL
	\sa loadMTL
	\return A boolean dictating whether or not the method was sucessful.
	*/
	bool init();

	MTL getMTL();
protected:
private:
	//! Loops through read symbols, parses these, parses parameters, parses prerequisites of symbol and then proceeds to load appropriate symbol.
	/*!
	\sa parseSymbol
	\sa parseParams
	\sa parsePrerequisites
	\sa loadSymbol
	\return A boolean dictating whether or not the method was sucessful.
	*/
	bool parseMTL();

	//! Translates first read string into MtlSymbol-object.
	/*!
	\return Symbol read from .mtl dictating next move of loader.
	*/
	MTLSymbol parseSymbol(std::vector<std::string>& params);
	//! Checks number of expected parameters based on symbol and ensures that these are numeric if so expected.
	/*!
	\sa parseParamsNumeric
	\param symbol Symbol read from .mtl.
	\param params Other parameters read from .mtl.
	\return A boolean dictating whether or not the method was sucessful.
	*/
	bool parseParams(
		MTLSymbol symbol, 
		std::vector<std::string>& params);
	//! Checks if all elements in vector are numeric.
	/*!
	\sa isNumeric
	\param params Parameters to be checked if numeric.
	\return A boolean dictating whether or not the method was sucessful.
	*/
	bool parseParamsNumeric(const std::vector<std::string>& params);
	//! Checks prerequisites if such are specified.
	/*!
	For example; most elements read from .mtl require there to have been an existing newmtl-command to specify an imminent new material.
	\param symbol Symbol read from .mtl.
	\param params Other parameters read from .mtl.
	\return A boolean dictating whether or not the method was sucessful.
	*/
	bool parsePrerequisites(
		MTLSymbol symbol, 
		const std::vector<std::string>& params);
	//! Based on symbol, this method calls the corresponding function to load that symbol.
	/*!
	\param symbol Symbol read from .mtl.
	\param params Other parameters read from .mtl.
	*/
	void loadSymbol(
		const MTLSymbol symbol, 
		const std::vector<std::string>& params);
	//! Loads a new material from .mtl and pushes this into materials_-vector.
	/*!
	\param params Other parameters read from .mtl.
	*/
	void loadNewMTL(const std::vector<std::string>& params);
	//! Pushes a new illumination model to previously added material.
	/*!
	\sa MTLIllum
	\param params Other parameters read from .mtl.
	*/
	void loadIllum(const std::vector<std::string>& params);
	//! Loads a color from .mtl into correct slot of previously added material.
	/*!
	E.g. diffuse color if a such color is read.
	\param symbol Symbol read from .mtl.
	\param params Other parameters read from .mtl.
	*/
	void loadColor(
		const MTLSymbol symbol,
		const std::vector<std::string>& params);
	//! Loads a vector from .mtl.
	/*!
	\param params Other parameters read from .mtl.
	\return Loaded vector.
	\sa loadVector
	*/
	DirectX::XMFLOAT3 loadVector(const std::vector<std::string>& params);
	//! Loads a texture identifier from .mtl into correct slot of previously added material.
	/*!
	E.g. diffuse texture if such a texture is read.
	\param symbol Symbol read from .mtl.
	\param params Other parameters read from .mtl.
	\sa loadTex
	*/
	void loadTex(
		const MTLSymbol symbol,
		const std::vector<std::string>& params);
	//! Reads a single string from .mtl.
	/*!
	\param params Other parameters read from .mtl.
	\return Read string.
	*/
	std::string loadFilename(const std::vector<std::string>& params);
	//! Reads optical density from .mtl.
	/*!
	\param params Parameters read from .mtl.
	*/
	void loadOptical(const std::vector<std::string>& params);

	//! Creates resulting MTL-type based on materials read from .mtl.
	void loadMTL();

	//! Reads a line from file. Also increments a linenumber-counter.
	void getLine(std::string& line);
	//! Used to print error if such have occured.
	void printFail(const std::string curLine);
	//! Checks if a string is numeric.
	bool isNumeric(const std::string value);

	/*helper*/
	unsigned int lineNum_;					//!< Line-number previously read from .mtl.
	SimpleStringSplitter sss_;				//!< Helper-object to aid in splitting lines read from .mtl.

	/*intermediate*/
	std::vector<MTLMaterial> materials_;	//!< Materials read from .mtl.

	/*result*/
	MTL mtl_;								//!< Resulting MTL-type.
};

#endif //XKILL_RENDERER_MESHLOADERMTL_H