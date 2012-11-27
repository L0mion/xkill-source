#ifndef XKILL_RENDERER_MESHLOADEROBJ_H
#define XKILL_RENDERER_MESHLOADEROBJ_H

#include <vector>
#include <string>
#include <DirectXMath.h>

#include "MeshLoader.h"
#include "MeshFace.h"
#include "SimpleStringSplitter.h"
#include "vertices.h"

#include "renderingUtilities.h"

static const std::string OBJ_INDICATOR_LIB		= "mtllib";
static const std::string OBJ_INDICATOR_GROUP	= "g";
static const std::string OBJ_INDICATOR_VERTEX	= "v";
static const std::string OBJ_INDICATOR_TEX		= "vt";
static const std::string OBJ_INDICATOR_NORM		= "vn";
static const std::string OBJ_INDICATOR_FACE		= "f";
static const std::string OBJ_INDICATOR_SMOOTH	= "s";

static const char OBJ_SEPARATOR_DEFAULT	= ' ';
static const char OBJ_SEPARATOR_FACE	= '/';

static const unsigned int OBJ_PARAMS = 1;
static const unsigned int OBJ_PARAMS_NUM_VERTEX				= 3 + OBJ_PARAMS;
static const unsigned int OBJ_PARAMS_NUM_VERTEX_OPTIONAL	= 4 + OBJ_PARAMS;
static const unsigned int OBJ_PARAMS_NUM_TEX				= 2 + OBJ_PARAMS;
static const unsigned int OBJ_PARAMS_NUM_TEX_OPTIONAL		= 3 + OBJ_PARAMS;
static const unsigned int OBJ_PARAMS_NUM_NORM				= 3 + OBJ_PARAMS;
static const unsigned int OBJ_PARAMS_NUM_FACE				= 3 + OBJ_PARAMS;
static const unsigned int OBJ_PARAMS_NUM_FACE_V				= 3;

static const unsigned int OBJ_PARAMS_INDEX_VERTEX_X = 0 + OBJ_PARAMS;
static const unsigned int OBJ_PARAMS_INDEX_VERTEX_Y = 1 + OBJ_PARAMS;
static const unsigned int OBJ_PARAMS_INDEX_VERTEX_Z = 2 + OBJ_PARAMS;
static const unsigned int OBJ_PARAMS_INDEX_VERTEX_W = 3 + OBJ_PARAMS;

static const unsigned int OBJ_PARAMS_INDEX_TEX_U = 0 + OBJ_PARAMS;
static const unsigned int OBJ_PARAMS_INDEX_TEX_V = 1 + OBJ_PARAMS;
static const unsigned int OBJ_PARAMS_INDEX_TEX_W = 2 + OBJ_PARAMS;

static const unsigned int OBJ_PARAMS_INDEX_FACE_X = 0;
static const unsigned int OBJ_PARAMS_INDEX_FACE_Y = 1;
static const unsigned int OBJ_PARAMS_INDEX_FACE_Z = 2;

enum ObjSymbol { SYMBOL_VERTEX, SYMBOL_TEX, SYMBOL_NORM, SYMBOL_FACE, SYMBOL_IGNORE };

class MeshLoaderObj : public MeshLoader
{
public:
	MeshLoaderObj(LPCWSTR mlFileName);
	~MeshLoaderObj();

	bool init();

	std::vector<VertexPosNormTex>	getMLVertices();
	std::vector<unsigned int>		getMLIndices();
protected:
	bool mlLoadMesh();
private:
	bool mlParseSymbols();
	bool mlParseSymbol();
	bool mlParseParams();
	bool mlParseParam(ObjSymbol symbol);

	bool mlLoadVertex();
	bool mlLoadNorm();
	bool mlLoadTex();
	bool mlLoadFaces();
	void mlLoadFace(std::vector<std::string>& face);
	bool mlParseFace(std::vector<std::string>& splitFaces);

	void mlGetLine(std::string& line);
	void mlPrintFail();
	bool mlIsNumeric(std::string value);

	unsigned int lineNum_;

	std::string					curLine_;
	SimpleStringSplitter		sss_;
	std::vector<std::string>	curLineSplit_;
	ObjSymbol					curSymbol_;

	std::vector<DirectX::XMFLOAT3> mlPosition_;
	std::vector<DirectX::XMFLOAT3> mlNormal_;
	std::vector<DirectX::XMFLOAT2> mlTexCoord_;

	std::vector<MeshFace> mlFaces_;

	std::vector<VertexPosNormTex>	mlVertices_;
	std::vector<unsigned int>		mlIndices_;
};

#endif //XKILL_RENDERER_MESHLOADEROBJ_H