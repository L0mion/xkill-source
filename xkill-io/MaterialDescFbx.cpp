#include "MaterialDescFbx.h"

MaterialDescFbx::MaterialDescFbx()
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
MaterialDescFbx::MaterialDescFbx(Float3	ambient,
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
MaterialDescFbx::MaterialDescFbx(Float3	ambient,
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

void MaterialDescFbx::addFbxFileTexture(FbxFileTexture* fileTexture)
{
	fbxFileTextures_.push_back(fileTexture);
}
void MaterialDescFbx::addFbxLayeredTexture(FbxLayeredTexture* layeredTexture)
{
	fbxLayeredTextures_.push_back(layeredTexture);
}
void MaterialDescFbx::addFbxProceduralTexture(FbxProceduralTexture* proceduralTexture)
{
	fbxProceduralTextures_.push_back(proceduralTexture);
}

void MaterialDescFbx::setAmbient(Float3 ambient)
{
	ambient_ = ambient;
}
void MaterialDescFbx::setDiffuse(Float3 diffuse)
{
	diffuse_ = diffuse;
}
void MaterialDescFbx::setSpecular(Float3 specular)
{
	specular_ = specular;
}
void MaterialDescFbx::setEmissive(Float3 emissive)
{
	emissive_ = emissive;
}
void MaterialDescFbx::setOpacity(float opacity)
{
	opacity_ = opacity;
}
void MaterialDescFbx::setShininess(float shininess)
{
	shininess_ = shininess;
}
void MaterialDescFbx::setReflectivity(float reflectivity)
{
	reflectivity_ = reflectivity;
}
void MaterialDescFbx::setFbxFileTextures(std::vector<FbxFileTexture*> fbxFileTextures)
{
	fbxFileTextures_ = fbxFileTextures;
}
void MaterialDescFbx::setFbxLayeredTextures(std::vector<FbxLayeredTexture*> fbxLayeredTextures)
{
	fbxLayeredTextures_ = fbxLayeredTextures;
}
void MaterialDescFbx::setFbxProceduralTextures(std::vector<FbxProceduralTexture*> fbxProceduralTextures)
{
	fbxProceduralTextures_ = fbxProceduralTextures;
}

Float3 MaterialDescFbx::getAmbient() const
{
	return ambient_;
}
Float3 MaterialDescFbx::getDiffuse() const
{
	return diffuse_;
}
Float3 MaterialDescFbx::getSpecular() const
{
	return specular_;
}
Float3 MaterialDescFbx::getEmissive() const
{
	return emissive_;
}
float  MaterialDescFbx::getOpacity() const
{
	return opacity_;
}
float  MaterialDescFbx::getShininess() const
{
	return shininess_;
}
float  MaterialDescFbx::getReflectivity() const
{
	return reflectivity_;
}

std::vector<FbxFileTexture*>		MaterialDescFbx::getFbxFileTextures() const
{
	return fbxFileTextures_;
}
std::vector<FbxLayeredTexture*>		MaterialDescFbx::getFbxLayeredTextures() const
{
	return fbxLayeredTextures_;
}
std::vector<FbxProceduralTexture*>	MaterialDescFbx::getFbxProceduralTextures()	const
{
	return fbxProceduralTextures_;
}