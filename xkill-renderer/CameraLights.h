#ifndef XKILL_RENDERER_CAMERALIGHTS_H
#define XKILL_RENDERER_CAMERALIGHTS_H

#include "DataStreamSRV.h"

class CameraLights
{
public:
	CameraLights();
	~CameraLights();

	void init();
	void reset();

	void resetLights();
	void updateLights(ID3D11Device* device, ID3D11DeviceContext* devcon);

	void addPosPointLight(Float3 pointLightPos);
	void addDirDirLight(Float3 dirLightDir);

	void setSRV(ID3D11DeviceContext* devcon, unsigned int shaderRegisterPos, unsigned int shaderRegisterDir);
	void unsetSRV(ID3D11DeviceContext* devcon, unsigned int shaderRegisterPos, unsigned int shaderRegisterDir);
protected:
private:
	DataStreamSRV<Float3>* streamPosPointLight_;
	DataStreamSRV<Float3>* streamDirDirLight_;
};

#endif //XKILL_RENDERER_CAMERALIGHTS_H