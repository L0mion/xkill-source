#ifndef XKILL_RENDERER_MESHMODELD3D_H
#define XKILL_RENDERER_MESHMODELD3D_H

#include <vector>

struct ID3D11Buffer;

class MeshModelD3D
{
public:
	MeshModelD3D(
		ID3D11Buffer*						vertexBuffer,
		const std::vector<ID3D11Buffer*>	indexBuffers);
	~MeshModelD3D();

	const std::vector<ID3D11Buffer*>&	getIndexBuffers();
	ID3D11Buffer*						getVertexBuffer();
protected:
private:
	ID3D11Buffer*				vertexBuffer_;
	std::vector<ID3D11Buffer*>	indexBuffers_;
};

#endif //XKILL_RENDERER_MESHMODELD3D_H