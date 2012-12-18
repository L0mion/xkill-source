#ifndef XKILL_RENDERER_MODELMANAGEMENT_H
#define XKILL_RENDERER_MODELMANAGEMENT_H

typedef long HRESULT;

struct ID3D11Device;

class ModelD3D;
class DebugShapeD3D;
class SubsetD3D;
class VB;
class IB;

#include <map>

#include <xkill-utilities\MeshGeometry.h>
#include <xkill-utilities\AttributeType.h>

//! Holds rendering-view of models, including vertex- and index-buffers.
/*!
\ingroup xkill-renderer
*/
class ManagementModel
{
public:
	//! Does nothing.
	ManagementModel();
	//! Deletes all models held by ModelManagement. (the rendering-view of these, not the actual mesh-data read from file)
	~ManagementModel();

	//! Does nothing.
	HRESULT init();

	//! Gets a ModelD3D-type object from ModelManagement, based on ModelID.
	/*! If model is not known to ModelManagement; ModelManagement will attempt to load model.
	\param modelID	ID of model.
	\param device	Handle to D3D-device.
	\sa createModelD3D
	*/
	ModelD3D* getModelD3D(
		const unsigned int	modelID, 
		ID3D11Device*		device);
	DebugShapeD3D* getDebugShapeD3D(
		const unsigned int	shapeIndex,
		ID3D11Device*		device);
protected:
private:
	HRESULT createModelD3D(const unsigned int modelID, ID3D11Device* device);	//!< Creates a model based on model ID.
	bool getMeshAttribute(unsigned int modelID, MeshAttribute& inout);			//!< Retrieves a MeshAttribute corresponding to modelID, if such exists. Returns whether or not such an attribute was found.
	HRESULT createVertexBuffer(
		const unsigned int	modelID, 
		MeshGeometry&		geometry,
		VB*					vb,
		ID3D11Device*		device); //!< Initializes vb.
	HRESULT createIndexBuffers(
		const unsigned int			modelID, 
		MeshGeometry&				geometry, 
		std::vector<SubsetD3D*>&	subsetD3Ds,
		ID3D11Device*				device); //!< Initializes ibs.
	HRESULT createIndexBuffer(
		const unsigned int	modelID,
		MeshSubset&			subset,
		IB*					ib,
		ID3D11Device*		device); //!< Initializes ib.

	HRESULT createDebugShapeD3D(unsigned int shapeID, ID3D11Device* device);

	void pushModelD3D(
		const unsigned int modelID, 
		ModelD3D* modelD3D); //!< Adds a loaded ModelD3D to ModelManagement, where it will be stored for future use.
	
	bool			existingModelD3D(const int unsigned modelID);			//!< Checks whether or a model corresponding to passed ID exists or not.
	unsigned int	getModelD3DIndex(const int unsigned modelID);	//!< Gets index in vector containing ModelD3Ds based on model ID.
	bool			existingDebugShapeD3D(const unsigned int shapeIndex);
	unsigned int	getDebugShapeD3DIndex(const int unsigned shapeIndex);

	std::vector<ModelD3D*>	modelD3Ds_;						//!< Collection of ModelD3Ds.
	std::map<unsigned int, unsigned int> modelIDtoIndex_;	//!< Map mapping model ID to index in vector containing the models.

	std::vector<DebugShapeD3D*> debugShapeD3Ds_;
	std::map<unsigned int, unsigned int> shapeIndextoD3DIndex_;
};

#endif //XKILL_RENDERER_MODELMANAGEMENT_H
