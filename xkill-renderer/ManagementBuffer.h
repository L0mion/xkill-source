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

static const unsigned int SHADER_REGISTER_SRV_GLOW_HIGH = 3;

static const unsigned int DOWNSAMPLE_SCREEN_RES_FACTOR = 4;

class ManagementBuffer
{
public:
	ManagementBuffer(Winfo* winfo);
	~ManagementBuffer();

	void reset();
	HRESULT resize(ID3D11Device* device);
	HRESULT init(ID3D11Device* device, ID3D11DeviceContext* devcon);

	void clearBuffers(ID3D11DeviceContext* devcon);
	void setBuffersAndDepthBufferAsRenderTargets(ID3D11DeviceContext*	devcon, ID3D11DepthStencilView*	depthBuffer);
	void unsetBuffersAndDepthBufferAsRenderTargets(ID3D11DeviceContext* devcon);
	void setBuffersAsCSShaderResources(ID3D11DeviceContext* devcon);
	void unsetBuffersAsCSShaderResources(ID3D11DeviceContext* devcon);

	void setGlowLowAsRTV(ID3D11DeviceContext* devcon);
	void unsetGlowLowAsRTV(ID3D11DeviceContext* devcon);
	void setGlowLowAsSRVToCS(ID3D11DeviceContext* devcon, unsigned int shaderRegister);
	void setGlowLowAsSRVToPS(ID3D11DeviceContext* devcon, unsigned int shaderRegister);
	void unsetGlowLowAsSRV(ID3D11DeviceContext* devcon, unsigned int shaderRegister);
	void setGlowLowAsUAV(ID3D11DeviceContext* devcon, unsigned int shaderRegister);
	
	void setGlowHighAsSRV(ID3D11DeviceContext* devcon, unsigned int shaderRegister);
	void unsetGlowHighAsSrv(ID3D11DeviceContext* devcon, unsigned int shaderRegister);
	void setGlowHighAsRTV(ID3D11DeviceContext* devcon);
	void unsetGlowHighAsRTV(ID3D11DeviceContext* devcon);
	
	void setGlowLowUtilAsUAV(ID3D11DeviceContext* devcon, unsigned int shaderRegister);
	void setGlowLowUtilAsSRV(ID3D11DeviceContext* devcon, unsigned int shaderRegister);

	//void setGlow(ID3D11DeviceContext* devcon, GLOW_SET glowSet, TYPE_SET typeSet, unsigned int registerSet);
	//void unsetGlow(ID3D11DeviceContext* devcon, TYPE_SET typeSet, unsigned int registerSet);

	D3D11_VIEWPORT getDownSampledViewport();

	Buffer_SrvRtvUav* getGlowLow() { return glowLow_; }
	Buffer_SrvRtvUav* getGlowLowUtil() { return glowLowUtil_; }
protected:
private:
	HRESULT initAlbedo(ID3D11Device* device);
	HRESULT initNormal(ID3D11Device* device);
	HRESULT initMaterial(ID3D11Device* device);
	HRESULT initGlow(ID3D11Device* device, ID3D11DeviceContext* devcon);

	DXGI_FORMAT getFormat(GBUFFER_FORMAT format);
	void getDownSampleDim(
		unsigned int screenWidth,
		unsigned int screenHeight,
		unsigned int& downSampleWidth, 
		unsigned int& downSampleHeight);

	/*desc*/
	Winfo* winfo_;

	Buffer_SrvRtv* gBuffers_[GBUFFERID_NUM_BUFFERS];	//!< Containing data for deferred rendering.

	//Glow
	unsigned int downSampleWidth_;
	unsigned int downSampleHeight_;
	D3D11_VIEWPORT downSampleViewport_;
	Buffer_SrvRtvUav* glowHigh_;
	Buffer_SrvRtvUav* glowLow_;
	Buffer_SrvRtvUav* glowLowUtil_;
};

#endif //XKILL_RENDERER_MANAGEMENTBUFFER_H