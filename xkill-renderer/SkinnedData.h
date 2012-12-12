#ifndef XKILL_RENDERER_SKINNEDDATA_H
#define XKILL_RENDERER_SKINNEDDATA_H

#include <vector>
#include <map>
#include <string>

namespace DirectX
{
	struct XMFLOAT4X4;
}

class AnimationClip;

class SkinnedData
{
public:
	SkinnedData();
	~SkinnedData();

	void set(std::vector<int>*						boneHierarchy,
			  std::vector<DirectX::XMFLOAT4X4>*		boneOffsets,
			  std::map<std::string, AnimationClip*>* animations);

	void getFinalTransforms(const std::string&					clipName,
							float								timePosition, 
							std::vector<DirectX::XMFLOAT4X4>*	finalTransforms) const;

	float getBoneCount()								const;
	float getClipStartTime(const std::string& clipName) const;
	float getClipEndTime(const std::string& clipName)	const;

private:
	std::vector<int>*						boneHierarchy_;
	std::vector<DirectX::XMFLOAT4X4>*		boneOffsets_;
	std::map<std::string, AnimationClip*>*	animations_;
};

#endif //XKILL_RENDERER_SKINNEDDATA_H