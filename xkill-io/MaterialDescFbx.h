#ifndef XKILL_IO_MATERIALDESCFBX_H
#define XKILL_IO_MATERIALDESCFBX_H

#include <vector>
#include <fbxsdk.h>

#include <xkill-utilities/Math.h>

class MaterialDescFbx
{
public:
	MaterialDescFbx();
	MaterialDescFbx(Float3	ambient,
					Float3	diffuse,
					Float3	specular,
					Float3	emissive,
					float	opacity,
					float	shininess,
					float	reflectivity);
	MaterialDescFbx(Float3	ambient,
					Float3	diffuse,
					Float3	specular,
					Float3	emissive,
					float	opacity,
					float	shininess,
					float	reflectivity,
					std::vector<FbxFileTexture*> fbxFileTextures,
					std::vector<FbxLayeredTexture*>	fbxLayeredTextures,
					std::vector<FbxProceduralTexture*> fbxProceduralTextures);

	void addFbxFileTexture(FbxFileTexture* fileTexture);
	void addFbxLayeredTexture(FbxLayeredTexture* layeredTexture);
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
	Float3	ambient_;
	Float3	diffuse_;
	Float3	specular_;
	Float3	emissive_;
	float	opacity_;
	float	shininess_;
	float	reflectivity_;
	
	std::vector<FbxFileTexture*>		fbxFileTextures_;
	std::vector<FbxLayeredTexture*>		fbxLayeredTextures_;
	std::vector<FbxProceduralTexture*>	fbxProceduralTextures_;
};

#endif //XKILL_IO_MATERIALDESCFBX_H