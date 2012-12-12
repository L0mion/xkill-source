#ifndef XKILL_RENDERER_BONEANIMATION_H
#define XKILL_RENDERER_BONEANIMATION_H

#include <vector>

namespace DirectX
{
	struct XMFLOAT4X4;
}

struct Keyframe;

class BoneAnimation
{
public:
	BoneAnimation();
	~BoneAnimation();

	void interpolate(float time, DirectX::XMFLOAT4X4& matrix) const;

	float getStartTime()	const;
	float getEndTime()		const;

	std::vector<Keyframe*>* getKeyframes() const;
private:
	
	void keyframeFirst(DirectX::XMFLOAT4X4& matrix)						const;
	void keyframeLast(DirectX::XMFLOAT4X4& matrix)						const;
	void keyframeInterpolate(float time, DirectX::XMFLOAT4X4& matrix)	const; 	

	std::vector<Keyframe*>* keyframes_;
};

#endif //XKILL_RENDERER_BONEANIMATION_H