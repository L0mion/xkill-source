#include "LoaderFbxTextureDesc.h"

LoaderFbxTextureDesc::LoaderFbxTextureDesc()
{
	textureName_ = "Unknown";
	fileName_	 = "Unknown";
	
	scale_					= Float2(0.0f, 0.0f);
	translation_			= Float2(0.0f, 0.0f);
	rotation_				= Float3(0.0f, 0.0f, 0.0f);
	swap_					= false;
	alphaSource_			= 0;
	croppingLeft_			= 0;
	croppingTop_			= 0;
	croppingRight_			= 0;
	croppingBottom_			= 0;
	mappingType_			= 0;
	planarMappingNormal_	= 0;
	blendMode_				= 0;
	defaultAlpha_			= 0.0f;
	materialUse_			= 0;
	textureUse_				= 0;
}
LoaderFbxTextureDesc::~LoaderFbxTextureDesc()
{
}

void LoaderFbxTextureDesc::setTextureName(const char* textureName)
{
	textureName_ = textureName;
}
void LoaderFbxTextureDesc::setFileName(const char* fileName)
{
	fileName_ = fileName;
}
void LoaderFbxTextureDesc::setScale(Float2 scale)
{
	scale_ = scale;
}
void LoaderFbxTextureDesc::setTranslation(Float2 translation)
{
	translation_ = translation;
}
void LoaderFbxTextureDesc::setRotation(Float3 rotation)
{
	rotation_ = rotation;
}
void LoaderFbxTextureDesc::setSwap(bool swap)
{
	swap_ = swap;
}
void LoaderFbxTextureDesc::setAlphaSource(int alphaSource)
{
	alphaSource_ = alphaSource;
}
void LoaderFbxTextureDesc::setCroppingLeft(int croppingLeft)
{
	croppingLeft_ = croppingLeft;
}
void LoaderFbxTextureDesc::setCroppingTop(int croppingTop)
{
	croppingTop_ = croppingTop;
}
void LoaderFbxTextureDesc::setCroppingRight(int croppingRight)
{
	croppingRight_ = croppingRight;
}
void LoaderFbxTextureDesc::setCroppingBottom(int croppingBottom)
{
	croppingBottom_ = croppingBottom;
}
void LoaderFbxTextureDesc::setMappingType(int mappingType)
{
	mappingType_ = mappingType;
}
void LoaderFbxTextureDesc::setPlanarMappingNormal(int planarMappingNormal)
{
	planarMappingNormal_ = planarMappingNormal;
}
void LoaderFbxTextureDesc::setBlendMode(int blendMode)
{
	blendMode_ = blendMode;
}
void LoaderFbxTextureDesc::setDefaultAlpha(float defaultAlpha)
{
	defaultAlpha_ = defaultAlpha;
}
void LoaderFbxTextureDesc::setMaterialUse(int materialUse)
{
	materialUse_ = materialUse;
}
void LoaderFbxTextureDesc::setTextureUse(int textureUse)
{
	textureUse_ = textureUse;
}

const char* LoaderFbxTextureDesc::getTextureName()
{
	return textureName_;
}
const char* LoaderFbxTextureDesc::getFileName()
{
	return fileName_;
}
Float2 LoaderFbxTextureDesc::getScale()
{
	return scale_;
}
Float2 LoaderFbxTextureDesc::getTranslation()
{
	return translation_; 
}
Float3 LoaderFbxTextureDesc::getRotation()
{
	return rotation_;
}
bool LoaderFbxTextureDesc::getSwap()
{
	return swap_;
}
int	LoaderFbxTextureDesc::getAlphaSource()
{
	return alphaSource_;
}
int	LoaderFbxTextureDesc::getCroppingLeft()
{
	return croppingLeft_;
}
int	LoaderFbxTextureDesc::getCroppingTop()
{
	return croppingTop_;
}
int	LoaderFbxTextureDesc::getCroppingRight()
{
	return croppingRight_;
}
int	LoaderFbxTextureDesc::getCroppingBottom()
{
	return croppingBottom_;
}
int	LoaderFbxTextureDesc::getMappingType()
{
	return mappingType_;
}
int	LoaderFbxTextureDesc::getPlanarMappingNormal()
{
	return planarMappingNormal_;
}
int LoaderFbxTextureDesc::getBlendMode()
{
	return blendMode_;
}
float LoaderFbxTextureDesc::getDefaultAlpha()
{
	return defaultAlpha_;
}
int	LoaderFbxTextureDesc::getMaterialUse()
{
	return materialUse_;
}
int	LoaderFbxTextureDesc::getTextureUse()
{
	return textureUse_;
}