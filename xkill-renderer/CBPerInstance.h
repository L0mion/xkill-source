#ifndef XKILL_RENDERER_CBPERINSTANCE_H
#define XKILL_RENDERER_CBPERINSTANCE_H

#include "CB.h"

//! Class for a constant buffer that will be updated at initialization of the application.
/*!
\ingroup xkill-renderer-constant-buffer
*/
class CBPerInstance : public CB
{
public:
	//! Sets CBPerInstance to its default state.
	CBPerInstance();
	//! Releases all memory and returns CBPerInstance to default state
	virtual ~CBPerInstance();
	//! Releases all memory and returns CBPerInstance to default state
	virtual void reset();
	//! Updates the constant buffer with new data.
	/*!
	\param devcon DirectX Decive Context pointer.
	\param screenWidth Width of the screen/window.
	\param screenHeight Height of the screen/window.
	\param tileWidth Number of threads in each thread group along the x-axis
	\param tileHeight Number of threads in each thread group along the y-axis
	*/
	void update(ID3D11DeviceContext* devcon,
				unsigned int screenWidth,
				unsigned int screenHeight,
				unsigned int tileWidth,
				unsigned int tileHeight);
};

#endif //XKILL_RENDERER_CBPERINSTANCE_H