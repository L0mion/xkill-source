#ifndef XKILL_RENDERER_MESHLOADEROBJ_H
#define XKILL_RENDERER_MESHLOADEROBJ_H

#include <vector>
#include <string>
#include <DirectXMath.h>

#include "MeshLoader.h"
#include "MeshFace.h"
#include "MeshGroup.h"
#include "SimpleStringSplitter.h"
#include "vertices.h"

#include "renderingUtilities.h"

static const std::string OBJ_INDICATOR_VERTEX	= "v";
static const std::string OBJ_INDICATOR_TEX		= "vt";
static const std::string OBJ_INDICATOR_NORM		= "vn";
static const std::string OBJ_INDICATOR_FACE		= "f";
static const std::string OBJ_INDICATOR_GROUP	= "g";

//static const std::string OBJ_INDICATOR_SMOOTH	= "s";
//static const std::string OBJ_INDICATOR_LIB	= "mtllib";

static const char OBJ_SEPARATOR_DEFAULT	= ' ';
static const char OBJ_SEPARATOR_FACE	= '/';

static const unsigned int OBJ_PARAMS = 1;
static const unsigned int OBJ_PARAMS_NUM_VERTEX				= 3 + OBJ_PARAMS;	//v x y z
static const unsigned int OBJ_PARAMS_NUM_VERTEX_OPTIONAL	= 4 + OBJ_PARAMS;	//v x y z w
static const unsigned int OBJ_PARAMS_NUM_TEX				= 2 + OBJ_PARAMS;	//vt u v
static const unsigned int OBJ_PARAMS_NUM_TEX_OPTIONAL		= 3 + OBJ_PARAMS;	//vt u v w
static const unsigned int OBJ_PARAMS_NUM_NORM				= 3 + OBJ_PARAMS;	//vn x y z
static const unsigned int OBJ_PARAMS_NUM_FACES				= 3 + OBJ_PARAMS;	//f v/vt/vn v/vt/vn v/vt/vn
static const unsigned int OBJ_PARAMS_NUM_FACE				= 3;				//v vt vn
static const unsigned int OBJ_PARAMS_NUM_GROUP				= 1 + OBJ_PARAMS;	//g groupname

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

static const unsigned int OBJ_PARAMS_INDEX_GROUP_NAME = 0 + OBJ_PARAMS;

enum ObjSymbol { SYMBOL_VERTEX, SYMBOL_TEX, SYMBOL_NORM, SYMBOL_FACE, SYMBOL_GROUP, SYMBOL_IGNORE, SYMBOL_INVALID };

//! MeshLoader with functionality to load .obj-files.
/*!
Derived from basic MeshLoader-class.
\ingroup xkill-renderer-mesh
\sa MeshLoader
*/
class MeshLoaderObj : public MeshLoader
{
public:
	//! Forwards path to .obj to parent MeshLoader-class.
	/*!
	\param mlFilePath Path to desired .obj-file.
	*/
	MeshLoaderObj(const LPCWSTR mlFilePath);
	//! Clears all vectors.
	~MeshLoaderObj();

	//! Function initializing MeshLoaderObj and loading specified .obj-file.
	/*!
	\return A boolean dictating whether or not the method was sucessful.
	*/
	bool init();

	std::vector<VertexPosNormTex>	getMLVertices();
	std::vector<unsigned int>		getMLIndices();
protected:
private:
	//! Method loading .obj-file.
	/*!
		Reads line from file, interprets symbol, parses parameters and calls correct load-function.
		\return A boolean dictating whether or not the method was sucessful.
		\sa mlParseSymbol
		\sa mlParseParams
		\sa mlLoadVertex
		\sa mlLoadTex
		\sa mlLoadNorm
		\sa mlLoadFaces
	*/
	bool mlParseObj();
	//! Interprets symbol of first string recieved from splitting line.
	/*!
	Stores correct symbol in member variable curSymbol_.
	\return A boolean dictating whether or not the method was sucessful.
	\sa curSymbol_
	*/
	ObjSymbol mlParseSymbol(const std::vector<std::string>& params);
	//! Based on symbol, this method calls a mlParseParam.
	/*!
		Method does not call a parse-method for faces, as this is done later during the loading process.
		\return A boolean dictating whether or not the method was sucessful.
		\sa mlParseParam
	*/
	bool mlParseParams(
		const ObjSymbol symbol,
		const std::vector<std::string>& params);
	//! Checks if all parameters for a certain symbol are numeric.
	/*!
		\return A boolean dictating whether or not parameters contain only numeric values.
		\sa mlParseParam
		\sa mlIsNumeric
	*/
	bool mlParseParamsNumeric(const std::vector<std::string>& params);

	void mlLoadSymbol(
		const ObjSymbol symbol,
		const std::vector<std::string>& params);

	//! Loads parameters previously parsed as being entirely numeric and valid, and creates a new Position-attribute in mlPosition_.
	void mlLoadVertex(const std::vector<std::string>& params);
	//! Loads parameters previously parsed as being entirely numeric and valid, and creates a new Normal-element in mlNormal_.
	void mlLoadNorm(const std::vector<std::string>& params);
	//! Loads parameters previously parsed as being entirely numeric and valid, and creates a new Tex-element in mlTex_.
	void mlLoadTex(const std::vector<std::string>& params);
	//! Further splits the read line into seperate face-attributes, parses them to ensure valid values - and calls loading of these.
	/*!
		\return A boolean dictating whether or not the method was sucessful.
		\sa mlParseFace
		\sa mlLoadFace
	*/
	bool mlLoadFaces(const std::vector<std::string>& params);
	//! Parses faces and ensures that these values are the correct number and entirely numeric.
	/*!
		\return A boolean dictating whether or not the method was sucessful.
		\sa mlIsNumeric
	*/
	bool mlParseFace(const std::vector<std::string>& splitFaces);
	//! Loads faces.
	/*!
		Based on a vector of intermediate MeshFace-types, the method loads vertices from file and creates a vector of indices to save on memory.
		If a face is found to be equal to a previously loaded face, an index from this face is loaded as an index instead of a new one.
		\sa mlIndices_
		\sa mlVertices_
	*/
	void mlLoadFace(const std::vector<std::string>& face);

	void mlLoadGroup(const std::vector<std::string>& params);
	
	bool mlLoadIndex(unsigned int);

	//! Used to, for each read line, increment a counter so that an error may report a line number.
	void mlGetLine(std::string& line);
	//! If something were to go avry, this method prints an error based on line number.
	/*! \sa lineNum_ */
	void mlPrintFail();
	//! Checks whether or not a string contains an numeric value.
	/*! \return True if entirely numeric, False if not. */
	bool mlIsNumeric(std::string value);

	unsigned int				mlLineNum_;			//!< Line number previously read from file.
	SimpleStringSplitter		mlSSS_;				//!< Helper class used to split strings when reading these from file.

	std::vector<DirectX::XMFLOAT3> mlPosition_;	//!< Spatial attributes read from file.
	std::vector<DirectX::XMFLOAT3> mlNormal_;	//!< Normal attributes read from file.
	std::vector<DirectX::XMFLOAT2> mlTex_;		//!< Texture elements read from file.

	std::vector<MeshFace> mlFaces_;	//!< Utility-vector holding faces read form file to save on memory.

	std::vector<VertexPosNormTex>	mlVertices_;	//!< Result: Vertices read from file.
	std::vector<unsigned int>		mlIndices_;		//!< Result: Indices generated.

	std::vector<MeshGroup> mlGroups_;
};

#endif //XKILL_RENDERER_MESHLOADEROBJ_H