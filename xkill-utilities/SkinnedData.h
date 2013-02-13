#ifndef XKILL_UTILITIES_SKINNEDDATA_H
#define XKILL_UTILITIES_SKINNEDDATA_H

#include <vector>
#include <map>
#include <string>

#include "dllUtilities.h"

namespace DirectX
{
	struct XMFLOAT4X4;
}

class AnimationClip;

//! Class containing skeleton information.
/*!
\ingroup xkill-renderer
*/
class DLL_U SkinnedData
{
public:
	//! Initializes SkinnedData to its default state.
	SkinnedData();
	//! Releases all memory and returns SkinnedData to its default state.
	~SkinnedData();

	//! Sets all of SkinnedData's member variables.
	/*!
	\param boneHierarchy A vector containing the hierarchy for a skeleton.
	\param boneOffsets A vector containing the offset for each bone in the skeleton.
	\param animations A map containing all animation clips for this skeleteon and thier corresponding names.
	*/
	void set(std::vector<int>*						boneHierarchy,
			  std::vector<DirectX::XMFLOAT4X4>*		boneOffsets,
			  std::map<std::string, AnimationClip*>* animations);
	//! Calculates the final transformation matrix for each bone for a specified animation.
	/*!
	\param clipName The name of the animation.
	\param timePosition At what point in the animation the character is.
	\param finalTransforms A vector where the resulting matrices will be stored.
	*/
	void getFinalTransforms(const std::string&					clipName,
							float								timePosition, 
							std::vector<DirectX::XMFLOAT4X4>*	finalTransforms) const;

	unsigned int getBoneCount()							const;
	float getClipStartTime(const std::string& clipName) const;
	float getClipEndTime(const std::string& clipName)	const;


	std::vector<int>*						getBoneHierarchy()	const;
	std::vector<DirectX::XMFLOAT4X4>*		getBoneOffsets()	const;
	std::map<std::string, AnimationClip*>*	getAnimations()		const;
private:
	std::vector<int>*						boneHierarchy_; //!< Vector containing the skeletons hierarchy.
	std::vector<DirectX::XMFLOAT4X4>*		boneOffsets_;	//!< Vector containing bone offsets.
	std::map<std::string, AnimationClip*>*	animations_;	//!< Map that holds AnimationClips and thier corresponding names.
};

#endif //XKILL_UTILITIES_SKINNEDDATA_H