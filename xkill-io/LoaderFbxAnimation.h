#ifndef XKILL_IO_LOADERFBXANIMATION_H
#define XKILL_IO_LOADERFBXANIMATION_H

#include <vector>

#include "LoaderFbxAnimationDesc.h"
#include "LoaderFbxAnimationKeyFrame.h"

namespace fbxsdk_2014_0_beta2
{
	class FbxScene;
	class FbxNode;
	class FbxNodeAttribute;
	class FbxAnimStack;
	class FbxAnimLayer;
	class FbxAnimCurve;
	class FbxAnimCurveNode;
	class FbxProperty;
	class FbxCluster;
}

class LoaderFbxAnimation
{
public:
	LoaderFbxAnimation();
	~LoaderFbxAnimation();
	void reset();

	void parseAnimation(FbxScene* scene, std::vector<LoaderFbxAnimationDesc>* animationDescs);
private:
	void parseAnimationStack(FbxAnimStack* animStack, FbxNode* node, bool isSwitcher);
	void parseAnimationLayer(FbxAnimLayer* animLayer, FbxNode* node, bool isSwitcher);
	
	void parseAnimationChannels(FbxNode* node, FbxAnimLayer* animLayer, bool isSwitcher);
	void parseAnimationChannelsGeneral(FbxNode* node, FbxAnimLayer* animLayer, FbxAnimCurve* animCurve);
	void parseAnimationChannelsGeneralTranslation(FbxNode* node, FbxAnimLayer* animLayer, FbxAnimCurve* animCurve);
	void parseAnimationChannelsGeneralRotation(FbxNode* node, FbxAnimLayer* animLayer, FbxAnimCurve* animCurve);
	void parseAnimationChannelsGeneralScaling(FbxNode* node, FbxAnimLayer* animLayer, FbxAnimCurve* animCurve);
	void parseAnimationChannelsColor(FbxNodeAttribute* nodeAttribute, FbxAnimLayer* animLayer, FbxAnimCurve* animCurve);
	void parseAnimationChannelsLamp(FbxNode* node, FbxAnimLayer* animLayer, FbxAnimCurve* animCurve);
	void parseAnimationChannelsCamera(FbxNode* node, FbxAnimLayer* animLayer, FbxAnimCurve* animCurve);
	void parseAnimationChannelsGeometry(FbxNodeAttribute* nodeAttribute, FbxAnimLayer* animLayer, FbxAnimCurve* animCurve);
	void parseAnimationChannelsProperty(FbxNode* node, FbxAnimLayer* animLayer, FbxAnimCurve* animCurve);
	void parseAnimationChannelsPropertyDataType(FbxAnimCurveNode* curveNode, FbxAnimCurve* animCurve, FbxProperty* fbxProperty);
	void parseAnimationChannelsPropertyPrimitive(FbxAnimCurveNode* curveNode, FbxAnimCurve* animCurve);
	void parseAnimationChannelsPropertyVector(FbxAnimCurveNode* curveNode, FbxAnimCurve* animCurve);
	void parseAnimationChannelsPropertyEnum(FbxAnimCurveNode* curveNode, FbxAnimCurve* animCurve, FbxProperty* fbxProperty);

	void parseAnimationCurve(FbxAnimCurve* animCurve, std::vector<LoaderFbxAnimationKeyFrame>* keyFrames);
	void parseAnimationListCurve(FbxAnimCurve* animCurve, FbxProperty* fbxProperty, std::vector<LoaderFbxAnimationKeyFrame>* keyFrames);

	void removeNonAffectingBones();
	std::vector<LoaderFbxAnimationBone> animationBones_;
};

#endif //XKILL_IO_LOADERFBXANIMATION_H