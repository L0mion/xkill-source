#ifndef XKILL_RENDERER_MODELMANAGEMENT_H
#define XKILL_RENDERER_MODELMANAGEMENT_H

typedef long HRESULT;

struct ID3D11Device;

class ModelD3D;
class SubsetD3D;

template <class Vertex>    
class VB;
class IB;

#include <xkill-utilities/MeshDesc.h>
#include <xkill-utilities/AttributeType.h>

#include <map>

//! Holds rendering-view of models, including vertex- and index-buffers.
/*!
\ingroup xkill-renderer
*/
class ManagementModel
{
public:
	ManagementModel();	//!< Sets ManagementModel to default state.
	~ManagementModel();	//! Deletes all models held by ModelManagement. (the rendering-view of these, not the actual mesh-data read from file)

	void unloadModels(); //!< Unload all models

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
	HRESULT createModelD3D(const unsigned int modelID, ID3D11Device* device);	//!< Creates a model based on model ID.
	bool getMeshAttribute(unsigned int modelID, AttributePtr<Attribute_Mesh>& inout);			//!< Retrieves a MeshAttribute corresponding to modelID, if such exists. Returns whether or not such an attribute was found.
	HRESULT createVertexBuffer(
		const unsigned int		modelID, 
		std::vector<VertexDesc>	vertices,
		ID3D11Buffer**			vb,
		VertexType				vertexType,
		ID3D11Device*			device);		//!< Initializes vb.
	HRESULT createIndexBuffers(
		const unsigned int			modelID, 
		std::vector<SubsetDesc>		subsets, 
		std::vector<SubsetD3D*>&	subsetD3Ds,
		ID3D11Device*				device);	//!< Initializes ibs.
	HRESULT createIndexBuffer(
		const unsigned int	modelID,
		SubsetDesc			subset,
		ID3D11Buffer**		indexBuffer,
		ID3D11Device*		device);			//!< Initializes ib.

	void pushModelD3D(
		const unsigned int modelID, 
		ModelD3D* modelD3D);		//!< Adds a loaded ModelD3D to ModelManagement, where it will be stored for future use.

	std::vector<VertexPosColor> convertVertexPosColor(std::vector<VertexDesc>& vertices);
	std::vector<VertexPosNormTex> convertVertexPosNormTex(std::vector<VertexDesc>& vertices);
	std::vector<VertexPosNormTexSkinned> convertVertexPosNormTexSkinned(std::vector<VertexDesc>& vertices);
	std::vector<VertexPosNormSkinned> convertVertexPosNormSkinned(std::vector<VertexDesc>& vertices);
	std::vector<VertexPosNormTexTanSkinned> convertVertexPosNormTexTanSkinned(std::vector<VertexDesc>& vertices);
	std::vector<VertexPosNormTexTan> convertVertexPosNormTexTan(std::vector<VertexDesc>& vertices);

	bool			existingModelD3D(const int unsigned modelID);			//!< Checks whether or a model corresponding to passed ID exists or not.
	unsigned int	getModelD3DIndex(const int unsigned modelID);			//!< Gets index in vector containing ModelD3Ds based on model ID.

	std::vector<ModelD3D*>	modelD3Ds_;						//!< Collection of ModelD3Ds.
	std::map<unsigned int, unsigned int> modelIDtoIndex_;	//!< Map mapping model ID to index in vector containing the models.
};

#endif //XKILL_RENDERER_MODELMANAGEMENT_H
