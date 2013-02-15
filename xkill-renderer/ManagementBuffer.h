#ifndef XKILL_RENDERER_MANAGEMENTBUFFER_H
#define XKILL_RENDERER_MANAGEMENTBUFFER_H

typedef long HRESULT;

class Winfo;
class Buffer_SrvRtv;
class Buffer_SrvRtvUav;

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

static const GBUFFER_FORMAT GBUFFER_FORMAT_ALBEDO		= R8_G8_B8_A8__UNORM;
static const GBUFFER_FORMAT GBUFFER_FORMAT_NORMAL		= R16_G16_B16_A16__FLOAT;
static const GBUFFER_FORMAT GBUFFER_FORMAT_MATERIAL		= R16_G16_B16_A16__FLOAT;
static const GBUFFER_FORMAT GBUFFER_FORMAT_GLOW_HIGH	= R8_G8_B8_A8__UNORM;
static const GBUFFER_FORMAT GBUFFER_FORMAT_GLOW_LOW		= R8_G8_B8_A8__UNORM;

enum GLOW_BUFFER_DIMENSIONS
{
	HALF_SCREEN_RES
};
static const GLOW_BUFFER_DIMENSIONS GLOWBUFLOW_GLOW_BUFFER_DIMENSIONS = HALF_SCREEN_RES;

static const unsigned int SHADER_REGISTER_GLOW = 3;

class ManagementBuffer
{
public:
	ManagementBuffer(Winfo* winfo);
	~ManagementBuffer();

	void reset();
	HRESULT resize(ID3D11Device* device);
	HRESULT init(ID3D11Device* device);

	void clearBuffers(ID3D11DeviceContext* devcon);
	void setBuffersAndDepthBufferAsRenderTargets(ID3D11DeviceContext*	devcon, ID3D11DepthStencilView*	depthBuffer);
	void unsetBuffersAndDepthBufferAsRenderTargets(ID3D11DeviceContext* devcon);
	void setBuffersAsCSShaderResources(ID3D11DeviceContext* devcon);
	void unsetBuffersAsCSShaderResources(ID3D11DeviceContext* devcon);

	void setGlowLowAsRTV(ID3D11DeviceContext* devcon);
	void unsetGlowLowAsRTV(ID3D11DeviceContext* devcon);
	void setGlowHighAsSRV(ID3D11DeviceContext* devcon);
	void unsetGlowHighAsSrv(ID3D11DeviceContext* devcon);
protected:
private:
	HRESULT initAlbedo(ID3D11Device* device);
	HRESULT initNormal(ID3D11Device* device);
	HRESULT initMaterial(ID3D11Device* device);
	HRESULT initGlow(ID3D11Device* device);

	/*desc*/
	Winfo* winfo_;

	Buffer_SrvRtv* gBuffers_[GBUFFERID_NUM_BUFFERS];	//!< Containing data for deferred rendering.

	Buffer_SrvRtvUav* glowHigh_;
	Buffer_SrvRtvUav* glowLow_;
};

#endif //XKILL_RENDERER_MANAGEMENTBUFFER_H