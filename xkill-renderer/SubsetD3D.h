#ifndef XKILL_RENDERER_SUBSETD3D_H
#define XKILL_RENDERER_SUBSETD3D_H

typedef long HRESULT;

struct ID3D11Device;
struct ID3D11Buffer;

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
		const unsigned int	numIndices,
		ID3D11Buffer*		indexBuffer);
	//! Deletes IB.
	~SubsetD3D();

	unsigned int	getMaterialIndex()	const;
	unsigned int	getNumIndices()		const;
	ID3D11Buffer*	getIndexBuffer()	const;
protected:
private:
	unsigned int	materialIndex_;	//!< Index of material used by subset.
	unsigned int	numIndices_;
	ID3D11Buffer*	indexBuffer_;
};

#endif //XKILL_RENDERER_SUBSETD3D_H