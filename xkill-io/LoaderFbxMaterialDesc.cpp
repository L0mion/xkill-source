#include "LoaderFbxMaterialDesc.h"

LoaderFbxMaterialDesc::LoaderFbxMaterialDesc()
{
	ambient_		= Float3(0.0f, 0.0f, 0.0f);
	diffuse_		= Float3(0.0f, 0.0f, 0.0f);
	specular_		= Float3(0.0f, 0.0f, 0.0f);
	emissive_		= Float3(0.0f, 0.0f, 0.0f);
	opacity_		= 0.0f;
	shininess_		= 0.0f;
	reflectivity_	= 0.0f;

	fbxFileTextures_.clear();
	fbxLayeredTextures_.clear();
	fbxProceduralTextures_.clear();
}
LoaderFbxMaterialDesc::LoaderFbxMaterialDesc(Float3	ambient,
											 Float3	diffuse,
											 Float3	specular,
											 Float3	emissive,
											 float	opacity,
											 float	shininess,
											 float	reflectivity)
{
	ambient_		= ambient;
	diffuse_		= diffuse;
	specular_		= specular;
	emissive_		= emissive;
	opacity_		= opacity;
	shininess_		= shininess;
	reflectivity_	= reflectivity;

	fbxFileTextures_.clear();
	fbxLayeredTextures_.clear();
	fbxProceduralTextures_.clear();
}
LoaderFbxMaterialDesc::LoaderFbxMaterialDesc(Float3	ambient,
											 Float3	diffuse,
											 Float3	specular,
											 Float3	emissive,
											 float	opacity,
											 float	shininess,
											 float	reflectivity,
											 std::vector<FbxFileTexture*>		fbxFileTextures,
											 std::vector<FbxLayeredTexture*>	fbxLayeredTextures,
											 std::vector<FbxProceduralTexture*> fbxProceduralTextures)
{
	ambient_		= ambient;
	diffuse_		= diffuse;
	specular_		= specular;
	emissive_		= emissive;
	opacity_		= opacity;
	shininess_		= shininess;
	reflectivity_	= reflectivity;

	fbxFileTextures_		= fbxFileTextures;
	fbxLayeredTextures_		= fbxLayeredTextures;
	fbxProceduralTextures_	= fbxProceduralTextures;
}

void LoaderFbxMaterialDesc::reset()
{
	ambient_		= Float3(0.0f, 0.0f, 0.0f);
	diffuse_		= Float3(0.0f, 0.0f, 0.0f);
	specular_		= Float3(0.0f, 0.0f, 0.0f);
	emissive_		= Float3(0.0f, 0.0f, 0.0f);
	opacity_		= 0.0f;
	shininess_		= 0.0f;
	reflectivity_	= 0.0f;

	fbxFileTextures_.clear();
	fbxLayeredTextures_.clear();
	fbxProceduralTextures_.clear();
}

void LoaderFbxMaterialDesc::addFbxFileTexture(FbxFileTexture* fileTexture)
{
	fbxFileTextures_.push_back(fileTexture);
}
void LoaderFbxMaterialDesc::addFbxLayeredTexture(FbxLayeredTexture* layeredTexture)
{
	fbxLayeredTextures_.push_back(layeredTexture);
}
void LoaderFbxMaterialDesc::addFbxProceduralTexture(FbxProceduralTexture* proceduralTexture)
{
	fbxProceduralTextures_.push_back(proceduralTexture);
}

void LoaderFbxMaterialDesc::setAmbient(Float3 ambient)
{
	ambient_ = ambient;
}
void LoaderFbxMaterialDesc::setDiffuse(Float3 diffuse)
{
	diffuse_ = diffuse;
}
void LoaderFbxMaterialDesc::setSpecular(Float3 specular)
{
	specular_ = specular;
}
void LoaderFbxMaterialDesc::setEmissive(Float3 emissive)
{
	emissive_ = emissive;
}
void LoaderFbxMaterialDesc::setOpacity(float opacity)
{
	opacity_ = opacity;
}
void LoaderFbxMaterialDesc::setShininess(float shininess)
{
	shininess_ = shininess;
}
void LoaderFbxMaterialDesc::setReflectivity(float reflectivity)
{
	reflectivity_ = reflectivity;
}
void LoaderFbxMaterialDesc::setFbxFileTextures(std::vector<FbxFileTexture*> fbxFileTextures)
{
	fbxFileTextures_ = fbxFileTextures;
}
void LoaderFbxMaterialDesc::setFbxLayeredTextures(std::vector<FbxLayeredTexture*> fbxLayeredTextures)
{
	fbxLayeredTextures_ = fbxLayeredTextures;
}
void LoaderFbxMaterialDesc::setFbxProceduralTextures(std::vector<FbxProceduralTexture*> fbxProceduralTextures)
{
	fbxProceduralTextures_ = fbxProceduralTextures;
}

Float3 LoaderFbxMaterialDesc::getAmbient() const
{
	return ambient_;
}
Float3 LoaderFbxMaterialDesc::getDiffuse() const
{
	return diffuse_;
}
Float3 LoaderFbxMaterialDesc::getSpecular() const
{
	return specular_;
}
Float3 LoaderFbxMaterialDesc::getEmissive() const
{
	return emissive_;
}
float  LoaderFbxMaterialDesc::getOpacity() const
{
	return opacity_;
}
float  LoaderFbxMaterialDesc::getShininess() const
{
	return shininess_;
}
float  LoaderFbxMaterialDesc::getReflectivity() const
{
	return reflectivity_;
}

std::vector<FbxFileTexture*>		LoaderFbxMaterialDesc::getFbxFileTextures() const
{
	return fbxFileTextures_;
}
std::vector<FbxLayeredTexture*>		LoaderFbxMaterialDesc::getFbxLayeredTextures() const
{
	return fbxLayeredTextures_;
}
std::vector<FbxProceduralTexture*>	LoaderFbxMaterialDesc::getFbxProceduralTextures()	const
{
	return fbxProceduralTextures_;
}

MaterialDesc LoaderFbxMaterialDesc::getMaterialDesc()
{
	MaterialDesc material;

	material.ambientTerm_ = ambient_;
	material.diffuseTerm_ = diffuse_;
	material.specularTerm_ = specular_;
	material.specularPower_ = reflectivity_; //shininess_;
	material.reflectivity_ = Float3(reflectivity_, reflectivity_, reflectivity_);
	material.transperency_ = opacity_;
	material.emissive_ = emissive_;
	material.shinyness_ = shininess_;
	
	return material;
}