#ifndef XKILL_RENDERER_MESHMODELD3D_H
#define XKILL_RENDERER_MESHMODELD3D_H

#include <vector>

#include <xkill-utilities/MeshMaterial.h>
#include <xkill-utilities/MeshVertices.h>

template <class Vertex>    
class VB;
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
		VB<VertexPosNormTex>*				vertexBuffer,
		const std::vector<SubsetD3D*>		subsets,
		const std::vector<MeshMaterial>		materials);
	//! Deletes VB-type and vector of IBs.
	~ModelD3D();

	VB<VertexPosNormTex>*		getVertexBuffer();
	std::vector<SubsetD3D*>&	getSubsetD3Ds();
	std::vector<MeshMaterial>&	getMaterials();
protected:
private:
	VB<VertexPosNormTex>*		vertexBuffer_;	//!< Vertex buffer.
	std::vector<SubsetD3D*>		subsets_;		//!< Index buffers.
	std::vector<MeshMaterial>	materials_;		//!< Materials.
};

/*
All work and no play makes Eric a dull boy.
*/

#endif //XKILL_RENDERER_MESHMODELD3D_H