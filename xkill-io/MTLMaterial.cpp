#include "MTLMaterial.h"

MTLMaterial::MTLMaterial(const std::string name)
{
	this->name_ = name;

	ambientColor_	= Float3(0.0f, 0.0f, 0.0f);
	diffuseColor_	= Float3(0.0f, 0.0f, 0.0f);
	specularColor_	= Float3(0.0f, 0.0f, 0.0f);
	
	specularPow_	= 1.0f;
	alpha_			= 0.0f;

	texAmbient_		= "";
	texDiffuse_		= "";
	texSpecular_		= "";
	texSpecularPow_	= "";
	texSpecularPow_	= "";
	texAlpha_		= "";
	texBump_			= "";
	texDisplacement_	= "";
	texStencil_		= "";
}
MTLMaterial::~MTLMaterial()
{
	//Do nothing.
}

void MTLMaterial::addIllum(const IlluminationModel im)
{
	illums_.push_back(im);
}

/*sets*/
void MTLMaterial::setAmbientColor(const Float3 ambientColor)
{
	this->ambientColor_ = ambientColor;
}
void MTLMaterial::setDiffuseColor(const Float3 diffuseColor)
{
	this->diffuseColor_ = diffuseColor;
}
void MTLMaterial::setSpecularColor(const Float3 specularColor)	
{
	this->specularColor_ = specularColor;
}
void MTLMaterial::setTF(const Float3 tf)
{
	this->tf_ = tf;
}

void MTLMaterial::setSpecularPow(const float specularPow)	
{
	this->specularPow_ = specularPow;
}
void MTLMaterial::setAlpha(const float alpha)
{
	this->alpha_ = alpha;
}
void MTLMaterial::setOpticalDensity(const float opticalDensity)
{
	this->opticalDensity_ = opticalDensity;
}

void MTLMaterial::setTexAmbient(const std::string texAmbient)
{
	this->texAmbient_ = texAmbient;
}
void MTLMaterial::setTexDiffuse(const std::string texDiffuse)		
{
	this->texDiffuse_ = texDiffuse;
}
void MTLMaterial::setTexSpecular(const std::string texSpecular)	
{
	this->texSpecular_ = texSpecular;
}
void MTLMaterial::setTexSpecularPow(const std::string texSpecularPow)	
{
	this->texSpecularPow_ = texSpecularPow;
}		
void MTLMaterial::setTexAlpha(const std::string texAlpha)	
{
	this->texAlpha_ = texAlpha;
}
void MTLMaterial::setTexBump(const std::string texBump)
{
	this->texBump_ = texBump;
}
void MTLMaterial::setTexDisplacement(const std::string texDisplacement)	
{
	this->texDisplacement_ = texDisplacement;
}
void MTLMaterial::setTexStencil(const std::string texStencil)
{
	this->texStencil_ = texStencil;
}

/*gets*/
const std::string MTLMaterial::getName()
{
	return name_;
}

const Float3 MTLMaterial::getAmbientColor()
{
	return ambientColor_;
}
const Float3 MTLMaterial::getDiffuseColor()
{
	return diffuseColor_;
}
const Float3 MTLMaterial::getSpecularColor()
{
	return specularColor_;
}
const Float3 MTLMaterial::getTF()
{
	return tf_;
}

const float MTLMaterial::getSpecularPow()
{
	return  specularPow_;
}	
const float MTLMaterial::getAlpha()	
{
	return alpha_;
}		
const float MTLMaterial::getOpticalDensity()
{
	return opticalDensity_;
}

const std::vector<IlluminationModel> MTLMaterial::getIllums()
{
	return illums_;
}

const std::string MTLMaterial::getTexAmbient()
{
	return texAmbient_;
}
const std::string MTLMaterial::getTexDiffuse()	
{
	return texDiffuse_;
}
const std::string MTLMaterial::getTexSpecular()
{
	return texSpecular_;
}
const std::string MTLMaterial::getTexSpecularPow()		
{
	return texSpecularPow_;
}
const std::string MTLMaterial::getTexAlpha()		
{
	return texAlpha_;
}
const std::string MTLMaterial::getTexBump()
{
	return texBump_;
}
const std::string MTLMaterial::getTexDisplacement()	
{
	return texDisplacement_;
}
const std::string MTLMaterial::getTexStencil()
{
	return texStencil_;
}