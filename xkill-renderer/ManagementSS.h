#ifndef XKILL_RENDERER_SSMANAGEMENT_H
#define XKILL_RENDERER_SSMANAGEMENT_H

#include <d3d11.h>

#include "TypeFX.h"
#include "d3dInterface.h"

enum SS_ID
{
	SS_ID_DEFAULT,
	SS_ID_SPRITE,
	SS_ID_SHADOW,
	SS_ID_NORMAL,
	SS_ID_DEPTH,
	SS_ID_RANDOM
};

//! Class for maintaining sampler states.
/*!
\ingroup xkill-renderer
*/
class ManagementSS : public D3DInterface
{
public:
	ManagementSS();				//!< Initializes SSManagement to its default state.
	virtual ~ManagementSS();	//!< Releases all memory and resets SSManagement to its default state.
	virtual void reset();		//!< Releases all memory and resets SSManagement to its default state.

	void setSS(
		ID3D11DeviceContext*	devcon,
		TypeFX					shaderStage,
		unsigned int			shaderRegister,
		SS_ID					ssId); //!< Sets a sampler state based on a samplerstate-identifier, a specified shader-stage and regiser.
	void ManagementSS::unsetSS(
		ID3D11DeviceContext*	devcon,
		TypeFX					shaderStage,
		unsigned int			shaderRegister);

	HRESULT init(ID3D11Device* device);

	void unsetSS();
private:
	HRESULT initSSDefault(ID3D11Device* device);
	HRESULT initSSShadow(ID3D11Device* device);
	HRESULT initSSNormal(ID3D11Device* device);
	HRESULT initSSDepth(ID3D11Device* device);
	HRESULT initSSRandom(ID3D11Device* device);

	ID3D11SamplerState* ssDefault_;	//!< A sampler state object.
	ID3D11SamplerState* ssShadow_;

	ID3D11SamplerState* ssNormal_;
	ID3D11SamplerState* ssDepth_;
	ID3D11SamplerState* ssRandom_;
};

#endif //XKILL_RENDERER_SSMANAGEMENT_H