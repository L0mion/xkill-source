#ifndef XKILL_RENDERER_MESHORIGINS_H
#define XKILL_RENDERER_MESHORIGINS_H

#include <vector>

#include "dllUtilities.h"
#include "MeshSubset.h"
#include "MeshVertices.h"

//! Encapsulates origin data of a mesh.
/*!
Encapsulates filename etc.
\ingroup xkill-mesh
*/
class DLL_U MeshOrigins
{
public:
	//! Does nothing.
	MeshOrigins();
	//! Initializes MeshOrigins to data passed through constructor.
	MeshOrigins(const std::string name);
	~MeshOrigins();

	std::string getName();
protected:
private:
	std::string name_;	//!< Name of the file
};

#endif //XKILL_RENDERER_MESHORIGINS_H