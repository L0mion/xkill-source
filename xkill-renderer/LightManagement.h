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

namespace RendererLight
{

	class LightManagement : public D3DInterface
	{
	public:
		LightManagement();
		~LightManagement();
	
		virtual void reset();
	
		void init(ID3D11Device* device);

		void setLightUAVCS(ID3D11DeviceContext* devcon);
	private:
		void createDirectionalLight();

		HRESULT createLightBuffer(ID3D11Device* device);
		HRESULT createLightUAV(ID3D11Device* device);
		
		std::vector<LightDesc>		lights;
		ID3D11Buffer*				lightBuffer;
		ID3D11UnorderedAccessView*	lightUAV;
	};
}  //namespace RendererLight

#endif //XKILL_RENDERER_LIGHTMANAGEMENT_H