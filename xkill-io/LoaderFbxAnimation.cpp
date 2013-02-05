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
	animationBones_.clear();
}

void LoaderFbxAnimation::parseAnimation(FbxScene* scene, std::vector<LoaderFbxAnimationDesc>* animationDescs, std::vector<FbxNode*> boneNodes)
{
	reset();

	for(int i=0; i<scene->GetSrcObjectCount<FbxAnimStack>(); i++)
	{
		animationBones_.clear();
		FbxAnimStack* animStack = scene->GetSrcObject<FbxAnimStack>(i);
	
	//	FbxString str = "Animation Stack Name: ";
	//	str += animStack->GetName();
	//	str += "\n";
	//	printf("%s", str.Buffer());

		animationDescs->push_back(LoaderFbxAnimationDesc(animStack->GetName()));

	//	parseAnimationStack(animStack, scene->GetRootNode(), true);
		parseAnimationStack(animStack, boneNodes, false);

		animationDescs->back().setBones(animationBones_);
	}
}
void LoaderFbxAnimation::parseAnimationStack(FbxAnimStack* animStack, std::vector<FbxNode*> boneNodes, bool isSwitcher)
{
	int numAnimationLayers = animStack->GetMemberCount<FbxAnimLayer>();
	
	//FbxString str = "Animation stack contains ";
	//str += numAnimationLayers;
	//str += " Animation Layer(s)\n";
	//printf(str.Buffer());

	for(int i=0; i<numAnimationLayers; i++)
	{
		FbxAnimLayer* animLayer = animStack->GetMember<FbxAnimLayer>(i);

	//	str = "AnimLayer ";
	//	str += i;
	//	str += ": ";
	//	str += animLayer->GetName();
	//	str += "\n";
	//	printf(str.Buffer());

		parseAnimationLayer(animLayer, boneNodes, isSwitcher);
	}
}
void LoaderFbxAnimation::parseAnimationLayer(FbxAnimLayer* animLayer, std::vector<FbxNode*> boneNodes, bool isSwitcher)
{
//	FbxString str = "Node name: ";
//	str += node->GetName();
//	str += "\n";
//	printf(str.Buffer());

	for(unsigned int i=0; i<boneNodes.size(); i++)
	{
		animationBones_.push_back(LoaderFbxAnimationBone(boneNodes[i]->GetName()));
		parseAnimationChannels(boneNodes[i], animLayer, isSwitcher);
	}
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
		//printf("    TX:\n");
		parseAnimationCurve(animCurve, animationBones_.back().getKeyframes(FBX_KEYFRAME_INDEX_TRANSLATION_X));
	}
	animCurve = node->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
	if(animCurve)
	{
		//printf("    TY:\n");
		parseAnimationCurve(animCurve, animationBones_.back().getKeyframes(FBX_KEYFRAME_INDEX_TRANSLATION_Y));
	}
	animCurve = node->LclTranslation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
	if(animCurve)
	{
		//printf("    TZ:\n");
		parseAnimationCurve(animCurve, animationBones_.back().getKeyframes(FBX_KEYFRAME_INDEX_TRANSLATION_Z));
	}
}
void LoaderFbxAnimation::parseAnimationChannelsGeneralRotation(FbxNode* node, FbxAnimLayer* animLayer, FbxAnimCurve* animCurve)
{
	animCurve = node->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
	if(animCurve)
	{
		//printf("    RX:\n");
		parseAnimationCurve(animCurve, animationBones_.back().getKeyframes(FBX_KEYFRAME_INDEX_ROTATION_X));
	}
	animCurve = node->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
	if(animCurve)
	{
		//printf("    RY:\n");
		parseAnimationCurve(animCurve, animationBones_.back().getKeyframes(FBX_KEYFRAME_INDEX_ROTATION_Y));
	}
	animCurve = node->LclRotation.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
	if(animCurve)
	{
		//printf("    RZ:\n");
		parseAnimationCurve(animCurve, animationBones_.back().getKeyframes(FBX_KEYFRAME_INDEX_ROTATION_Z));
	}
}
void LoaderFbxAnimation::parseAnimationChannelsGeneralScaling(FbxNode* node, FbxAnimLayer* animLayer, FbxAnimCurve* animCurve)
{
	animCurve = node->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_X);
	if(animCurve)
	{
		//printf("    SX:\n");
		parseAnimationCurve(animCurve, animationBones_.back().getKeyframes(FBX_KEYFRAME_INDEX_SCALING_X));
	}
	animCurve = node->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Y);
	if(animCurve)
	{
		//printf("    SY:\n");
		parseAnimationCurve(animCurve, animationBones_.back().getKeyframes(FBX_KEYFRAME_INDEX_SCALING_Y));
	}
	animCurve = node->LclScaling.GetCurve(animLayer, FBXSDK_CURVENODE_COMPONENT_Z);
	if(animCurve)
	{
		//printf("    SZ:\n");
		parseAnimationCurve(animCurve, animationBones_.back().getKeyframes(FBX_KEYFRAME_INDEX_SCALING_Z));
	}
}
void LoaderFbxAnimation::parseAnimationChannelsColor(FbxNodeAttribute* nodeAttribute, FbxAnimLayer* animLayer, FbxAnimCurve* animCurve)
{
	if(nodeAttribute)
	{
		animCurve = nodeAttribute->Color.GetCurve(animLayer, FBXSDK_CURVENODE_COLOR_RED);
		if(animCurve)
		{
			parseAnimationCurve(animCurve, animationBones_.back().getKeyframes(FBX_KEYFRAME_INDEX_COLOR_RED));
		}
		animCurve = nodeAttribute->Color.GetCurve(animLayer, FBXSDK_CURVENODE_COLOR_GREEN);
		if(animCurve)
		{
			parseAnimationCurve(animCurve, animationBones_.back().getKeyframes(FBX_KEYFRAME_INDEX_COLOR_GREEN));
		}
		animCurve = nodeAttribute->Color.GetCurve(animLayer, FBXSDK_CURVENODE_COLOR_BLUE);
		if(animCurve)
		{
			parseAnimationCurve(animCurve, animationBones_.back().getKeyframes(FBX_KEYFRAME_INDEX_COLOR_BLUE));
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
			parseAnimationCurve(animCurve, animationBones_.back().getKeyframes(FBX_KEYFRAME_INDEX_LIGHT_INTENSITY));
		}
		animCurve = light->OuterAngle.GetCurve(animLayer);
		if(animCurve)
		{
			parseAnimationCurve(animCurve, animationBones_.back().getKeyframes(FBX_KEYFRAME_INDEX_LIGHT_OUTER_ANGLE));
		}
		animCurve = light->Fog.GetCurve(animLayer);
		if(animCurve)
		{
			parseAnimationCurve(animCurve, animationBones_.back().getKeyframes(FBX_KEYFRAME_INDEX_LIGHT_FOG));
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
			parseAnimationCurve(animCurve, animationBones_.back().getKeyframes(FBX_KEYFRAME_INDEX_CAMERA_FIELD_OF_VIEW));
		}
		animCurve = camera->FieldOfViewX.GetCurve(animLayer);
		if(animCurve)
		{
			parseAnimationCurve(animCurve, animationBones_.back().getKeyframes(FBX_KEYFRAME_INDEX_CAMERA_FIELD_OF_VIEW_X));
		}
		animCurve = camera->FieldOfViewY.GetCurve(animLayer);
		if(animCurve)
		{
			parseAnimationCurve(animCurve, animationBones_.back().getKeyframes(FBX_KEYFRAME_INDEX_CAMERA_FIELD_OF_VIEW_Y));
		}
		animCurve = camera->OpticalCenterX.GetCurve(animLayer);
		if(animCurve)
		{
			parseAnimationCurve(animCurve, animationBones_.back().getKeyframes(FBX_KEYFRAME_INDEX_CAMERA_OPTICAL_CENTER_X));
		}
		animCurve = camera->OpticalCenterY.GetCurve(animLayer);
		if(animCurve)
		{
			parseAnimationCurve(animCurve, animationBones_.back().getKeyframes(FBX_KEYFRAME_INDEX_CAMERA_OPTICAL_CENTER_X));
		}
		animCurve = camera->Roll.GetCurve(animLayer);
		if(animCurve)
		{
			parseAnimationCurve(animCurve, animationBones_.back().getKeyframes(FBX_KEYFRAME_INDEX_CAMERA_ROLL));
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
						parseAnimationCurve(animCurve, animationBones_.back().getKeyframes(FBX_KEYFRAME_INDEX_GEOMETRY_SHAPE_CHANNEL));
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
			parseAnimationCurve(animCurve, animationBones_.back().getKeyframes(FBX_KEYFRAME_INDEX_PROPERTY_PRIMITIVE));
	}
}
void LoaderFbxAnimation::parseAnimationChannelsPropertyVector(FbxAnimCurveNode* curveNode, FbxAnimCurve* animCurve)
{
	for(int i=0; i<curveNode->GetCurveCount(0U); i++)
	{
		animCurve = curveNode->GetCurve(0U, i);
		if(animCurve)
			parseAnimationCurve(animCurve, animationBones_.back().getKeyframes(FBX_KEYFRAME_INDEX_PROPERTY_VECTOR));
	}
	for(int i=0; i<curveNode->GetCurveCount(1U); i++)
	{
		animCurve = curveNode->GetCurve(1U, i);
		if(animCurve)
			parseAnimationCurve(animCurve, animationBones_.back().getKeyframes(FBX_KEYFRAME_INDEX_PROPERTY_VECTOR));
	}
	for(int i=0; i<curveNode->GetCurveCount(2U); i++)
	{
		animCurve = curveNode->GetCurve(2U, i);
		if(animCurve)
			parseAnimationCurve(animCurve, animationBones_.back().getKeyframes(FBX_KEYFRAME_INDEX_PROPERTY_VECTOR));
	}
}
void LoaderFbxAnimation::parseAnimationChannelsPropertyEnum(FbxAnimCurveNode* curveNode, FbxAnimCurve* animCurve, FbxProperty* fbxProperty)
{
	 for(int i=0; i<curveNode->GetCurveCount(0U); i++)
	 {
	 	 animCurve = curveNode->GetCurve(0U, i);
	 	 if(animCurve)
	 	 {
			 parseAnimationListCurve(animCurve, fbxProperty, animationBones_.back().getKeyframes(FBX_KEYFRAME_INDEX_PROPERTY_ENUM));
	 	 }
	 }
}
void LoaderFbxAnimation::parseAnimationCurve(FbxAnimCurve* animCurve, std::vector<LoaderFbxAnimationKeyFrame>* keyFrames)
{
	FbxTime keyTime;

	int numKeys = animCurve->KeyGetCount();

	for(int i=0; i<animCurve->KeyGetCount(); i++)
	{
		LoaderFbxAnimationKeyFrame keyFrame;

		keyTime			= animCurve->KeyGetTime(i);
		keyFrame.value_ = static_cast<float>(animCurve->KeyGetValue(i));
		keyFrame.time_	= static_cast<float>(keyTime.GetSecondDouble());

		keyFrame.interpolationType_ = animCurve->KeyGetInterpolation(i);
		
		if((animCurve->KeyGetInterpolation(i)&FbxAnimCurveDef::eInterpolationConstant) == FbxAnimCurveDef::eInterpolationConstant)
		{
			keyFrame.constantMode_ = animCurve->KeyGetConstantMode(i);
		}
		else if((animCurve->KeyGetInterpolation(i)&FbxAnimCurveDef::eInterpolationCubic) == FbxAnimCurveDef::eInterpolationCubic)
		{
			keyFrame.tangentMode_	= animCurve->KeyGetTangentMode(i);
			keyFrame.weightedMode_	= animCurve->KeyGet(i).GetTangentWeightMode();
			keyFrame.velocityMode_	= animCurve->KeyGet(i).GetTangentVelocityMode();
		}

		keyFrames->push_back(keyFrame);
	}
}
void LoaderFbxAnimation::parseAnimationListCurve(FbxAnimCurve* animCurve, FbxProperty* fbxProperty, std::vector<LoaderFbxAnimationKeyFrame>* keyFrames)
{
	FbxTime keyTime;
	for(int i=0; i<animCurve->KeyGetCount(); i++)
	{
		LoaderFbxAnimationKeyFrame keyFrame;

		keyFrame.value_ = static_cast<int>(animCurve->KeyGetValue(i));
		keyTime			= animCurve->KeyGetTime(i);
		keyFrame.time_	= static_cast<float>(keyTime.GetSecondDouble());

		keyFrames->push_back(keyFrame);

		//FbxString outputString;
		//char timeString[256];
		//outputString = "            Key Time: ";
        //outputString += keyTime.GetTimeString(timeString, FbxUShort(256));
        //outputString += ".... Key Value: ";
        //outputString += keyValue;
        //outputString += " (";
        //outputString += fbxProperty->GetEnumValue(keyValue);
        //outputString += ")";
		//
        //outputString += "\n";
        //FBXSDK_printf (outputString);
	}
}