#ifndef XKILL_RENDERER_MANAGEMENT_ANIMATION_H
#define XKILL_RENDERER_MANAGEMENT_ANIMATION_H

#include <vector>

#include <xkill-utilities/SkinnedData.h>

#include "d3dInterface.h"


class ManagementAnimation : public D3DInterface
{
private:
	float timePosition;

public:
	ManagementAnimation();
	virtual ~ManagementAnimation();
	virtual void reset();
	float getTimePosition(){return timePosition;}

	void addAnimation(SkinnedData* skinnedData);

	void update(float delta, std::string clipName, int animationIndex);
	SkinnedData* getAnimation(unsigned int index);

	void debug_clearOffsetMatrices(unsigned int index);

private:

	std::vector<SkinnedData*> animations_;
};

#endif //XKILL_RENDERER_MANAGEMENT_ANIMATION_H