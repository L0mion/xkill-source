#ifndef XKILL_RENDERER_SUBSETD3D_H
#define XKILL_RENDERER_SUBSETD3D_H

typedef long HRESULT;

struct ID3D11Device;

class IB;

#include <vector>

//! Describes the rendering view of a model subset.
/*!
\ingroup xkill-renderer
*/
class SubsetD3D
{
public:
	//! Initializes SubsetD3D with previously initialized data.
	SubsetD3D(
		const unsigned int	materialIndex,
		IB*					indexBuffer);
	//! Deletes IB.
	~SubsetD3D();

	unsigned int	getMaterialIndex()	const;
	IB*				getIndexBuffer()	const;
protected:
private:
	unsigned int	materialIndex_;	//!< Index of material used by subset.
	IB*				indexBuffer_;	//!< Object wrapping a D3D-indexbuffer.
};

#endif //XKILL_RENDERER_SUBSETD3D_H