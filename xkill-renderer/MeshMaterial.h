#ifndef XKILL_RENDERER_MESHMATERIAL_H
#define XKILL_RENDERER_MESHMATERIAL_H

#include <DirectXMath.h>

class MeshMaterial
{
public:
	MeshMaterial();
	MeshMaterial(
		const DirectX::XMFLOAT3	mmAmbientTerm,
		const DirectX::XMFLOAT3	mmDiffuseTerm,
		const DirectX::XMFLOAT3	mmSpecularTerm,
		const float				mmSpecularPower,
		const DirectX::XMFLOAT3	mmReflectivity,
		const bool				mmTransperency);
	~MeshMaterial();

	const DirectX::XMFLOAT3	getMMAmbientTerm();
	const DirectX::XMFLOAT3	getMMDiffuseTerm();
	const DirectX::XMFLOAT3	getMMSpecularTerm();
	const float				getMMSpecularPower();
	const DirectX::XMFLOAT3	getMMReflectivity();
	const bool				getMMTransperency();
protected:
private:
	DirectX::XMFLOAT3	mmAmbientTerm_;
	DirectX::XMFLOAT3	mmDiffuseTerm_;
	DirectX::XMFLOAT3	mmSpecularTerm_;
	float				mmSpecularPower_;
	DirectX::XMFLOAT3	mmReflectivity_;
	bool				mmTransperency_;
};

#endif //XKILL_RENDERER_MESHMATERIAL_H