#ifndef XKILL_RENDERER_RSMANAGEMENT_H
#define XKILL_RENDERER_RSMANAGEMENT_H

#include <d3d11.h>

#include "d3dInterface.h"

enum RS_ID
{
	RS_ID_DEFAULT
};

class RSManagement : public D3DInterface
{
public:
	RSManagement();
	virtual ~RSManagement();

	virtual void reset();

	void setRS(ID3D11DeviceContext* devcon, RS_ID rsId);


	HRESULT init(ID3D11Device* device);
private:
	HRESULT initRSDefault(ID3D11Device* device);

	ID3D11RasterizerState* rsDefault_;
};

#endif //XKILL_RENDERER_RSMANAGEMENT_H