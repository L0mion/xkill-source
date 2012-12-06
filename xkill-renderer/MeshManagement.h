#ifndef XKILL_RENDERER_MESHMANAGEMENT_H
#define XKILL_RENDERER_MESHMANAGEMENT_H

#include <map>

#include <xkill-utilities\MeshGeometry.h>

class MeshModelD3D;
class VB;
class IB;

class MeshManagement
{
public:
	MeshManagement();
	~MeshManagement();

	MeshModelD3D* getMeshModelD3D(
		const unsigned int	index, 
		ID3D11Device*		device);
protected:
private:
	HRESULT createMeshModelD3D(
		const unsigned int	index, 
		ID3D11Device*		device);
	HRESULT createVertexBuffer(
		const unsigned int	index, 
		MeshGeometry&		geometry,
		VB*					vb,
		ID3D11Device*		device);
	HRESULT createIndexBuffers(
		const unsigned int			index, 
		MeshGeometry&				geometry, 
		std::vector<IB*>&			ibs,
		ID3D11Device*				device);
	HRESULT createIndexBuffer(
		const unsigned int	index,
		MeshSubset&			subset,
		IB*					ib,
		ID3D11Device*		device);

	void pushMeshModelD3D(
		const unsigned int index, 
		MeshModelD3D* meshModelD3D);

	bool existingMeshModelD3DIndex(const int unsigned index);
	unsigned int getMeshModelD3DIndex(const int unsigned index);

	std::vector<MeshModelD3D*>	meshModelD3Ds_;
	std::map<unsigned int, unsigned int> map;
};

#endif //XKILL_RENDERER_MESHMANAGEMENT_H
