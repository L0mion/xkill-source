#ifndef XKILL_UTILITIES_BONEANIMATION_H
#define XKILL_UTILITIES_BONEANIMATION_H

#include <vector>

#include "dllUtilities.h"

namespace DirectX
{
	struct XMFLOAT4X4;
}

struct Keyframe;

//! Class representing a single bone in a skeleton.
/*!
\ingroup xkill-renderer
*/
class DLL_U BoneAnimation
{
public:
	//! Initializes BoneAnimation to its default state.
	BoneAnimation();
	//! Releases all memory and resets BoneAnimation to its default state.
	~BoneAnimation();

	void addKeyframe(Keyframe* keyframe);

	//! Interpolates the animation on this bone.
	/*!
	\param time At which point in the animation the character is currently in.
	\param matrix A matrix where the result will be stored.
	*/
	void interpolate(float time, DirectX::XMFLOAT4X4& matrix) const;

	float getStartTime()	const;
	float getEndTime()		const;

	std::vector<Keyframe*>* getKeyframes() const;
private:
	
	//! Sets the first keyframe
	/*!
	\param matrix Matrix that will store the result
	*/
	void keyframeFirst(DirectX::XMFLOAT4X4& matrix)						const; 
	//! Sets the last keyframe
	/*!
	\param matrix Matrix that will store the result
	*/
	void keyframeLast(DirectX::XMFLOAT4X4& matrix)						const; 
	//! Interpolates between two keyframes.
	/*!
	\param time At which point in the animation the character is.
	\param matrix Matrix that will store the result
	*/
	void keyframeInterpolate(float time, DirectX::XMFLOAT4X4& matrix)	const; 

	std::vector<Keyframe*>* keyframes_; //!< Vectvor containing the keyframes for this bone.
};

#endif //XKILL_UTILITIES_BONEANIMATION_H