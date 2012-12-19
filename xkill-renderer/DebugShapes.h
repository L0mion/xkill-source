#ifndef XKILL_RENDERER_DEBUGSHAPES_H
#define XKILL_RENDERER_DEBUGSHAPES_H

#include <vector>

#include <xkill-utilities/MeshVertices.h>

const static unsigned int SPHERE_RES = 100;

//! Object creating debug shapes.
class DebugShapes
{
public:
	DebugShapes();	//!< Does nothing.
	~DebugShapes();	//!< Does nothing.

	std::vector<VertexPosColor> getSphere(float radius); //! Creates an array of vertices forming a sphere based on passed radius.
	std::vector<VertexPosColor> getBB(Float3 bbMin, Float3 bbMax);
protected:
private:
};

#endif //XKILL_RENDERER_DEBUGSHAPES_H