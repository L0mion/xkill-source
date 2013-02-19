
#include <DirectXMath.h>

#include "ManagementAnimation.h"
#include "renderingUtilities.h"

ManagementAnimation::ManagementAnimation()
{
	timePosition = 0.0f;
}
ManagementAnimation::~ManagementAnimation()
{
	for(unsigned int i=0; i<animations_.size(); i++)
		SAFE_DELETE(animations_[i]);
}

void ManagementAnimation::reset()
{
}

void ManagementAnimation::update(float delta, std::string clipName)
{
	timePosition += delta;

	int nrOfSkinnedData = animations_.size();
	for(unsigned int i=0; i<nrOfSkinnedData; i++)
	{
		SkinnedData* skinnedData = animations_.at(i);
		if(timePosition > skinnedData->getClipEndTime(clipName))
		{
			timePosition = 0.0f;
		}
	}

	/*
	timePosition += 0.01f;
	if(timePosition > 10.0f)
		timePosition = 0.0f;
	*/
}

SkinnedData* ManagementAnimation::getAnimation(unsigned int index)
{
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