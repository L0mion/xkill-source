#ifndef XKILL_RENDERER_MANAGEMENTGBUFFER_H
#define XKILL_RENDERER_MANAGEMENTGBUFFER_H

typedef long HRESULT;

class Winfo;
class GBuffer;

struct ID3D11Device;
struct ID3D11DeviceContext;

#include "gBufferID.h"

static const FLOAT CLEARCOLOR_BLACK[]	= { 0.0f, 0.0f, 0.0f, 1.0f };
static const FLOAT CLEARCOLOR_RED[]		= { 1.0f, 0.0f, 0.0f, 1.0f };
static const FLOAT CLEARCOLOR_GREEN[]	= { 0.0f, 1.0f, 0.0f, 1.0f };
static const FLOAT CLEARCOLOR_BLUE[]	= { 0.0f, 0.0f, 1.0f, 1.0f };

enum GBUFFER_FORMAT
{
	R8_G8_B8_A8__UNORM,
	R16_G16_B16_A16__FLOAT,
	R32_G32_B32_A32__FLOAT
};

static const GBUFFER_FORMAT GBUFFER_FORMAT_ALBEDO	= R8_G8_B8_A8__UNORM;
static const GBUFFER_FORMAT GBUFFER_FORMAT_NORMAL	= R16_G16_B16_A16__FLOAT;
static const GBUFFER_FORMAT GBUFFER_FORMAT_MATERIAL	= R32_G32_B32_A32__FLOAT;

class ManagementGBuffer
{
public:
	ManagementGBuffer(Winfo* winfo);
	~ManagementGBuffer();

	void reset();
	HRESULT resize(ID3D11Device* device);
	HRESULT init(ID3D11Device* device);

	void clearGBuffers(ID3D11DeviceContext* devcon);
	void setGBuffersAndDepthBuffer(ID3D11DeviceContext*	devcon, ID3D11DepthStencilView*	depthBuffer);
	void unsetGBuffersAndDepthBufferAsRenderTargets(ID3D11DeviceContext* devcon);
	void setGBuffersAsCSShaderResources(ID3D11DeviceContext* devcon);
protected:
private:
	HRESULT initAlbedo(ID3D11Device* device);
	HRESULT initNormal(ID3D11Device* device);
	HRESULT initMaterial(ID3D11Device* device);

	/*desc*/
	Winfo* winfo_;

	GBuffer* gBuffers_[GBUFFERID_NUM_BUFFERS];	//!< Containing data for deferred rendering.
};

#endif //XKILL_RENDERER_MANAGEMENTGBUFFER_H