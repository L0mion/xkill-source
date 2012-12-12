#ifndef XKILL_RENDERER_ANIMATIONCLIP_H
#define XKILL_RENDERER_ANIMATIONCLIP_H

#include <vector>

#include "BoneAnimation.h"

namespace DirectX
{
	struct XMFLOAT4X4;
}

class AnimationClip
{
public:
	AnimationClip();
	~AnimationClip();

	void interpolate(float time, std::vector<DirectX::XMFLOAT4X4>& boneTransforms) const;

	float getClipStartTime()	const;
	float getClipEndTime()		const;
	
	std::vector<BoneAnimation> boneAnimations_;
private:

};	

#endif //XKILL_RENDERER_ANIMATIONCLIP_H