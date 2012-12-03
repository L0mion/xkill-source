#ifndef XKILL_RENDERER_LIGHTDESC_H
#define XKILL_RENDERER_LIGHTDESC_H

#include <DirectXMath.h>


static const unsigned int LIGHT_DIRECTIONAL = 1;
static const unsigned int LIGHT_POINT		= 2;
static const unsigned int LIGHT_SPOT		= 3;

struct LightDesc
{
	DirectX::XMFLOAT3 position;
	float range;
	DirectX::XMFLOAT3 direction;
	float spotPower;
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT4 specular;
	DirectX::XMFLOAT3 attenuation;
	unsigned int type;

	LightDesc()
	{
		ZeroMemory(this, sizeof(LightDesc));
	}
};

#endif //XKILL_RENDERER_LIGHTDESC_H