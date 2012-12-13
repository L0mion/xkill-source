#ifndef XKILL_RENDERER_MODELMANAGEMENT_H
#define XKILL_RENDERER_MODELMANAGEMENT_H

typedef long HRESULT;

struct ID3D11Device;

class ModelD3D;
class VB;
class IB;

#include <map>

#include <xkill-utilities\MeshGeometry.h>
#include <xkill-utilities\AttributeType.h>

//! Holds rendering-view of models, including vertex- and index-buffers.
class ModelManagement
{
public:
	//! Does nothing.
	ModelManagement();
	//! Deletes all models held by ModelManagement. (the rendering-view of these, not the actual mesh-data read from file)
	~ModelManagement();

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
protected:
private:
	//! Creates a model based on model ID.
	HRESULT createModelD3D(
		const unsigned int	modelID, 
		ID3D11Device*		device);
	//! Retrieves a MeshAttribute corresponding to modelID, if such exists. Returns whether or not such an attribute was found.
	bool getMeshAttribute(unsigned int modelID, MeshAttribute& inout);
	//! Initializes vb.
	HRESULT createVertexBuffer(
		const unsigned int	modelID, 
		MeshGeometry&		geometry,
		VB*					vb,
		ID3D11Device*		device);
	//! Initializes ibs.
	HRESULT createIndexBuffers(
		const unsigned int	modelID, 
		MeshGeometry&		geometry, 
		std::vector<IB*>&	ibs,
		ID3D11Device*		device);
	//! Initializes ib.
	HRESULT createIndexBuffer(
		const unsigned int	modelID,
		MeshSubset&			subset,
		IB*					ib,
		ID3D11Device*		device);

	//! Adds a loaded ModelD3D to ModelManagement, where it will be stored for future use.
	void pushModelD3D(
		const unsigned int modelID, 
		ModelD3D* modelD3D);

	//! Checks whether or a model corresponding to passed ID exists or not.
	bool existingModelD3D(const int unsigned modelID);
	//! Gets index in vector containing ModelD3Ds based on model ID.
	unsigned int getModelD3DIndex(const int unsigned modelID);

	std::vector<ModelD3D*>	modelD3Ds_;						//!< Collection of ModelD3Ds.
	std::map<unsigned int, unsigned int> modelIDtoIndex_;	//!< Map mapping model ID to index in vector containing the models.
};

#endif //XKILL_RENDERER_MODELMANAGEMENT_H
