#include <d3d11.h>
#include "renderingUtilities.h"
#include "TimerDX.h"

void TimerDX::init(ID3D11Device* device)
{
	D3D11_QUERY_DESC desc;
	desc.Query	= D3D11_QUERY_TIMESTAMP_DISJOINT;
	desc.MiscFlags	= 0;
	device->CreateQuery(&desc, &disjoint);

	desc.Query = D3D11_QUERY_TIMESTAMP;
	device->CreateQuery(&desc, &start);
	device->CreateQuery(&desc, &stop);
}

void TimerDX::startTimer(ID3D11DeviceContext*	deviceContext)
{
	deviceContext->Begin(disjoint);
	deviceContext->End(start);
}
void TimerDX::stopTimer(ID3D11DeviceContext*	deviceContext)
{
	deviceContext->End(stop);
	deviceContext->End(disjoint);
}

double TimerDX::Time(ID3D11DeviceContext* deviceContext)
{
	UINT64 startTime = 0;
	while(deviceContext->GetData(start, &startTime, sizeof(startTime), 0) != S_OK);

	UINT64 endTime = 0;
	while(deviceContext->GetData(stop, &endTime, sizeof(endTime), 0) != S_OK);

	D3D11_QUERY_DATA_TIMESTAMP_DISJOINT disjointData;
	while(deviceContext->GetData(disjoint, &disjointData, sizeof(disjointData), 0) != S_OK);

	double time = -1.0f;
	if(disjointData.Disjoint == FALSE)
	{
		UINT64 delta = endTime - startTime;
		double frequency = static_cast<double>(disjointData.Frequency);
		time = (delta / frequency) * 1000.0f;
	}

	return time;
}

TimerDX::TimerDX()
{
	start	= nullptr;
	stop	= nullptr;
	disjoint	= nullptr;
}
TimerDX::~TimerDX()
{
	SAFE_RELEASE(start);
	SAFE_RELEASE(stop);
	SAFE_RELEASE(disjoint);
}