#ifndef XKILL_RENDERER_VB_H
#define XKILL_RENDERER_VB_H

#include <Windows.h>
#include <vector>

struct ID3D11Buffer;
struct VertexPosNormTex;
struct ID3D11Device;

class VB
{
public:
	VB();
	~VB();

	HRESULT init(
		std::vector<VertexPosNormTex>&	vertices,
		ID3D11Device*					device);

	ID3D11Buffer*		getVB();
	const unsigned int	getNumVertices();
protected:
private:
	ID3D11Buffer*	vb_;
	unsigned int	numVertices_;
};

#endif //XKILL_RENDERER_VB_H