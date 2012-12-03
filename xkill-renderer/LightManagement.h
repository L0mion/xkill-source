#ifndef XKILL_RENDERER_LIGHTMANAGEMENT_H
#define XKILL_RENDERER_LIGHTMANAGEMENT_H

#include <vector>

#include "d3dInterface.h"
#include "LightDesc.h"

struct ID3D11UnorderedAccesView;
struct ID3D11Buffer;
struct ID3D11Device;
struct ID3D11DeviceContext;
//typedef HRESULT;



class LightManagement : public D3DInterface
{
public:
	LightManagement();
	~LightManagement();

	virtual void reset();

	HRESULT init(ID3D11Device* device);

	HRESULT updateBufferData(ID3D11DeviceContext* devcon);
	void setLightSRVCS(ID3D11DeviceContext* devcon, unsigned int shaderRegister);
	void addLight(LightDesc light, ID3D11Device* device, ID3D11DeviceContext* devcon);
private:
	void createDirectionalLight(ID3D11Device* device);
	void resizeLights(ID3D11Device* device);
	HRESULT createLightBuffer(ID3D11Device* device);
	HRESULT createLightSRV(ID3D11Device* device);
	
	unsigned int maxNumLights_;
	unsigned int numLights_;
	std::vector<LightDesc>		lights_;
	ID3D11Buffer*				lightBuffer_;
	ID3D11ShaderResourceView*	lightSRV_;
};


#endif //XKILL_RENDERER_LIGHTMANAGEMENT_H