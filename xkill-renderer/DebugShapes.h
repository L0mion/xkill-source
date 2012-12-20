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

	std::vector<VertexPosColor> getSphere(float radius);			//!< Creates an array of vertices forming a sphere based on passed radius.
	std::vector<VertexPosColor> getBB(Float3 bbMin, Float3 bbMax);	//!< Creates a line-list making out a bounding-box based on min- and max-passed values.
	std::vector<VertexPosColor> getFrustum(
		Float3 p1, Float3 p2,
		Float3 p3, Float3 p4,
		Float3 p5, Float3 p6,
		Float3 p7, Float3 p8); //!< Creates a line-list making out a box based on passed points.
protected:
private:
	std::vector<VertexPosColor> getBoxLines(
		Float3 p1, Float3 p2,
		Float3 p3, Float3 p4,
		Float3 p5, Float3 p6,
		Float3 p7, Float3 p8,
		Float3 color); //!< Retrieves a vector making out a box based on passed values.
};

#endif //XKILL_RENDERER_DEBUGSHAPES_H