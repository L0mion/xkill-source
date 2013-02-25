#ifndef XKILL_RENDERER_DATASTREAMSRV_H
#define XKILL_RENDERER_DATASTREAMSRV_H

#include <d3d11.h>

#include "DataStreamBuffer.h"

template <class Data>
class DataStreamSRV : public DataStreamBuffer<Data>
{
public:
	DataStreamSRV(D3D11_BIND_FLAG bindFlag, D3D11_RESOURCE_MISC_FLAG miscFlag) : DataStreamBuffer(bindFlag, miscFlag)
	{
		dataStreamSRV_	= nullptr;
	}
	~DataStreamSRV()
	{
		SAFE_RELEASE(dataStreamSRV_);
	}

	HRESULT createStream(ID3D11Device* device)
	{
		HRESULT hr = S_OK;

		SAFE_RELEASE(dataStreamBuffer_);
		SAFE_RELEASE(dataStreamSRV_);

		hr = createBuffer(device);
		if(SUCCEEDED(hr))
			hr = createSRV(device);

		return hr;
	}

	HRESULT createSRV(ID3D11Device* device)
	{
		HRESULT hr = S_OK;

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));
		dataStreamBuffer_->GetDesc(&bufferDesc);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
		ZeroMemory(&srvDesc, sizeof(srvDesc));

		srvDesc.ViewDimension			= D3D11_SRV_DIMENSION_BUFFEREX;
		srvDesc.BufferEx.FirstElement	= 0;
		srvDesc.Format					= DXGI_FORMAT_UNKNOWN;
		srvDesc.BufferEx.NumElements	= bufferDesc.ByteWidth / bufferDesc.StructureByteStride;

		hr = device->CreateShaderResourceView(dataStreamBuffer_, &srvDesc, &dataStreamSRV_);
		if(FAILED(hr))
			ERROR_MESSAGEBOX("DataStreamSRV::createSRV device->CreateShaderResourceView failed!");

		SET_D3D_OBJECT_NAME(dataStreamSRV_, "dataStreamSRV");

		return hr;
	}

	void setStreamSRV(ID3D11DeviceContext* devcon, unsigned int shaderRegister)
	{
		devcon->CSSetShaderResources(
			shaderRegister, 
			1, 
			&dataStreamSRV_);
	}
	void unsetStreamSRV(ID3D11DeviceContext* devcon, unsigned int shaderRegister)
	{
		ID3D11ShaderResourceView* nullSRVs[1] = { NULL };
		devcon->CSSetShaderResources(
			shaderRegister, 
			1, 
			nullSRVs);
	}
protected:
private:
	ID3D11ShaderResourceView*	dataStreamSRV_;
};

#endif //XKILL_RENDERER_DATASTREAMSRV_H