#ifndef XKILL_RENDERER_IB_H
#define XKILL_RENDERER_IB_H

typedef long HRESULT;

struct ID3D11Buffer;
struct ID3D11Device;

#include <vector>

//! Describes an index buffer.
/*!
\ingroup xkill-renderer
*/
class IB
{
public:
	//! Sets IB to default state.
	IB();
	//! Releases ID3D11Buffer-type.
	~IB();

	//! Initializes IB based on indices sent as argument.
	HRESULT init(
		std::vector<unsigned int>	indices,
		ID3D11Device*				device);

	ID3D11Buffer*		getIB();
	const unsigned int	getNumIndices();
protected:
private:
	ID3D11Buffer*	ib_;			//!< D3D-type index buffer.
	unsigned int	numIndices_;	//!< Number of indices in index buffer.
};

#endif //XKILL_RENDERER_IB_H