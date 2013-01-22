#ifndef XKILL_IO_LOADER_FBX_ANIMATION_BONE_H
#define XKILL_IO_LOADER_FBX_ANIMATION_BONE_H

#include <vector>

#include <xkill-utilities/Math.h>

#include "LoaderFbxAnimationKeyFrame.h"

class LoaderFbxAnimationBone
{
public:
	LoaderFbxAnimationBone();
	LoaderFbxAnimationBone(const char* name);
	~LoaderFbxAnimationBone();

	const char* getName();
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
	const char*			name_;

	std::vector<LoaderFbxAnimationKeyFrame> translationX_;
	std::vector<LoaderFbxAnimationKeyFrame> translationY_;
	std::vector<LoaderFbxAnimationKeyFrame> translationZ_;
	std::vector<LoaderFbxAnimationKeyFrame> rotationX_;
	std::vector<LoaderFbxAnimationKeyFrame> rotationY_;
	std::vector<LoaderFbxAnimationKeyFrame> rotationZ_;
	std::vector<LoaderFbxAnimationKeyFrame> scalingX_;
	std::vector<LoaderFbxAnimationKeyFrame> scalingY_;
	std::vector<LoaderFbxAnimationKeyFrame> scalingZ_;
	std::vector<LoaderFbxAnimationKeyFrame> colorRed_;
	std::vector<LoaderFbxAnimationKeyFrame> colorGreen_;
	std::vector<LoaderFbxAnimationKeyFrame> colorBlue_;
	std::vector<LoaderFbxAnimationKeyFrame> lightIntensity_;
	std::vector<LoaderFbxAnimationKeyFrame> lightOuterAngle_;
	std::vector<LoaderFbxAnimationKeyFrame> lightFog_;
	std::vector<LoaderFbxAnimationKeyFrame> cameraFieldOfView_;
	std::vector<LoaderFbxAnimationKeyFrame> cameraFieldOfViewX_;
	std::vector<LoaderFbxAnimationKeyFrame> cameraFieldOfViewY_;
	std::vector<LoaderFbxAnimationKeyFrame> cameraOpticalCenterX_;
	std::vector<LoaderFbxAnimationKeyFrame> cameraOpticalCenterY_;
	std::vector<LoaderFbxAnimationKeyFrame> cameraRoll_;
	std::vector<LoaderFbxAnimationKeyFrame> geometryShapeChannel_;
	std::vector<LoaderFbxAnimationKeyFrame> propertyPrimitive_;
	std::vector<LoaderFbxAnimationKeyFrame> propertyVector_;
	std::vector<LoaderFbxAnimationKeyFrame> propertyEnum_;

};

#endif //XKILL_IO_LOADER_FBX_ANIMATION_BONE_H