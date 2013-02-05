#ifndef XKILL_IO_LOADER_FBX_ANIMATION_BONE_H
#define XKILL_IO_LOADER_FBX_ANIMATION_BONE_H

#include <vector>

#include <xkill-utilities/Math.h>

#include "LoaderFbxAnimationKeyFrame.h"

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

	const char* getName();

	float getAnimationStartTime();
	float getAnimationEndTime();

	std::vector<LoaderFbxAnimationKeyFrame>* getTranslationX();
	std::vector<LoaderFbxAnimationKeyFrame>* getTranslationY();
	std::vector<LoaderFbxAnimationKeyFrame>* getTranslationZ();
	std::vector<LoaderFbxAnimationKeyFrame>* getRotationX();
	std::vector<LoaderFbxAnimationKeyFrame>* getRotationY();
	std::vector<LoaderFbxAnimationKeyFrame>* getRotationZ();
	std::vector<LoaderFbxAnimationKeyFrame>* getScalingX();
	std::vector<LoaderFbxAnimationKeyFrame>* getScalingY();
	std::vector<LoaderFbxAnimationKeyFrame>* getScalingZ();
	std::vector<LoaderFbxAnimationKeyFrame>* getColorRed();
	std::vector<LoaderFbxAnimationKeyFrame>* getColorGreen();
	std::vector<LoaderFbxAnimationKeyFrame>* getColorBlue();
	std::vector<LoaderFbxAnimationKeyFrame>* getLightIntensity();
	std::vector<LoaderFbxAnimationKeyFrame>* getLightOuterAngle();
	std::vector<LoaderFbxAnimationKeyFrame>* getLightFog();
	std::vector<LoaderFbxAnimationKeyFrame>* getCameraFieldOfView();
	std::vector<LoaderFbxAnimationKeyFrame>* getCameraFieldOfViewX();
	std::vector<LoaderFbxAnimationKeyFrame>* getCameraFieldOfViewY();
	std::vector<LoaderFbxAnimationKeyFrame>* getCameraOpticalCenterX();
	std::vector<LoaderFbxAnimationKeyFrame>* getCameraOpticalCenterY();
	std::vector<LoaderFbxAnimationKeyFrame>* getCameraRoll();
	std::vector<LoaderFbxAnimationKeyFrame>* getGeometryShapeChannel();
	std::vector<LoaderFbxAnimationKeyFrame>* getPropertyPrimitive();
	std::vector<LoaderFbxAnimationKeyFrame>* getPropertyVector();
	std::vector<LoaderFbxAnimationKeyFrame>* getPropertyEnum();

	void setName(const char* name);
	void setTranslationX(std::vector<LoaderFbxAnimationKeyFrame> translationX);
	void setTranslationY(std::vector<LoaderFbxAnimationKeyFrame> translationY);
	void setTranslationZ(std::vector<LoaderFbxAnimationKeyFrame> translationZ);
	void setRotationX(std::vector<LoaderFbxAnimationKeyFrame> rotationX);
	void setRotationY(std::vector<LoaderFbxAnimationKeyFrame> rotationY);
	void setRotationZ(std::vector<LoaderFbxAnimationKeyFrame> rotationZ);
	void setScalingX(std::vector<LoaderFbxAnimationKeyFrame> scalingX);
	void setScalingY(std::vector<LoaderFbxAnimationKeyFrame> scalingY);
	void setScalingZ(std::vector<LoaderFbxAnimationKeyFrame> scalingZ);
	void setColorRed(std::vector<LoaderFbxAnimationKeyFrame> colorRed);
	void setColorGreen(std::vector<LoaderFbxAnimationKeyFrame> colorGreen);
	void setColorBlue(std::vector<LoaderFbxAnimationKeyFrame> colorBlue);
	void setLightIntensity(std::vector<LoaderFbxAnimationKeyFrame> lightIntensity);
	void setLightOuterAngle(std::vector<LoaderFbxAnimationKeyFrame> lightOuterAngle);
	void setLightFog(std::vector<LoaderFbxAnimationKeyFrame> lightFog);
	void setCameraFieldOfView(std::vector<LoaderFbxAnimationKeyFrame> cameraFieldOfView);
	void setCameraFieldOfViewX(std::vector<LoaderFbxAnimationKeyFrame> cameraFieldOfViewX);
	void setCameraFieldOfViewY(std::vector<LoaderFbxAnimationKeyFrame> cameraFieldOfViewY);
	void setCameraOpticalCenterX(std::vector<LoaderFbxAnimationKeyFrame> cameraOpticalCenterX);
	void setCameraOpticalCenterY(std::vector<LoaderFbxAnimationKeyFrame> cameraOpticalCenterY);
	void setCameraRoll(std::vector<LoaderFbxAnimationKeyFrame> cameraRoll);
	void setGeometryShapeChannel(std::vector<LoaderFbxAnimationKeyFrame> geometryShapeChannel);
	void setPropertyPrimitive(std::vector<LoaderFbxAnimationKeyFrame> propertyPrimitive);
	void setPropertyVector(std::vector<LoaderFbxAnimationKeyFrame> propertyVector);
	void setPropertyEnum(std::vector<LoaderFbxAnimationKeyFrame> propertyEnum);
private:
	const char*	name_; //<! Name of the bone.

	std::vector<LoaderFbxAnimationKeyFrame> translationX_;			//<! Contains translation along the X-axis.		
	std::vector<LoaderFbxAnimationKeyFrame> translationY_;			//<! Contains translation along the Y-axis.		
	std::vector<LoaderFbxAnimationKeyFrame> translationZ_;			//<! Contains translation along the Z-axis.		
	std::vector<LoaderFbxAnimationKeyFrame> rotationX_;				//<! Contains rotation along the X-axis.			
	std::vector<LoaderFbxAnimationKeyFrame> rotationY_;				//<! Contains rotation along the Y-axis.			
	std::vector<LoaderFbxAnimationKeyFrame> rotationZ_;				//<! Contains rotation along the Z-axis.			
	std::vector<LoaderFbxAnimationKeyFrame> scalingX_;				//<! Contains scaling along the X-axis.			
	std::vector<LoaderFbxAnimationKeyFrame> scalingY_;				//<! Contains scaling along the Y-axis.			
	std::vector<LoaderFbxAnimationKeyFrame> scalingZ_;				//<! Contains scaling along the Z-axis.			
	std::vector<LoaderFbxAnimationKeyFrame> colorRed_;				//<! Contains color red.			
	std::vector<LoaderFbxAnimationKeyFrame> colorGreen_;			//<! Contains color green.	
	std::vector<LoaderFbxAnimationKeyFrame> colorBlue_;				//<! Contains color blue.		
	std::vector<LoaderFbxAnimationKeyFrame> lightIntensity_;		//<! Contains light intensity.	
	std::vector<LoaderFbxAnimationKeyFrame> lightOuterAngle_;		//<! Contains light outer angle.	
	std::vector<LoaderFbxAnimationKeyFrame> lightFog_;				//<! Contains light fog.			
	std::vector<LoaderFbxAnimationKeyFrame> cameraFieldOfView_;		//<! Contains camera field of view.	
	std::vector<LoaderFbxAnimationKeyFrame> cameraFieldOfViewX_;	//<! Contains camera field of view along the X-axis.
	std::vector<LoaderFbxAnimationKeyFrame> cameraFieldOfViewY_;	//<! Contains camera field of view along the Y-axis.
	std::vector<LoaderFbxAnimationKeyFrame> cameraOpticalCenterX_;	//<! Contains camera optical center X.
	std::vector<LoaderFbxAnimationKeyFrame> cameraOpticalCenterY_;	//<! Contains camera optical center Y.
	std::vector<LoaderFbxAnimationKeyFrame> cameraRoll_;			//<! Contains camera roll.		
	std::vector<LoaderFbxAnimationKeyFrame> geometryShapeChannel_; 	//<! Contains geometry shape channel.
	std::vector<LoaderFbxAnimationKeyFrame> propertyPrimitive_;		//<! Contains property primitives.	
	std::vector<LoaderFbxAnimationKeyFrame> propertyVector_;		//<! Contains property vectors.
	std::vector<LoaderFbxAnimationKeyFrame> propertyEnum_;			//<! Contains property enums.		

};

#endif //XKILL_IO_LOADER_FBX_ANIMATION_BONE_H