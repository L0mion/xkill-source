#ifndef XKILL_RENDERER_DATASTREAMBUFFER_H
#define XKILL_RENDERER_DATASTREAMBUFFER_H

#include <d3d11.h>
#include <vector>

#include <xkill-utilities/Util.h>

#include "renderingUtilities.h"

template <class Data>
class DataStreamBuffer
{
public:
	DataStreamBuffer(unsigned int bindFlag, unsigned int miscFlag)
	{
		bindFlag_ = bindFlag;
		miscFlag_ = miscFlag;

		dataCountMax_ = 0;
		dataCountCur_ = 0;

		dataStreamBuffer_	= nullptr;
	}
	virtual ~DataStreamBuffer()
	{
		SAFE_RELEASE(dataStreamBuffer_);
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

	unsigned int getDataCountCur() { return dataCountCur_; }
	unsigned int getDataCountMax() { return dataCountMax_; }
	ID3D11Buffer* getDataBuffer()	{ return dataStreamBuffer_; }
protected:
	unsigned int getNewMaxCount(unsigned int maxCount, unsigned int targetMaxCount)
	{
		if(targetMaxCount < 1)
		{
			ERROR_MESSAGEBOX("DataStreamBuffer::getNewMaxCount targetMaxCount too small!");
		}

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
			NULL, //D3D11_MAP_FLAG_DO_NOT_WAIT, //
			&map);
		if(SUCCEEDED(hr))
		{
			memcpy(map.pData, &data_[0], dataCountCur_ * sizeof(Data));
			devcon->Unmap(dataStreamBuffer_, NULL);
		}
		else
		{
			ERROR_MESSAGEBOX("DataStreamShader::mapBuffer devcon->Map failed!");
		}

		return hr;
	}

	virtual HRESULT createStream(ID3D11Device* device)
	{
		HRESULT hr = S_OK;

		SAFE_RELEASE(dataStreamBuffer_);
		hr = createBuffer(device);

		return hr;
	}
	HRESULT createBuffer(ID3D11Device* device)
	{
		HRESULT hr = S_OK;

		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));

		bufferDesc.BindFlags			= bindFlag_; 
		bufferDesc.ByteWidth			= sizeof(Data) * dataCountMax_;
		bufferDesc.MiscFlags			= miscFlag_;
		bufferDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
		bufferDesc.Usage				= D3D11_USAGE_DYNAMIC;
		bufferDesc.StructureByteStride	= sizeof(Data);

		hr = device->CreateBuffer(
			&bufferDesc, 
			NULL, 
			&dataStreamBuffer_);
		if(FAILED(hr))
			ERROR_MESSAGEBOX("DataStreambuffer::createBuffer device->CreateBuffer failed!");

		SET_D3D_OBJECT_NAME(dataStreamBuffer_, "dataStreamBuffer");

		return hr;
	}

	unsigned int dataCountCur_;
	unsigned int dataCountMax_;
	std::vector<Data>			data_;
	ID3D11Buffer*				dataStreamBuffer_;

	//Desc
	unsigned int bindFlag_;
	unsigned int miscFlag_;
private:
};

#endif //XKILL_RENDERER_DATASTREAMBUFFER_H