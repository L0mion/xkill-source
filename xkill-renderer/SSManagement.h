#ifndef XKILL_RENDERER_SSMANAGEMENT_H
#define XKILL_RENDERER_SSMANAGEMENT_H

#include <d3d11.h>

#include "d3dInterface.h"

enum SS_ID
{
	SS_ID_DEFAULT
};

class SSManagement : public D3DInterface
{
public:
	SSManagement();
	virtual ~SSManagement();

	virtual void reset();

	void setVS(ID3D11DeviceContext* devcon, SS_ID ssId, unsigned int shaderRegister);
	void setPS(ID3D11DeviceContext* devcon, SS_ID ssId, unsigned int shaderRegister);
	void setCS(ID3D11DeviceContext* devcon, SS_ID ssId, unsigned int shaderRegister);

	HRESULT init(ID3D11Device* device);
private:

	HRESULT initSSDefault(ID3D11Device* device);

	ID3D11SamplerState* ssDefault_;
};

#endif //XKILL_RENDERER_SSMANAGEMENT_H