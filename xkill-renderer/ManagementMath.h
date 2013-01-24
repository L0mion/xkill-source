#ifndef XKILL_RENDERER_MANAGEMENTMATH_H
#define XKILL_RENDERER_MANAGEMENTMATH_H

namespace DirectX
{
	struct XMFLOAT4X4;
};

struct Attribute_Spatial;
struct Attribute_Position;

class ManagementMath
{
public:
	ManagementMath();
	~ManagementMath();

	DirectX::XMFLOAT4X4 calculateWorldMatrix(
		Attribute_Spatial*	spatialAttribute);
	DirectX::XMFLOAT4X4 calculateFinalMatrix(
		DirectX::XMFLOAT4X4 worldMatrix,
		DirectX::XMFLOAT4X4 viewMatrix,
		DirectX::XMFLOAT4X4 projectionMatrix);
	DirectX::XMFLOAT4X4 calculateMatrixInverse(DirectX::XMFLOAT4X4 matrix);
protected:
private:
};

#endif //XKILL_RENDERER_MANAGEMENTMATH_H