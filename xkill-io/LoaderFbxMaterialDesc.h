#ifndef XKILL_IO_LOADERFBXMATERIALDESC_H
#define XKILL_IO_LOADERFBXMATERIALDESC_H

#include <vector>
#include <fbxsdk.h>

#include <xkill-utilities/Math.h>

class LoaderFbxMaterialDesc
{
public:
	//! Initializes LoaderFbxMaterialDesc to its default values.
	LoaderFbxMaterialDesc();
	//! Initializes LoaderFbxMaterialDesc with specific values.
	LoaderFbxMaterialDesc(Float3 ambient,
		Float3	diffuse,
		Float3	specular,
		Float3	emissive,
		float	opacity,
		float	shininess,
		float	reflectivity);
	//! Initializes LoaderFbxMaterialDesc with specific values.
	LoaderFbxMaterialDesc(Float3 ambient,
		Float3	diffuse,
		Float3	specular,
		Float3	emissive,
		float	opacity,
		float	shininess,
		float	reflectivity,
		std::vector<FbxFileTexture*> fbxFileTextures,
		std::vector<FbxLayeredTexture*>	fbxLayeredTextures,
		std::vector<FbxProceduralTexture*> fbxProceduralTextures);
	//! Resets LoaderFbxMaterialDesc to its default state.
	void reset();

	//! Adds a file texture to the material.
	/*!
	\param fileTexture The texture to be added.
	*/
	void addFbxFileTexture(FbxFileTexture* fileTexture);
	//! Adds a layered texture to the material.
	/*!
	\param layeredTexture The texture to be added.
	*/
	void addFbxLayeredTexture(FbxLayeredTexture* layeredTexture);
	//! Adds a procedural texture to the material.
	/*!
	\param proceduralTexture The texture to be added.
	*/
	void addFbxProceduralTexture(FbxProceduralTexture* proceduralTexture);

	void setAmbient(Float3 ambient);
	void setDiffuse(Float3 diffuse);
	void setSpecular(Float3 specular);
	void setEmissive(Float3 emissive);
	void setOpacity(float opacity);
	void setShininess(float shininess);
	void setReflectivity(float reflectivity);
	void setFbxFileTextures(std::vector<FbxFileTexture*> fbxFileTextures);
	void setFbxLayeredTextures(std::vector<FbxLayeredTexture*> fbxLayeredTextures);
	void setFbxProceduralTextures(std::vector<FbxProceduralTexture*> fbxProceduralTextures);

	Float3 getAmbient()		 const;
	Float3 getDiffuse()		 const;
	Float3 getSpecular()	 const;
	Float3 getEmissive()	 const;
	float  getOpacity()		 const;
	float  getShininess()	 const;
	float  getReflectivity() const;

	std::vector<FbxFileTexture*>		getFbxFileTextures()		const;
	std::vector<FbxLayeredTexture*>		getFbxLayeredTextures()		const;
	std::vector<FbxProceduralTexture*>	getFbxProceduralTextures()	const;

private:
	Float3	ambient_;		//<! Ambient component of the material.
	Float3	diffuse_;		//<! Diffuse component of the material.
	Float3	specular_;		//<! Specular component of the material.
	Float3	emissive_;		//<! Emissive component of the material.
	float	opacity_;		//<! Opacity component of the material.
	float	shininess_;		//<! Shininess component of the material.
	float	reflectivity_;	//<! Reflectivity component of the material.

	std::vector<FbxFileTexture*>		fbxFileTextures_;		//<! Contains file textures.
	std::vector<FbxLayeredTexture*>		fbxLayeredTextures_;	//<! Contains layered textures.
	std::vector<FbxProceduralTexture*>	fbxProceduralTextures_;	//<! Contains procedural textures.
};

#endif //XKILL_IO_LOADERFBXMATERIALDESC_H