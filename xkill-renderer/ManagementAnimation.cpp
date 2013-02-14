
#include <DirectXMath.h>

#include "ManagementAnimation.h"
#include "renderingUtilities.h"

ManagementAnimation::ManagementAnimation()
{
	time = 0.0f;
}
ManagementAnimation::~ManagementAnimation()
{
	for(unsigned int i=0; i<animations_.size(); i++)
		SAFE_DELETE(animations_[i]);
}

void ManagementAnimation::reset()
{
}

SkinnedData* ManagementAnimation::getAnimation(unsigned int index)
{
	time += 0.01f;
	if(time > 10.0f)
		time = 0.0f;

	SkinnedData* skinnedData = nullptr;
	if(index < animations_.size())
		skinnedData = animations_.at(index);

	return skinnedData;
}

void ManagementAnimation::addAnimation(SkinnedData* skinnedData)
{
	animations_.push_back(skinnedData);
}
void ManagementAnimation::debug_clearOffsetMatrices(unsigned int index)
{
	for(unsigned int i=0; i<animations_[index]->getBoneOffsets()->size(); i++)
		animations_[index]->getBoneOffsets()->at(i) = DirectX::XMFLOAT4X4(1.0f, 0.0f, 0.0f, 0.0f,
																		  0.0f, 1.0f, 0.0f, 0.0f,
																		  0.0f, 0.0f, 1.0f, 0.0f,
																		  0.0f, 0.0f, 0.0f, 1.0f);
}