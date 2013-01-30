#ifndef XKILL_RENDERER_DATASTREAMSHADER_H
#define XKILL_RENDERER_DATASTREAMSHADER_H

#include <d3d11.h>
#include <vector>

#include <xkill-utilities/EventManager.h>

#include "renderingUtilities.h"

template <class Data>
class DataStreamShader
{
public:
	DataStreamShader()
	{
		dataCountMax_ = 0;
		dataCountCur_ = 0;

		dataStreamBuffer_	= nullptr;
		dataStreamSRV_		= nullptr;
	}
	~DataStreamShader()
	{
		SAFE_RELEASE(dataStreamBuffer_);
		SAFE_RELEASE(dataStreamSRV_);
	}

	void resetStream()
	{
		dataCountCur_ = 0;
	}
	void pushData(Data d)
	{
		if(dataCountCur_ < data_.size())
		{
			data_.at(dataCountCur_) = d;
		}
		else
		{
			data_.push_back(d);
		}

		dataCountCur_++;
	}

	HRESULT updateDataStream(ID3D11Device* device, ID3D11DeviceContext* devcon)
	{
		HRESULT hr = S_OK;

		//Only update data stream if there is data in data-vector.
		if(dataCountCur_ > 0)
		{
			streamData(device, devcon);
		}

		return hr;
	}

	void setStreamSRV(ID3D11DeviceContext* devcon, unsigned int shaderRegister)
	{
		devcon->CSSetShaderResources(
			shaderRegister, 
			1, 
			&dataStreamSRV_);
	}

	//std::vector<Data>& getData() { return data_; }
	unsigned int getDataCountCur() { return dataCountCur_; }
	unsigned int getDataCountMax() { return dataCountMax_; }
protected:
private:
	HRESULT streamData(ID3D11Device* device, ID3D11DeviceContext* devcon)
	{
		HRESULT hr = S_OK;

		if(dataCountCur_ > dataCountMax_)
		{
			hr = increaseStreamCapacity(device); //Will be guaranteed to run the first time data is streamed.
		}
		
		if(SUCCEEDED(hr))
		{
			hr = mapBuffer(devcon);
		}

		return hr;
	}

	HRESULT increaseStreamCapacity(ID3D11Device* device)
	{
		HRESULT hr = S_OK;

		dataCountMax_ = getNewMaxCount(dataCountMax_, dataCountCur_);

		SAFE_RELEASE(dataStreamBuffer_);
		SAFE_RELEASE(dataStreamSRV_);
		hr = createStream(device);
		
		return hr;
	}
	HRESULT mapBuffer(ID3D11DeviceContext* devcon)
	{
		HRESULT hr = S_OK;

		D3D11_MAPPED_SUBRESOURCE map;
		hr = devcon->Map(
			dataStreamBuffer_, 
			NULL, 
			D3D11_MAP_WRITE_DISCARD, 
			NULL, 
			&map);
		if(SUCCEEDED(hr))
		{
			memcpy(map.pData, &data_[0], dataCountCur_ * sizeof(Data));
			devcon->Unmap(dataStreamBuffer_, NULL);
		}
		else
		{
			SHOW_MESSAGEBOX("DataStreamShader::mapBuffer devcon->Map failed!");
		}

		return hr;
	}

	HRESULT createStream(ID3D11Device* device)
	{
		HRESULT hr = S_OK;

		hr = createBuffer(device);
		if(SUCCEEDED(hr))
			hr = createSRV(device);

		return hr;
	}
	HRESULT createBuffer(ID3D11Device* device)
	{
		HRESULT hr = S_OK;

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));

		bufferDesc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
		bufferDesc.ByteWidth			= sizeof(Data) * dataCountMax_;
		bufferDesc.MiscFlags			= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		bufferDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
		bufferDesc.Usage				= D3D11_USAGE_DYNAMIC;
		bufferDesc.StructureByteStride	= sizeof(Data);

		D3D11_SUBRESOURCE_DATA initialData;
		initialData.pSysMem = &data_.at(0);

		hr = device->CreateBuffer(&bufferDesc, &initialData, &dataStreamBuffer_);
		if(FAILED(hr))
			SHOW_MESSAGEBOX("DataStreamShader::createBuffer device->CreateBuffer failed!");

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
			SHOW_MESSAGEBOX("DataStreamShader::createSRV device->CreateShaderResourceView failed!");

		return hr;
	}

	unsigned int getNewMaxCount(unsigned int maxCount, unsigned int targetMaxCount)
	{
		unsigned int newMaxCount = 1;
		if(maxCount > 0)
		{
			newMaxCount = maxCount;
		}

		while(newMaxCount < targetMaxCount)
		{
			newMaxCount *= 2;
		}

		return newMaxCount;
	}

	unsigned int dataCountCur_;
	unsigned int dataCountMax_;
	std::vector<Data>			data_;
	ID3D11Buffer*				dataStreamBuffer_;
	ID3D11ShaderResourceView*	dataStreamSRV_;
};

#endif //XKILL_RENDERER_DATASTREAMSHADER_H