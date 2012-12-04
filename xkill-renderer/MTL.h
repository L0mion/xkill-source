#ifndef XKILL_RENDERER_MTL_H
#define XKILL_RENDERER_MTL_H

/*! \defgroup xkill-mesh-io-mtl xkill-mesh-io-mtl
	Loading of .mtl-format.
	\ingroup xkill-mesh-io
*/

#include <vector>

//! Data type representing read .mtl-file in its entirety. 
/*!
\ingroup xkill-mesh-io-mtl
*/
class MTLMaterial;

class MTL
{
public:
	//! Does nothing.
	MTL();
	//! Fills object with materials passed as arguments.
	MTL(std::vector<MTLMaterial> materials);
	~MTL();

	const std::vector<MTLMaterial> getMaterials();
protected:
private:
	std::vector<MTLMaterial> materials_; //!< Materials read from .mtl.
};

#endif //XKILL_RENDERER_MTL_H