#ifndef XKILL_RENDERER_D3DINTERFACE_H
#define XKILL_RENDERER_D3DINTERFACE_H

//! Common interface to be used by all D3D-classes.
/*!
\ingroup xkill-renderer
*/
class D3DInterface
{
public:
	//! Common abstract reset-method. Has to be implemented by all derived classes of D3DInterface.
	/*!
	Is used to fully 'reset' an object, effectively releasing all members - and initializing them to default values again.
	*/
	virtual void reset()	= 0;
};

#endif //XKILL_RENDERER_D3DINTERFACE_H