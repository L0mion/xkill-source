#include <MeshLoaderObj.h>

MeshLoaderObj::MeshLoaderObj(LPCWSTR mlFileName)
	: MeshLoader(mlFileName)
{
}
MeshLoaderObj::~MeshLoaderObj()
{
}

bool MeshLoaderObj::init()
{
	bool sucessfulLoad = true;

	LPCWSTR filename = getMLFileName();
	mlIFS_.open(filename);

	if(mlIFS_.is_open())
	{
		sucessfulLoad = mlLoadSymbols();
		if(mlIFS_.is_open())
			mlIFS_.close();
	}
	else
		sucessfulLoad = false;

	return sucessfulLoad;
}

bool MeshLoaderObj::mlLoadMesh()
{
	return mlLoadSymbols();
}
bool MeshLoaderObj::mlLoadSymbols()
{
	bool sucessfulLoad = true;

	std::string		symbol; 
	ObjLoadAction	objLoadAction;

	while(!mlIFS_.eof())
	{
		mlIFS_ >> symbol;
		objLoadAction = mlSymbolToAction(symbol);
		
		switch(objLoadAction)
		{
		case LOAD_VERTEX:
			sucessfulLoad = mlLoadVertex();
			break;
		case LOAD_TEXTURE_COORD:
			sucessfulLoad = mlLoadTexCoord();
			break;
		case LOAD_NORMAL:
			sucessfulLoad = mlLoadNormal();
			break;
		case LOAD_FACE:
			sucessfulLoad = mlLoadFace();
		case LOAD_NOTHING:
			//Do nothing.
			break;
		}
	}

	return sucessfulLoad;
}
bool MeshLoaderObj::mlLoadVertex()
{
	float x, y, z;
	mlIFS_ >> x >> y >> z;
	DirectX::XMFLOAT3 mlPosition(x, y, z);
	mlPosition_.push_back(mlPosition);

	return true; //temp
}
bool MeshLoaderObj::mlLoadNormal()
{
	float x, y, z;
	mlIFS_ >> x >> y >> z;
	DirectX::XMFLOAT3 mlNormal(x, y, z);
	mlNormal_.push_back(mlNormal);

	return true; //temp
}
bool MeshLoaderObj::mlLoadTexCoord()
{
	float x, y;
	mlIFS_ >> x >> y;
	DirectX::XMFLOAT2 mlTexCoord(x, y);
	mlTexCoord_.push_back(mlTexCoord);

	return true; //temp
}
bool MeshLoaderObj::mlLoadFace()
{
	return true; //temp
}
bool MeshLoaderObj::mlLoadMaterial()
{
	return true; //temp
}

const ObjLoadAction MeshLoaderObj::mlSymbolToAction(const std::string symbol)
{
	ObjLoadAction action = LOAD_NOTHING;

	if(symbol == OBJ_INDICATOR_VERTEX)
		action = LOAD_VERTEX;
	else if(symbol == OBJ_INDICATOR_TEX)
		action = LOAD_TEXTURE_COORD;
	else if(symbol == OBJ_INDICATOR_NORM)
		action = LOAD_NORMAL;
	else if(symbol == OBJ_INDICATOR_FACE)
		action = LOAD_FACE;

	/*
	else if(symbol == OBJ_INDICATOR_SMOOTH)
	else if(symbol == OBJ_INDICATOR_GROUP)
	else if(symbol == OBJ_INDICATOR_LIB)
	*/
	
	return action;
}