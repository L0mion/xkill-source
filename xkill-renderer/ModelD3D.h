#ifndef XKILL_RENDERER_MESHMODELD3D_H
#define XKILL_RENDERER_MESHMODELD3D_H

#include <vector>

#include <xkill-utilities/MeshMaterial.h>
#include <xkill-utilities/MeshVertices.h>

#include <xkill-utilities/MaterialDesc.h>

struct ID3D11Buffer;

class SubsetD3D;

//! Type representing the rendering-view of a model.
/*!
\ingroup xkill-renderer
*/
class ModelD3D
{
public:
	//! Initializes object based on passed parameters.
	/*!
	\param vb	Vertex Buffer.
	\param ibs	A collection of index buffers.
	*/
	ModelD3D(
		ID3D11Buffer*						vertexBuffer,
		const std::vector<SubsetD3D*>		subsets,
		const std::vector<MaterialDesc>		materials);
	//! Deletes VB-type and vector of IBs.
	~ModelD3D();

	std::vector<SubsetD3D*>&	getSubsetD3Ds();
	std::vector<MaterialDesc>&	getMaterials();
	ID3D11Buffer*				getVertexBuffer();
protected:
private:
	std::vector<SubsetD3D*>		subsets_;		//!< Index buffers.
	std::vector<MaterialDesc>	materials_;		//!< Materials.
	ID3D11Buffer*				vertexBuffer_;
};

/*
All work and no play makes Eric a dull boy.
*/

#endif //XKILL_RENDERER_MESHMODELD3D_H