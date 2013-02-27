#ifndef XKILL_RENDERER_MANAGEMENTBUFFER_H
#define XKILL_RENDERER_MANAGEMENTBUFFER_H

typedef long HRESULT;

class Winfo;
class Buffer_Srv;
class Buffer_SrvRtv;
class Buffer_SrvDsv;
class Buffer_SrvRtvUav;

struct ID3D11Device;
struct ID3D11DeviceContext;

#include "gBufferID.h"

static const FLOAT CLEARCOLOR_BLACK[]	= { 0.0f, 0.0f, 0.0f, 1.0f };
static const FLOAT CLEARCOLOR_RED[]		= { 1.0f, 0.0f, 0.0f, 1.0f };
static const FLOAT CLEARCOLOR_GREEN[]	= { 0.0f, 1.0f, 0.0f, 1.0f };
static const FLOAT CLEARCOLOR_BLUE[]	= { 0.0f, 0.0f, 1.0f, 1.0f };

enum BUFFER_FORMAT
{
	R8_G8_B8_A8__UNORM,
	R16_G16_B16_A16__FLOAT,
	R32_G32_B32_A32__FLOAT,
	R16__FLOAT
};

static const BUFFER_FORMAT BUFFER_FORMAT_ALBEDO		= R8_G8_B8_A8__UNORM;
static const BUFFER_FORMAT BUFFER_FORMAT_NORMAL		= R16_G16_B16_A16__FLOAT;
static const BUFFER_FORMAT BUFFER_FORMAT_MATERIAL	= R16_G16_B16_A16__FLOAT;
static const BUFFER_FORMAT BUFFER_FORMAT_GLOW_HIGH	= R8_G8_B8_A8__UNORM;
static const BUFFER_FORMAT BUFFER_FORMAT_GLOW_LOW	= R8_G8_B8_A8__UNORM;
static const BUFFER_FORMAT BUFFER_FORMAT_SSAO		= R16__FLOAT;
static const BUFFER_FORMAT BUFFER_FORMAT_RANDOM		= R8_G8_B8_A8__UNORM;

static const unsigned int SHADER_REGISTER_DOWNSAMPLE_INPUT = 3;
static const unsigned int SHADER_REGISTER_BLUR_INPUT	= 9;
static const unsigned int SHADER_REGISTER_BLUR_OUTPUT	= 1;

static const unsigned int SHADER_REGISTER_SHADOWMAP = 4;

static const unsigned int DOWNSAMPLE_SCREEN_RES_FACTOR = 4;

static const unsigned int SHADOWMAP_DIM = 1024; //Remember to also set dimensions in CS_Lighting (SHADOWMAP_SIZE)

static const unsigned int SSAO_MAP_SCREEN_RES_FACTOR = 2;

static const unsigned int RANDOM_DIM = 256;

enum SET_TYPE
{
	SET_TYPE_SRV,
	SET_TYPE_RTV,
	SET_TYPE_UAV,
	SET_TYPE_DSV
};
enum SET_STAGE
{
	SET_STAGE_PS,
	SET_STAGE_CS
};
enum SET_ID
{
	SET_ID_GLOW_HIGH,
	SET_ID_GLOW_LOW,
	SET_ID_GLOW_LOW_UTIL,
	SET_ID_SHADOW,
	SET_ID_NORMAL,
	SET_ID_RANDOM
};

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

	void setBuffer(
		ID3D11DeviceContext* devcon,
		SET_ID setID, 
		SET_TYPE setType, 
		SET_STAGE setStage, 
		unsigned int shaderRegister); //!< This function can be dangerous if one's not careful. Be sure not to set SET_TYPE to something not corresponding to SET_ID.
	void unset(
		ID3D11DeviceContext* devcon,
		SET_TYPE setType,
		SET_STAGE setStage,
		unsigned int shaderRegister);

	D3D11_VIEWPORT getDownSampledViewport();
	D3D11_VIEWPORT getShadowViewport();

	//temp
	Buffer_SrvRtvUav* getSSAO() { return ssaoMap_; }

	void setRandomBuf(ID3D11DeviceContext* devcon, unsigned int shaderRegister)
	{ 
		ID3D11ShaderResourceView* resourceViews[1];
		resourceViews[0] = randomSRV_;
		devcon->CSSetShaderResources(shaderRegister, 1, resourceViews);
	}
protected:
private:
	HRESULT initAlbedo(		ID3D11Device* device);
	HRESULT initNormal(		ID3D11Device* device);
	HRESULT initMaterial(	ID3D11Device* device);
	HRESULT initGlow(		ID3D11Device* device, ID3D11DeviceContext* devcon);
	HRESULT initShadow(		ID3D11Device* device);
	HRESULT initSSAO(		ID3D11Device* device);
	HRESULT initRandom(		ID3D11Device* device);

	DXGI_FORMAT getFormat(BUFFER_FORMAT format);
	void getDownSampleDim(
		unsigned int screenWidth,
		unsigned int screenHeight,
		unsigned int& downSampleWidth, 
		unsigned int& downSampleHeight);
	void getSSAODim(
		unsigned int screenWidth,
		unsigned int screenHeight,
		unsigned int& ssaoWidth,
		unsigned int& ssaoHeight);

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

	//Shadowmap
	Buffer_SrvDsv* shadowMap_;
	D3D11_VIEWPORT shadowViewport_;

	//SSAO
	unsigned int ssaoWidth_;
	unsigned int ssaoHeight_;
	Buffer_SrvRtvUav* ssaoMap_;
	D3D11_VIEWPORT ssaoViewport_;

	ID3D11Texture2D*			randomTex_;
	ID3D11ShaderResourceView*	randomSRV_;
};

#endif //XKILL_RENDERER_MANAGEMENTBUFFER_H