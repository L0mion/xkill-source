#ifndef XKILL_IO_LOADER_FBX_ANIMATION_KEY_FRAME_H
#define XKILL_IO_LOADER_FBX_ANIMATION_KEY_FRAME_H

//! Struct for describing a keyframe
/*!
\ingroup xkill-io-fbx
*/
struct LoaderFbxAnimationKeyFrame
{
	//! Initializes LoaderFbxAnimationKeyFrame to its default state.
	LoaderFbxAnimationKeyFrame();
	//! Initializes LoaderFbxAnimationKeyFrame with specific values.
	LoaderFbxAnimationKeyFrame(float time,
							   float value,
							   int interpolationType,
							   int constantMode,
							   int tangentMode,
							   int weightedMode,
							   int velocityMode);
	
	float time_;			//!< At what point in time the keyframe is located.
	float value_;			//!< What value the keyframe has.
							 
	int interpolationType_;	//!< Type of interpolation between keyframes.
	int constantMode_;		//!< Constant mode.
	int tangentMode_;		//!< Tangent mode.
	int weightedMode_;		//!< Weighted mode.
	int velocityMode_;		//!< Velocity mode.
};

#endif //XKILL_IO_LOADER_FBX_ANIMATION_KEY_FRAME_H