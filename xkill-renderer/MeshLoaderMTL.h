#ifndef XKILL_RENDERER_MESHLOADERMTL_H
#define XKILL_RENDERER_MESHLOADERMTL_H

#include <Windows.h>

#include "MeshLoader.h"
#include "renderingUtilities.h"
#include "MeshMTLIllum.h"
#include "SimpleStringSplitter.h"
#include "MTL.h"

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

enum MtlSymbol
{
	MTLSYMBOL_NEWMTL,
	MTLSYMBOL_ILLUM,
	MTLSYMBOL_COLOR_DIFFUSE,
	MTLSYMBOL_COLOR_AMBIENT,
	MTLSYMBOL_COLOR_SPECULAR,
	MTLSYMBOL_COLOR_TF,

	MTLSYMBOL_TEX_DIFFUSE,
	MTLSYMBOL_TEX_AMBIENT,
	MTLSYMBOL_TEX_SPECULAR,
	MTLSYMBOL_TEX_SPECPOW,
	MTLSYMBOL_TEX_ALPHA,

	MTLSYMBOL_OPTICAL,

	MTLSYMBOL_IGNORE,
	MTLSYMBOL_INVALID
};

class MeshLoaderMTL : public MeshLoader
{
public:
	MeshLoaderMTL(LPCWSTR mtlPath);
	~MeshLoaderMTL();

	bool init();

	MTL getMTL();
protected:
private:
	bool parseMTL();

	MtlSymbol parseSymbol(std::vector<std::string>& params);
	bool parseParams(
		MtlSymbol symbol, 
		std::vector<std::string>& params);
	bool parseParamsNumeric(const std::vector<std::string>& params);
	bool parsePrerequisites(
		MtlSymbol symbol, 
		const std::vector<std::string>& params);

	void loadSymbol(
		const MtlSymbol symbol, 
		const std::vector<std::string>& params);
	void loadNewMTL(const std::vector<std::string>& params);
	void loadIllum(const std::vector<std::string>& params);
	void loadColor(
		const MtlSymbol symbol,
		const std::vector<std::string>& params);
	DirectX::XMFLOAT3 loadVector(const std::vector<std::string>& params);
	void loadTex(
		const MtlSymbol symbol,
		const std::vector<std::string>& params);
	std::string loadFilename(const std::vector<std::string>& params);
	void loadOptical(const std::vector<std::string>& params);

	void loadMTL();

	void getLine(std::string& line);
	void printFail();
	bool isNumeric(std::string value);

	/*helper*/
	unsigned int lineNum_;
	SimpleStringSplitter sss_;

	/*intermediate*/
	std::vector<MTLMaterial> materials_;

	/*result*/
	MTL mtl_;
};

#endif //XKILL_RENDERER_MESHLOADERMTL_H