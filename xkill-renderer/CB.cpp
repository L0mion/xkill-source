#include <CB.h>

#include <RenderingUtilities.h>

CB::CB()
{
	constantBuffer_ = nullptr;
}
CB::~CB()
{
	SAFE_RELEASE(constantBuffer_);
}
void CB::reset()
{
	SAFE_RELEASE(constantBuffer_);
}

HRESULT CB::init(ID3D11Device* device, unsigned int size)
{
	HRESULT hr = S_OK;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = size;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	hr = device->CreateBuffer(&bd, NULL, &constantBuffer_);
	if(FAILED(hr))
		ERROR_MSG(L"ConstantBuffer::init CreateBuffer Failed!");

	return hr;
}

void CB::vsSet(ID3D11DeviceContext* devcon)
{
	devcon->VSSetConstantBuffers(0, 1, &constantBuffer_);
}
void CB::psSet(ID3D11DeviceContext* devcon)
{
	devcon->PSSetConstantBuffers(0, 1, &constantBuffer_);
}
void CB::csSet(ID3D11DeviceContext* devcon)
{
	devcon->CSSetConstantBuffers(0, 1, &constantBuffer_);
}
