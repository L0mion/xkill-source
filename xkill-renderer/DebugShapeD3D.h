#ifndef XKILL_RENDERER_DEBUGSHAPED3D_H
#define XKILL_RENDERER_DEBUGSHAPED3D_H

class VB;
class IB;

class DebugShapeD3D
{
public:
	DebugShapeD3D(VB* vb, IB* ib);
	~DebugShapeD3D();

	VB* getVB() const;
	IB* getIB() const;
protected:
private:
	VB* vb_;
	IB* ib_;
};

#endif //XKILL_RENDERER_DEBUGSHAPED3D_H