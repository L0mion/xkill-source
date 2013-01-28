#include "LoaderFbxAnimationBone.h"

LoaderFbxAnimationBone::LoaderFbxAnimationBone()
{
}
LoaderFbxAnimationBone::LoaderFbxAnimationBone(const char* name)
{
	name_ = name;
}
LoaderFbxAnimationBone::~LoaderFbxAnimationBone()
{
}

const char* LoaderFbxAnimationBone::getName()
{
	return name_;
}

std::vector<LoaderFbxAnimationKeyFrame>* LoaderFbxAnimationBone::getTranslationX()
{
	return &translationX_;
}
std::vector<LoaderFbxAnimationKeyFrame>* LoaderFbxAnimationBone::getTranslationY()
{
	return &translationY_;
}
std::vector<LoaderFbxAnimationKeyFrame>* LoaderFbxAnimationBone::getTranslationZ()
{
	return &translationZ_;
}
std::vector<LoaderFbxAnimationKeyFrame>* LoaderFbxAnimationBone::getRotationX()
{
	return &rotationX_;
}
std::vector<LoaderFbxAnimationKeyFrame>* LoaderFbxAnimationBone::getRotationY()
{
	return &rotationY_;
}
std::vector<LoaderFbxAnimationKeyFrame>* LoaderFbxAnimationBone::getRotationZ()
{
	return &rotationZ_;
}
std::vector<LoaderFbxAnimationKeyFrame>* LoaderFbxAnimationBone::getScalingX()
{
	return &scalingX_;
}
std::vector<LoaderFbxAnimationKeyFrame>* LoaderFbxAnimationBone::getScalingY()
{
	return &scalingY_;
}
std::vector<LoaderFbxAnimationKeyFrame>* LoaderFbxAnimationBone::getScalingZ()
{
	return &scalingZ_;
}
std::vector<LoaderFbxAnimationKeyFrame>* LoaderFbxAnimationBone::getColorRed()
{
	return &colorRed_;
}
std::vector<LoaderFbxAnimationKeyFrame>* LoaderFbxAnimationBone::getColorGreen()
{
	return &colorGreen_;
}
std::vector<LoaderFbxAnimationKeyFrame>* LoaderFbxAnimationBone::getColorBlue()
{
	return &colorBlue_;
}
std::vector<LoaderFbxAnimationKeyFrame>* LoaderFbxAnimationBone::getLightIntensity()
{
	return &lightIntensity_;
}
std::vector<LoaderFbxAnimationKeyFrame>* LoaderFbxAnimationBone::getLightOuterAngle()
{
	return &lightOuterAngle_;
}
std::vector<LoaderFbxAnimationKeyFrame>* LoaderFbxAnimationBone::getLightFog()
{
	return &lightFog_;
}
std::vector<LoaderFbxAnimationKeyFrame>* LoaderFbxAnimationBone::getCameraFieldOfView()
{
	return &cameraFieldOfView_;
}
std::vector<LoaderFbxAnimationKeyFrame>* LoaderFbxAnimationBone::getCameraFieldOfViewX()
{
	return &cameraFieldOfViewX_;
}
std::vector<LoaderFbxAnimationKeyFrame>* LoaderFbxAnimationBone::getCameraFieldOfViewY()
{
	return &cameraFieldOfViewY_;
}
std::vector<LoaderFbxAnimationKeyFrame>* LoaderFbxAnimationBone::getCameraOpticalCenterX()
{
	return &cameraOpticalCenterX_;
}
std::vector<LoaderFbxAnimationKeyFrame>* LoaderFbxAnimationBone::getCameraOpticalCenterY()
{
	return &cameraOpticalCenterY_;
}
std::vector<LoaderFbxAnimationKeyFrame>* LoaderFbxAnimationBone::getCameraRoll()
{
	return &cameraRoll_;
}
std::vector<LoaderFbxAnimationKeyFrame>* LoaderFbxAnimationBone::getGeometryShapeChannel()
{
	return &geometryShapeChannel_;
}
std::vector<LoaderFbxAnimationKeyFrame>* LoaderFbxAnimationBone::getPropertyPrimitive()
{
	return &propertyPrimitive_;
}
std::vector<LoaderFbxAnimationKeyFrame>* LoaderFbxAnimationBone::getPropertyVector()
{
	return &propertyVector_;
}
std::vector<LoaderFbxAnimationKeyFrame>* LoaderFbxAnimationBone::getPropertyEnum()
{
	return &propertyEnum_;
}

void LoaderFbxAnimationBone::setName(const char* name)
{
	name_ = name;
}
void LoaderFbxAnimationBone::setTranslationX(std::vector<LoaderFbxAnimationKeyFrame> translationX)
{
	translationX_ = translationX;
}
void LoaderFbxAnimationBone::setTranslationY(std::vector<LoaderFbxAnimationKeyFrame> translationY)
{
	translationY_ = translationY;
}
void LoaderFbxAnimationBone::setTranslationZ(std::vector<LoaderFbxAnimationKeyFrame> translationZ)
{
	translationZ_ = translationZ;
}
void LoaderFbxAnimationBone::setRotationX(std::vector<LoaderFbxAnimationKeyFrame> rotationX)
{
	rotationX_ = rotationX;
}
void LoaderFbxAnimationBone::setRotationY(std::vector<LoaderFbxAnimationKeyFrame> rotationY)
{
	rotationY_ = rotationY;
}
void LoaderFbxAnimationBone::setRotationZ(std::vector<LoaderFbxAnimationKeyFrame> rotationZ)
{
	rotationZ_ =rotationZ;
}
void LoaderFbxAnimationBone::setScalingX(std::vector<LoaderFbxAnimationKeyFrame> scalingX)
{
	scalingX_ = scalingX;
}
void LoaderFbxAnimationBone::setScalingY(std::vector<LoaderFbxAnimationKeyFrame> scalingY)
{
	scalingY_ = scalingY;
}
void LoaderFbxAnimationBone::setScalingZ(std::vector<LoaderFbxAnimationKeyFrame> scalingZ)
{
	scalingZ_ = scalingZ;
}
void LoaderFbxAnimationBone::setColorRed(std::vector<LoaderFbxAnimationKeyFrame> colorRed)
{
	colorRed_ = colorRed;
}
void LoaderFbxAnimationBone::setColorGreen(std::vector<LoaderFbxAnimationKeyFrame> colorGreen)
{
	colorGreen_ = colorGreen;
}
void LoaderFbxAnimationBone::setColorBlue(std::vector<LoaderFbxAnimationKeyFrame> colorBlue)
{
	colorBlue_ = colorBlue;
}
void LoaderFbxAnimationBone::setLightIntensity(std::vector<LoaderFbxAnimationKeyFrame> lightIntensity)
{
	lightIntensity_ = lightIntensity;
}
void LoaderFbxAnimationBone::setLightOuterAngle(std::vector<LoaderFbxAnimationKeyFrame> lightOuterAngle)
{
	lightOuterAngle_ = lightOuterAngle;
}
void LoaderFbxAnimationBone::setLightFog(std::vector<LoaderFbxAnimationKeyFrame> lightFog)
{
	lightFog_ = lightFog;
}
void LoaderFbxAnimationBone::setCameraFieldOfView(std::vector<LoaderFbxAnimationKeyFrame> cameraFieldOfView)
{
	cameraFieldOfView_ = cameraFieldOfView;
}
void LoaderFbxAnimationBone::setCameraFieldOfViewX(std::vector<LoaderFbxAnimationKeyFrame> cameraFieldOfViewX)
{
	cameraFieldOfViewX_ = cameraFieldOfViewX;
}
void LoaderFbxAnimationBone::setCameraFieldOfViewY(std::vector<LoaderFbxAnimationKeyFrame> cameraFieldOfViewY)
{
	cameraFieldOfViewY_ = cameraFieldOfViewY;
}
void LoaderFbxAnimationBone::setCameraOpticalCenterX(std::vector<LoaderFbxAnimationKeyFrame> cameraOpticalCenterX)
{
	cameraOpticalCenterX_ = cameraOpticalCenterX;
}
void LoaderFbxAnimationBone::setCameraOpticalCenterY(std::vector<LoaderFbxAnimationKeyFrame> cameraOpticalCenterY)
{
	cameraOpticalCenterY_ = cameraOpticalCenterY;
}
void LoaderFbxAnimationBone::setCameraRoll(std::vector<LoaderFbxAnimationKeyFrame> cameraRoll)
{
	cameraRoll_ = cameraRoll;
}
void LoaderFbxAnimationBone::setGeometryShapeChannel(std::vector<LoaderFbxAnimationKeyFrame> geometryShapeChannel)
{
	geometryShapeChannel_ = geometryShapeChannel;
}
void LoaderFbxAnimationBone::setPropertyPrimitive(std::vector<LoaderFbxAnimationKeyFrame> propertyPrimitive)
{
	propertyPrimitive_ = propertyPrimitive;
}
void LoaderFbxAnimationBone::setPropertyVector(std::vector<LoaderFbxAnimationKeyFrame> propertyVector)
{
	propertyVector_ = propertyVector;
}
void LoaderFbxAnimationBone::setPropertyEnum(std::vector<LoaderFbxAnimationKeyFrame> propertyEnum)
{
	propertyEnum_ = propertyEnum;
}