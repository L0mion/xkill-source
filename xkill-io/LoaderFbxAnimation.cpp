#include <fbxsdk.h>

#include "LoaderFbxAnimation.h"
#include "LoaderFbxMeshDesc.h"


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
	
		FbxString str = "Animation Stack Name: ";
		str += animStack->GetName();
		str += "\n";
		printf("%s", str.Buffer());

		parseAnimationStack(animStack, scene->GetRootNode(), true);
		parseAnimationStack(animStack, scene->GetRootNode(), false);
	}
}
void LoaderFbxAnimation::parseAnimationStack(FbxAnimStack* animStack, FbxNode* node, bool isSwitcher)
{
	int numAnimationLayers = animStack->GetMemberCount<FbxAnimLayer>();
	
	FbxString str = "Animation stack contains ";
	str += numAnimationLayers;
	str += " Animation Layer(s)\n";
	printf(str.Buffer());

	for(int i=0; i<numAnimationLayers; i++)
	{
		FbxAnimLayer* animLayer = animStack->GetMember<FbxAnimLayer>(i);

		str = "AnimLayer ";
		str += i;
		str += ": ";
		str += animLayer->GetName();
		str += "\n";
		printf(str.Buffer());

		parseAnimationLayer(animLayer, node, isSwitcher);
	}
}
void LoaderFbxAnimation::parseAnimationLayer(FbxAnimLayer* animLayer, FbxNode* node, bool isSwitcher)
{
	FbxString str = "Node name: ";
	str += node->GetName();
	str += "\n";
	printf(str.Buffer());

	parseAnimationChannels(node, animLayer, isSwitcher);

	for(int i=0; i< node->GetChildCount(); i++)
		parseAnimationLayer(animLayer, node->GetChild(i), isSwitcher);
}

void LoaderFbxAnimation::parseAnimationChannels(FbxNode* node, FbxAnimLayer* animLayer, bool isSwitcher)
{
	FbxAnimCurve* animCurve = nullptr;

	if(!isSwitcher)
		parseAnimationChannelsGeneral(node, animLayer, animCurve);
		
	FbxNodeAttribute* nodeAttribute = node->GetNodeAttribute();
	
	parseAnimationChannelsColor(nodeAttribute, animLayer, animCurve);
	parseAnimationChannelsLamp(node, animLayer, animCurve);
	parseAnimationChannelsCamera(node, animLayer, animCurve);
	parseAnimationChannelsGeometry(nodeAttribute, animLayer, animCurve);
	parseAnimationChannelsProperty(node, animLayer, animCurve);
}

void LoaderFbxAnimation::parseAnimationChannelsGeneral(FbxNode* node, FbxAnimLayer* animLayer, FbxAnimCurve* animCurve)
{
	parseAnimationChannelsGeneralTranslation(node, animLayer, animCurve);
	parseAnimationChannelsGeneralRotation(node, animLayer, animCurve);
	parseAnimationChannelsGeneralScaling(node, animLayer, animCurve);
}
void LoaderFbxAnimation::parseAnimationChannelsGeneralTranslation(FbxNode* node, FbxAnimLayer* animLayer, FbxAnimCurve* animCurve)
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
void LoaderFbxAnimation::parseAnimationChannelsGeneralRotation(FbxNode* node, FbxAnimLayer* animLayer, FbxAnimCurve* animCurve)
{
	animCurve = node->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
	if(animCurve)
	{
		printf("    RX:\n");
		parseAnimationCurve(animCurve);
	}
	animCurve = node->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
	if(animCurve)
	{
		printf("    RY:\n");
		parseAnimationCurve(animCurve);
	}
	animCurve = node->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
	if(animCurve)
	{
		printf("    RZ:\n");
		parseAnimationCurve(animCurve);
	}
}
void LoaderFbxAnimation::parseAnimationChannelsGeneralScaling(FbxNode* node, FbxAnimLayer* animLayer, FbxAnimCurve* animCurve)
{
	animCurve = node->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
	if(animCurve)
	{
		printf("    SX:\n");
		parseAnimationCurve(animCurve);
	}
	animCurve = node->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
	if(animCurve)
	{
		printf("    SY:\n");
		parseAnimationCurve(animCurve);
	}
	animCurve = node->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
	if(animCurve)
	{
		printf("    SZ:\n");
		parseAnimationCurve(animCurve);
	}
}

void LoaderFbxAnimation::parseAnimationChannelsColor(FbxNodeAttribute* nodeAttribute, FbxAnimLayer* animLayer, FbxAnimCurve* animCurve)
{
	if(nodeAttribute)
	{
		animCurve = nodeAttribute->Color.GetCurve(animLayer, FBXSDK_CURVENODE_COLOR_RED);
		if(animCurve)
		{
			parseAnimationCurve(animCurve);
		}
		animCurve = nodeAttribute->Color.GetCurve(animLayer, FBXSDK_CURVENODE_COLOR_GREEN);
		if(animCurve)
		{
			parseAnimationCurve(animCurve);
		}
		animCurve = nodeAttribute->Color.GetCurve(animLayer, FBXSDK_CURVENODE_COLOR_BLUE);
		if(animCurve)
		{
			parseAnimationCurve(animCurve);
		}
	}
}

void LoaderFbxAnimation::parseAnimationChannelsLamp(FbxNode* node, FbxAnimLayer* animLayer, FbxAnimCurve* animCurve)
{
	FbxLight* light = node->GetLight();
	if(light)
	{
		animCurve = light->Intensity.GetCurve(animLayer);
		if(animCurve)
		{
			parseAnimationCurve(animCurve);
		}
		animCurve = light->OuterAngle.GetCurve(animLayer);
		if(animCurve)
		{
			parseAnimationCurve(animCurve);
		}
		animCurve = light->Fog.GetCurve(animLayer);
		if(animCurve)
		{
			parseAnimationCurve(animCurve);
		}
	}
}

void LoaderFbxAnimation::parseAnimationChannelsCamera(FbxNode* node, FbxAnimLayer* animLayer, FbxAnimCurve* animCurve)
{
	FbxCamera* camera = node->GetCamera();
	if(camera)
	{
		animCurve = camera->FieldOfView.GetCurve(animLayer);
		if(animCurve)
		{
			parseAnimationCurve(animCurve);
		}
		animCurve = camera->FieldOfViewX.GetCurve(animLayer);
		if(animCurve)
		{
			parseAnimationCurve(animCurve);
		}
		animCurve = camera->FieldOfViewY.GetCurve(animLayer);
		if(animCurve)
		{
			parseAnimationCurve(animCurve);
		}
		animCurve = camera->OpticalCenterX.GetCurve(animLayer);
		if(animCurve)
		{
			parseAnimationCurve(animCurve);
		}
		animCurve = camera->OpticalCenterY.GetCurve(animLayer);
		if(animCurve)
		{
			parseAnimationCurve(animCurve);
		}
		animCurve = camera->Roll.GetCurve(animLayer);
		if(animCurve)
		{
			parseAnimationCurve(animCurve);
		}
	}
}

void LoaderFbxAnimation::parseAnimationChannelsGeometry(FbxNodeAttribute* nodeAttribute, FbxAnimLayer* animLayer, FbxAnimCurve* animCurve)
{
	if(nodeAttribute)
	{
		if( nodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh ||
			nodeAttribute->GetAttributeType() == FbxNodeAttribute::eNurbs ||
			nodeAttribute->GetAttributeType() == FbxNodeAttribute::ePatch)
		{
			FbxGeometry* geometry = static_cast<FbxGeometry*>(nodeAttribute);
			
			int blendShapeDeformerCount = geometry->GetDeformerCount(FbxDeformer::eBlendShape);
			for(int blendShapeIndex=0; blendShapeIndex<blendShapeDeformerCount; blendShapeIndex++)
			{
				FbxBlendShape* blendShape = static_cast<FbxBlendShape*>(geometry->GetDeformer(blendShapeIndex, FbxDeformer::eBlendShape));

				int blendShapeChannelCount = blendShape->GetBlendShapeChannelCount();
				for(int channelIndex=0; channelIndex<blendShapeChannelCount; channelIndex++)
				{
					FbxBlendShapeChannel* channel = blendShape->GetBlendShapeChannel(channelIndex);

					animCurve = geometry->GetShapeChannel(blendShapeIndex, channelIndex, animLayer, true);
					if(animCurve)
					{
						parseAnimationCurve(animCurve);
					}
				}
			}
		}
	}
}

void LoaderFbxAnimation::parseAnimationChannelsProperty(FbxNode* node, FbxAnimLayer* animLayer, FbxAnimCurve* animCurve)
{
	FbxProperty fbxProperty = node->GetFirstProperty();
	while(fbxProperty.IsValid())
	{
		if(fbxProperty.GetFlag(FbxPropertyAttr::eUser))
		{
			FbxAnimCurveNode* curveNode = fbxProperty.GetCurveNode(animLayer);

			if(!curveNode)
			{
				fbxProperty = node->GetNextProperty(fbxProperty);
			}
			else
			{
				parseAnimationChannelsPropertyDataType(curveNode, animCurve, &fbxProperty);
			}
		}

		fbxProperty = node->GetNextProperty(fbxProperty);
	}
}
void LoaderFbxAnimation::parseAnimationChannelsPropertyDataType(FbxAnimCurveNode* curveNode, FbxAnimCurve* animCurve, FbxProperty* fbxProperty)
{
	FbxDataType dataType = fbxProperty->GetPropertyDataType();

	if(dataType.GetType() == eFbxBool || dataType.GetType() == eFbxDouble || dataType.GetType() == eFbxFloat || dataType.GetType() == eFbxInt)
	{
		parseAnimationChannelsPropertyPrimitive(curveNode, animCurve);
	}
	else if(dataType.GetType() == eFbxDouble3 || dataType.GetType() == eFbxDouble4 || dataType.Is(FbxColor3DT) || dataType.Is(FbxColor4DT))
	{
		parseAnimationChannelsPropertyVector(curveNode, animCurve);
	}
	else if(dataType.GetType() == eFbxEnum)
	{
		parseAnimationChannelsPropertyEnum(curveNode, animCurve, fbxProperty);
	}
}
void LoaderFbxAnimation::parseAnimationChannelsPropertyPrimitive(FbxAnimCurveNode* curveNode, FbxAnimCurve* animCurve)
{
	for(int i=0; i<curveNode->GetCurveCount(0U); i++)
	{
		animCurve = curveNode->GetCurve(0U, i);
		if(animCurve)
			parseAnimationCurve(animCurve);
	}
}
void LoaderFbxAnimation::parseAnimationChannelsPropertyVector(FbxAnimCurveNode* curveNode, FbxAnimCurve* animCurve)
{
	for(int i=0; i<curveNode->GetCurveCount(0U); i++)
	{
		animCurve = curveNode->GetCurve(0U, i);
		if(animCurve)
			parseAnimationCurve(animCurve);
	}
	for(int i=0; i<curveNode->GetCurveCount(1U); i++)
	{
		animCurve = curveNode->GetCurve(1U, i);
		if(animCurve)
			parseAnimationCurve(animCurve);
	}
	for(int i=0; i<curveNode->GetCurveCount(2U); i++)
	{
		animCurve = curveNode->GetCurve(2U, i);
		if(animCurve)
			parseAnimationCurve(animCurve);
	}
}
void LoaderFbxAnimation::parseAnimationChannelsPropertyEnum(FbxAnimCurveNode* curveNode, FbxAnimCurve* animCurve, FbxProperty* fbxProperty)
{
	 for(int i=0; i<curveNode->GetCurveCount(0U); i++)
	 {
	 	 animCurve = curveNode->GetCurve(0U, i);
	 	 if(animCurve)
	 	 {
	 		 parseAnimationListCurve(animCurve, fbxProperty);
	 	 }
	 }
}

void LoaderFbxAnimation::parseAnimationCurve(FbxAnimCurve* animCurve)
{
	FbxAnimCurveDef::EInterpolationType interpolationType;
	FbxAnimCurveDef::EConstantMode		constantMode;
	FbxAnimCurveDef::ETangentMode		tangentMode;
	FbxAnimCurveDef::EWeightedMode		weightedMode;
	FbxAnimCurveDef::EVelocityMode		velocityMode;

	FbxTime keyTime;
	float	keyValue;

	for(int i=0; i<animCurve->KeyGetCount(); i++)
	{
		keyValue = static_cast<float>(animCurve->KeyGetValue(i));
		keyTime	 = animCurve->KeyGetTime(i);

		interpolationType = animCurve->KeyGetInterpolation(i);
		
		if((animCurve->KeyGetInterpolation(i)&FbxAnimCurveDef::eInterpolationConstant) == FbxAnimCurveDef::eInterpolationConstant)
		{
			constantMode = animCurve->KeyGetConstantMode(i);
		}
		else if((animCurve->KeyGetInterpolation(i)&FbxAnimCurveDef::eInterpolationCubic) == FbxAnimCurveDef::eInterpolationCubic)
		{
			tangentMode		= animCurve->KeyGetTangentMode(i);
			weightedMode	= animCurve->KeyGet(i).GetTangentWeightMode();
			velocityMode	= animCurve->KeyGet(i).GetTangentVelocityMode();
		}

		FbxString outStr;
		char timeString[256];
		outStr = "            Key Time: ";
        outStr += keyTime.GetTimeString(timeString, FbxUShort(256));
        outStr += ".... Key Value: ";
        outStr += keyValue;

		printf("AnimationCurve:  %s \n", outStr.Buffer());
	}
}
void LoaderFbxAnimation::parseAnimationListCurve(FbxAnimCurve* animCurve, FbxProperty* fbxProperty)
{
	FbxTime keyTime;
	int keyValue;
	char timeString[256];
	FbxString outputString;

	for(int i=0; i<animCurve->KeyGetCount(); i++)
	{
		keyValue = static_cast<int>(animCurve->KeyGetValue(i));
		keyTime = animCurve->KeyGetTime(i);

		outputString = "            Key Time: ";
        outputString += keyTime.GetTimeString(timeString, FbxUShort(256));
        outputString += ".... Key Value: ";
        outputString += keyValue;
        outputString += " (";
        outputString += fbxProperty->GetEnumValue(keyValue);
        outputString += ")";

        outputString += "\n";
        FBXSDK_printf (outputString);
	}
}
