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

//! Class for loading a FBX animation.
/*!
\ingroup xkill-io-fbx
*/
class LoaderFbxAnimation
{
public:
	//! Initializes LoaderFbxAnimation to its default state.
	LoaderFbxAnimation();
	//! Currently unused.
	~LoaderFbxAnimation();
	//! Clears vectors used for storing information.
	void reset();

	//! Loads animation data.
	/*!
	\param scene A FbxScene object from which animation stacks can be retrieved.
	\param animationDescs A vector of animation descriptions where the animations will be stored.
	\param boneNodes Nodes you want to retrieve animation data from.
	*/
	void parseAnimation(FbxScene* scene, std::vector<LoaderFbxAnimationDesc>* animationDescs, std::vector<FbxNode*> boneNodes);
private:
	//! Parses an animation stack.
	/*!
	\param animStack The animation stack to be parsed.
	\param boneNodes Nodes you want to retrieve animation data from.
	\param isSwitcher ?
	*/
	void parseAnimationStack(FbxAnimStack* animStack, std::vector<FbxNode*> boneNodes, bool isSwitcher);
	//! Parses an animation layer.
	/*!
	\param animLayer The animation layer to be parsed.
	\param boneNodes Nodes you want to retrieve animation data from.
	\param isSwitcher ?
	*/
	void parseAnimationLayer(FbxAnimLayer* animLayer, std::vector<FbxNode*> boneNodes, bool isSwitcher);
	
	//! Parses an animation channel.
	/*!
	\param node A node containing data to be animated.
	\param animLayer Animation layer containing curve nodes.
	\param isSwitcher ?
	*/
	void parseAnimationChannels(FbxNode* node, FbxAnimLayer* animLayer, bool isSwitcher);
	//! Parses general information from an animation channel.
	/*!
	\param node A node containing data to be animated.
	\param animLayer Animation layer containing curve nodes.
	\param animCurve Animation curve describing the animation.
	*/
	void parseAnimationChannelsGeneral(FbxNode* node, FbxAnimLayer* animLayer, FbxAnimCurve* animCurve);
	//! Parses translation information from an animation channel.
	/*!
	\param node A node containing data to be animated.
	\param animLayer Animation layer containing curve nodes.
	\param animCurve Animation curve describing the animation.
	*/
	void parseAnimationChannelsGeneralTranslation(FbxNode* node, FbxAnimLayer* animLayer, FbxAnimCurve* animCurve);
	//! Parses rotation information from an animation channel.
	/*!
	\param node A node containing data to be animated.
	\param animLayer Animation layer containing curve nodes.
	\param animCurve Animation curve describing the animation.
	*/
	void parseAnimationChannelsGeneralRotation(FbxNode* node, FbxAnimLayer* animLayer, FbxAnimCurve* animCurve);
	//! Parses scaling information from an animation channel.
	/*!
	\param node A node containing data to be animated.
	\param animLayer Animation layer containing curve nodes.
	\param animCurve Animation curve describing the animation.
	*/
	void parseAnimationChannelsGeneralScaling(FbxNode* node, FbxAnimLayer* animLayer, FbxAnimCurve* animCurve);
	//! Parses color information from an animation channel.
	/*!
	\param nodeAttribute A node attribute containing data to be animated.
	\param animLayer Animation layer containing curve nodes.
	\param animCurve Animation curve describing the animation.
	*/
	void parseAnimationChannelsColor(FbxNodeAttribute* nodeAttribute, FbxAnimLayer* animLayer, FbxAnimCurve* animCurve);
	//! Parses lamp information from an animation channel.
	/*!
	\param node A node containing data to be animated.
	\param animLayer Animation layer containing curve nodes.
	\param animCurve Animation curve describing the animation.
	*/
	void parseAnimationChannelsLamp(FbxNode* node, FbxAnimLayer* animLayer, FbxAnimCurve* animCurve);
	//! Parses camera information from an animation channel.
	/*!
	\param node A node containing data to be animated.
	\param animLayer Animation layer containing curve nodes.
	\param animCurve Animation curve describing the animation.
	*/
	void parseAnimationChannelsCamera(FbxNode* node, FbxAnimLayer* animLayer, FbxAnimCurve* animCurve);
	//! Parses geometry information from an animation channel.
	/*!
	\param nodeAttribute A node attribute containing data to be animated.
	\param animLayer Animation layer containing curve nodes.
	\param animCurve Animation curve describing the animation.
	*/
	void parseAnimationChannelsGeometry(FbxNodeAttribute* nodeAttribute, FbxAnimLayer* animLayer, FbxAnimCurve* animCurve);
	//! Parses property information from an animation channel.
	/*!
	\param node A node containing data to be animated.
	\param animLayer Animation layer containing curve nodes.
	\param animCurve Animation curve describing the animation.
	*/
	void parseAnimationChannelsProperty(FbxNode* node, FbxAnimLayer* animLayer, FbxAnimCurve* animCurve);
	//! Parses property data type information from an animation channel.
	/*!
	\param curveNode A curve node describing the animation.
	\param animCurve Animation curve describing the animation.
	\param fbxProperty Property containing data.
	*/
	void parseAnimationChannelsPropertyDataType(FbxAnimCurveNode* curveNode, FbxAnimCurve* animCurve, FbxProperty* fbxProperty);
	//! Parses property primitive information from an animation channel.
	/*!
	\param curveNode A curve node describing the animation.
	\param animCurve Animation curve describing the animation.
	*/
	void parseAnimationChannelsPropertyPrimitive(FbxAnimCurveNode* curveNode, FbxAnimCurve* animCurve);
	//! Parses property vector information from an animation channel.
	/*!
	\param curveNode A curve node describing the animation.
	\param animCurve Animation curve describing the animation.
	*/
	void parseAnimationChannelsPropertyVector(FbxAnimCurveNode* curveNode, FbxAnimCurve* animCurve);
	//! Parses property enum information from an animation channel.
	/*!
	\param curveNode A curve node describing the animation.
	\param animCurve Animation curve describing the animation.
	\param fbxProperty Property containing data.
	*/
	void parseAnimationChannelsPropertyEnum(FbxAnimCurveNode* curveNode, FbxAnimCurve* animCurve, FbxProperty* fbxProperty);

	//! Fills a keyframe with animation information
	/*!
	\param animCurve Animation curve conatining the animation data.
	\parma keyFrames A vector of key frames where the new key frame will be added. 
	*/
	void parseAnimationCurve(FbxAnimCurve* animCurve, std::vector<LoaderFbxAnimationKeyFrame>* keyFrames);
	//! Fills a keyframe with animation information
	/*!
	\param animCurve Animation curve conatining the animation data.
	\parma keyFrames A vector of key frames where the new key frame will be added. 
	*/
	void parseAnimationListCurve(FbxAnimCurve* animCurve, FbxProperty* fbxProperty, std::vector<LoaderFbxAnimationKeyFrame>* keyFrames);


	std::vector<LoaderFbxAnimationBone> animationBones_; //<! Vector containing bones. Here each bone is a collection of key frames.
};

#endif //XKILL_IO_LOADERFBXANIMATION_H