#include "LoaderFbxAnimation.h"


LoaderFbxAnimation::LoaderFbxAnimation()
{
}
LoaderFbxAnimation::~LoaderFbxAnimation()
{
}
void LoaderFbxAnimation::reset()
{
}

void LoaderFbxAnimation::parseAnimation(FbxScene* scene)
{
	for(int i=0; i<scene->GetSrcObjectCount<FbxAnimStack>(); i++)
	{
		FbxAnimStack* animStack = scene->GetSrcObject<FbxAnimStack>(i);
	
		parseAnimationStack(animStack, scene->GetRootNode(), true);
		parseAnimationStack(animStack, scene->GetRootNode(), false);
	}
}
void LoaderFbxAnimation::parseAnimationStack(FbxAnimStack* animStack, FbxNode* node, bool isSwitcher)
{
	int numAnimationLayers = animStack->GetMemberCount<FbxAnimLayer>();
	
	for(int i=0; i<numAnimationLayers; i++)
	{
		FbxAnimLayer* animLayer = animStack->GetMember<FbxAnimLayer>(i);
		parseAnimationLayer(animLayer, node, isSwitcher);
	}
}
void LoaderFbxAnimation::parseAnimationLayer(FbxAnimLayer* animLayer, FbxNode* node, bool isSwitcher)
{
	parseAnimationChannels(node, animLayer, isSwitcher);

	for(int i=0; i< node->GetChildCount(); i++)
		parseAnimationLayer(animLayer, node->GetChild(i), isSwitcher);
}
void LoaderFbxAnimation::parseAnimationChannels(FbxNode* node, FbxAnimLayer* animLayer, bool isSwitcher)
{
	FbxAnimCurve* animCurve = nullptr;

	if(!isSwitcher)
	{
		animCurve = node->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
		if(animCurve)
		{
			printf("    TX:\n");
			parseAnimationCurve(animCurve);
		}
		animCurve = node->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
		if(animCurve)
		{
			printf("    TY:\n");
			parseAnimationCurve(animCurve);
		}
		animCurve = node->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
		if(animCurve)
		{
			printf("    TZ:\n");
			parseAnimationCurve(animCurve);
		}
	}
}
void LoaderFbxAnimation::parseAnimationCurve(FbxAnimCurve* animCurve)
{
	FbxTime keyTime;
	float	keyValue;

	int hour, minute, second, frame, field, residual;

	for(int i=0; i<animCurve->KeyGetCount(); i++)
	{
		keyValue = static_cast<float>(animCurve->KeyGetValue(i));
		keyTime	 = animCurve->KeyGetTime(i);

		FbxString outStr;
		char timeString[256];
		outStr = "            Key Time: ";
        outStr += keyTime.GetTimeString(timeString, FbxUShort(256));
        outStr += ".... Key Value: ";
        outStr += keyValue;

		printf("AnimationCurve:  %s \n", outStr.Buffer());
	}
}