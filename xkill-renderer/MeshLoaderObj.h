#ifndef XKILL_RENDERER_MESHLOADEROBJ_H
#define XKILL_RENDERER_MESHLOADEROBJ_H

#include <vector>
#include <string>
#include <DirectXMath.h>

#include <MeshLoader.h>

static const std::string OBJ_INDICATOR_LIB		= "mtllib";
static const std::string OBJ_INDICATOR_GROUP	= "g";
static const std::string OBJ_INDICATOR_VERTEX	= "v";
static const std::string OBJ_INDICATOR_TEX		= "vt";
static const std::string OBJ_INDICATOR_NORM		= "vn";
static const std::string OBJ_INDICATOR_FACE		= "f";
static const std::string OBJ_INDICATOR_SMOOTH	= "s";

enum ObjLoadAction { LOAD_VERTEX, LOAD_TEXTURE_COORD, LOAD_NORMAL, LOAD_FACE, LOAD_NOTHING };

class MeshLoaderObj : public MeshLoader
{
public:
	MeshLoaderObj(LPCWSTR mlFileName);
	~MeshLoaderObj();

	bool init();
protected:
	bool mlLoadMesh();
private:
	bool mlLoadSymbols();
	bool mlLoadVertex();
	bool mlLoadNormal();
	bool mlLoadTexCoord();
	bool mlLoadFace();
	bool mlLoadMaterial();

	const ObjLoadAction mlSymbolToAction(const std::string symbol);

	std::vector<DirectX::XMFLOAT3> mlPosition_;
	std::vector<DirectX::XMFLOAT3> mlNormal_;
	std::vector<DirectX::XMFLOAT2> mlTexCoord_;
};

#endif //XKILL_RENDERER_MESHLOADEROBJ_H