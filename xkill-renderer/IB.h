#ifndef XKILL_RENDERER_IB_H
#define XKILL_RENDERER_IB_H

#include <vector>

struct ID3D11Buffer;
struct ID3D11Device;

class IB
{
public:
	IB();
	~IB();

	HRESULT init(
		std::vector<unsigned int>	indices,
		ID3D11Device*				device);

	ID3D11Buffer*		getIB();
	const unsigned int	getNumIndices();
protected:
private:
	ID3D11Buffer*	ib_;
	unsigned int	numIndices_;
};

#endif //XKILL_RENDERER_IB_H