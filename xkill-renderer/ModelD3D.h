#ifndef XKILL_RENDERER_MESHMODELD3D_H
#define XKILL_RENDERER_MESHMODELD3D_H

#include <vector>

class VB;
class IB;

//! Type representing the rendering-view of a model.
class ModelD3D
{
public:
	//! Initializes object based on passed parameters.
	/*!
	\param vb	Vertex Buffer.
	\param ibs	A collection of index buffers.
	*/
	ModelD3D(
		VB* vb,
		const std::vector<IB*> ibs);
	//! Deletes VB-type and vector of IBs.
	~ModelD3D();

	VB* getVB();
	const std::vector<IB*>&	getIBs();
protected:
private:
	VB* vb_;				//!< Vertex buffer.
	std::vector<IB*> ibs_;	//!< Index buffers.
};

/*
All work and no play makes Eric a dull boy.
*/

#endif //XKILL_RENDERER_MESHMODELD3D_H