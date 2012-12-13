#ifndef XKILL_RENDERER_VB_H
#define XKILL_RENDERER_VB_H

typedef long HRESULT;

struct ID3D11Buffer;
struct VertexPosNormTex;
struct ID3D11Device;

#include <vector>

//! Describes a vertex buffer.
class VB
{
public:
	//! Initializes VB to default state.
	VB();
	//! Release ID3D11Buffer-type object.
	~VB();

	//! Initializes VB based on vertices sent as argument.
	HRESULT init(
		std::vector<VertexPosNormTex>&	vertices,
		ID3D11Device*					device);

	ID3D11Buffer*		getVB();
	const unsigned int	getNumVertices();
protected:
private:
	ID3D11Buffer*	vb_;			//!< D3D-vertex buffer type object.
	unsigned int	numVertices_;	//!< Number of vertices contained in vertex buffer.
};

#endif //XKILL_RENDERER_VB_H