#ifndef XKILL_RENDERER_DEBUGSHAPED3D_H
#define XKILL_RENDERER_DEBUGSHAPED3D_H

template <class Vertex>    
class VB;

#include <xkill-utilities/MeshVertices.h>

//! Describes the render-view of a debug-shape, such as a bounding-sphere.
class DebugShapeD3D
{
public:
	DebugShapeD3D(VB<VertexPosColor>* vb);
	~DebugShapeD3D();

	VB<VertexPosColor>* getVB() const;
protected:
private:
	VB<VertexPosColor>* vb_; //!< Holds vertex-buffer.
};

#endif //XKILL_RENDERER_DEBUGSHAPED3D_H