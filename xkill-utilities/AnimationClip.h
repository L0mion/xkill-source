#ifndef XKILL_UTILITIES_ANIMATIONCLIP_H
#define XKILL_UTILITIES_ANIMATIONCLIP_H

#include <vector>

#include "dllUtilities.h"

namespace DirectX
{
	struct XMFLOAT4X4;
}

class BoneAnimation;

//! Class containing an animation.
/*!
\ingroup xkill-renderer
*/
class DLL_U AnimationClip
{
public:
	//! Initializes AnimationClip to its default state.
	AnimationClip();
	//! Releases all memory and resets AnimationClip to its default state.
	~AnimationClip();

	//! Interpolates the animation
	/*!
	\param time At what point in the animation the character is.
	\param boneTransforms Vector where the results will be stored.
	*/
	void interpolate(float time, std::vector<DirectX::XMFLOAT4X4>& boneTransforms) const;

	float getClipStartTime()	const;
	float getClipEndTime()		const;
	
	std::vector<BoneAnimation*>* getBoneAnimations() const;
	void setBoneAnimations(std::vector<BoneAnimation*>* boneAnimations);

private:
	std::vector<BoneAnimation*>* boneAnimations_; //!< Vector containing the skeleton
};	

#endif //XKILL_UTILITIES_ANIMATIONCLIP_H