#ifndef XKILL_RENDERER_BONEANIMATION_H
#define XKILL_RENDERER_BONEANIMATION_H

#include <vector>

#include "Keyframe.h"

namespace DirectX
{
	struct XMFLOAT4X4;
}

class BoneAnimation
{
public:
	BoneAnimation();
	~BoneAnimation();

	void interpolate(float time, DirectX::XMFLOAT4X4& matrix) const;

	float getStartTime()	const;
	float getEndTime()		const;

	std::vector<Keyframe> keyframes_;
private:
	
	void keyframeFirst(DirectX::XMFLOAT4X4& matrix)						const;
	void keyframeLast(DirectX::XMFLOAT4X4& matrix)						const;
	void keyframeInterpolate(float time, DirectX::XMFLOAT4X4& matrix)	const; 	

};

#endif //XKILL_RENDERER_BONEANIMATION_H