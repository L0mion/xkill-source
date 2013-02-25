#ifndef XKILL_RENDERER_RSMANAGEMENT_H
#define XKILL_RENDERER_RSMANAGEMENT_H

#include <d3d11.h>

#include "d3dInterface.h"

enum RS_ID
{
	RS_ID_DEFAULT,
	RS_ID_DEPTH
};

//! Class for maintaining rasterizer states.
/*!
\ingroup xkill-renderer
*/
class ManagementRS : public D3DInterface
{
public:
	//! Initializes RSManagement to its default state.
	ManagementRS();
	//!Releases all memory and resets RSManagement to its default state.
	virtual ~ManagementRS();
	//!Releases all memory and resets RSManagement to its default state.
	virtual void reset();

	//! Set a rasterizer state to the pipeline
	/*!
	\param devcon Pointer to a DirectX Device Context.
	\param rsId An enum identifying the rasterizer state to be set.
	*/
	void setRS(ID3D11DeviceContext* devcon, RS_ID rsId);

	void unsetRS(ID3D11DeviceContext* devcon);

	//! Initializes RSManagement's members.
	/*!
	\return Return any error encountered.
	\param device A pointer to a DirectX Device.
	\sa initRSDefault
	*/
	HRESULT init(ID3D11Device* device);
private:
	//! Initializes the variable rsDefault_.
	/*!
	\return Return any error encountered.
	\param device A pointer to a DirectX Device.
	\sa rsDefault_
	*/
	HRESULT initRSDefault(ID3D11Device* device);
	HRESULT initRSDepth(ID3D11Device* device);

	ID3D11RasterizerState* rsDefault_; //!< A rasterizer state object.
	ID3D11RasterizerState* rsDepth_;
};

#endif //XKILL_RENDERER_RSMANAGEMENT_H