#ifndef XKILL_RENDERER_MANAGEMENTMATH_H
#define XKILL_RENDERER_MANAGEMENTMATH_H

namespace DirectX
{
	struct XMFLOAT4X4;
};

struct SpatialAttribute;
struct PositionAttribute;

class ManagementMath
{
public:
	ManagementMath();
	~ManagementMath();

	DirectX::XMFLOAT4X4 calculateWorldMatrix(
		SpatialAttribute*	spatialAttribute,							 
		PositionAttribute*	positionAttribute);
	DirectX::XMFLOAT4X4 calculateFinalMatrix(
		DirectX::XMFLOAT4X4 worldMatrix,
		DirectX::XMFLOAT4X4 viewMatrix,
		DirectX::XMFLOAT4X4 projectionMatrix);
	DirectX::XMFLOAT4X4 calculateMatrixInverse(DirectX::XMFLOAT4X4 matrix);
protected:
private:
};

#endif //XKILL_RENDERER_MANAGEMENTMATH_H