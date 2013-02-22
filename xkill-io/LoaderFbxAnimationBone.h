#ifndef XKILL_IO_LOADER_FBX_ANIMATION_BONE_H
#define XKILL_IO_LOADER_FBX_ANIMATION_BONE_H

#include <vector>

#include <xkill-utilities/Math.h>

#include "LoaderFbxAnimationKeyFrame.h"

enum FbxKeyframeIndex
{
	FBX_KEYFRAME_INDEX_TRANSLATION_X,		
	FBX_KEYFRAME_INDEX_TRANSLATION_Y,		
	FBX_KEYFRAME_INDEX_TRANSLATION_Z,		
	FBX_KEYFRAME_INDEX_ROTATION_X,			
	FBX_KEYFRAME_INDEX_ROTATION_Y,			
	FBX_KEYFRAME_INDEX_ROTATION_Z,			
	FBX_KEYFRAME_INDEX_SCALING_X,			
	FBX_KEYFRAME_INDEX_SCALING_Y,			
	FBX_KEYFRAME_INDEX_SCALING_Z,			
	FBX_KEYFRAME_INDEX_COLOR_RED,			
	FBX_KEYFRAME_INDEX_COLOR_GREEN,		
	FBX_KEYFRAME_INDEX_COLOR_BLUE,			
	FBX_KEYFRAME_INDEX_LIGHT_INTENSITY,	
	FBX_KEYFRAME_INDEX_LIGHT_OUTER_ANGLE,	
	FBX_KEYFRAME_INDEX_LIGHT_FOG,			
	FBX_KEYFRAME_INDEX_CAMERA_FIELD_OF_VIEW,	
	FBX_KEYFRAME_INDEX_CAMERA_FIELD_OF_VIEW_X,
	FBX_KEYFRAME_INDEX_CAMERA_FIELD_OF_VIEW_Y,
	FBX_KEYFRAME_INDEX_CAMERA_OPTICAL_CENTER_X,
	FBX_KEYFRAME_INDEX_CAMERA_OPTICAL_CENTER_Y,
	FBX_KEYFRAME_INDEX_CAMERA_ROLL,		
	FBX_KEYFRAME_INDEX_GEOMETRY_SHAPE_CHANNEL,
	FBX_KEYFRAME_INDEX_PROPERTY_PRIMITIVE,	
	FBX_KEYFRAME_INDEX_PROPERTY_VECTOR,	
	FBX_KEYFRAME_INDEX_PROPERTY_ENUM	
};

static const unsigned int NUM_KEYFRAME_TYPES = 25;

//! Class for describing a bone and its keyframes.
/*!
\ingroup xkill-io-fbx
*/
class LoaderFbxAnimationBone
{
public:
	//! Initializes LoaderFbxAnimationBone to its default state.
	LoaderFbxAnimationBone();
	//! Initializes LoaderFbxAnimationBone with specific values.
	LoaderFbxAnimationBone(const char* name);
	//! Currently unused.
	~LoaderFbxAnimationBone();

	int averageNumKeyframes(FbxKeyframeIndex indices[], int numIndices);

	float interpolateValue(FbxKeyframeIndex index, float time);

	float getAnimationStartTime(FbxKeyframeIndex index);
	float getAnimationEndTime(FbxKeyframeIndex index);

	const char* getName();
	std::vector<LoaderFbxAnimationKeyFrame>* getKeyframes(FbxKeyframeIndex index);

	void setName(const char* name);
	void setKeyframes(FbxKeyframeIndex index, std::vector<LoaderFbxAnimationKeyFrame> keyframes);

private:

	float keyframeFirst(FbxKeyframeIndex index);
	float keyframeLast(FbxKeyframeIndex index);
	float keyframeInterpolate(FbxKeyframeIndex index, float time);

	const char*	name_; //!< Name of the bone.
	std::vector<std::vector<LoaderFbxAnimationKeyFrame>> keyframes_;
};

#endif //XKILL_IO_LOADER_FBX_ANIMATION_BONE_H