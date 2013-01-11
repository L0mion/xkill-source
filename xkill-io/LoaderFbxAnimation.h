#ifndef XKILL_IO_LOADERFBXANIMATION_H
#define XKILL_IO_LOADERFBXANIMATION_H

#include <fbxsdk.h>

class LoaderFbxAnimation
{
public:
	LoaderFbxAnimation();
	~LoaderFbxAnimation();
	void reset();

	void parseAnimation(FbxScene* scene);

private:
	void parseAnimationStack(FbxAnimStack* animStack, FbxNode* node, bool isSwitcher);
	void parseAnimationLayer(FbxAnimLayer* animLayer, FbxNode* node, bool isSwitcher);
	void parseAnimationChannels(FbxNode* node, FbxAnimLayer* animLayer, bool isSwitcher);
	void parseAnimationCurve(FbxAnimCurve* animCurve);
};

#endif //XKILL_IO_LOADERFBXANIMATION_H