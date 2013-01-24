#ifndef XKILL_IO_LOADERFBXMATERIAL_H
#define XKILL_IO_LOADERFBXMATERIAL_H



namespace fbxsdk_2014_0_beta2
{
	class FbxGeometry;
	class FbxSurfaceMaterial;
	class FbxImplementation;
}

class LoaderFbxMaterialDesc;

//! Class for loading a FBX material
/*!
\ingroup xkill-io-fbx
*/
class LoaderFbxMaterial
{
public:
	//! Initializes LoaderFbxMaterial to its default state.
	LoaderFbxMaterial();
	//! Is unused.
	~LoaderFbxMaterial();
	//! Is unused.
	void reset();

	//! Retrieves material data from a FbxGeometry object.
	/*!
	\param geometry A FbxGeometry object from where the material data will be retrieved.
	\param materialDesc Material description where the results will be stored.
	*/
	void parseMaterial(FbxGeometry* geometry, LoaderFbxMaterialDesc* materialDesc);

private:
	//! Parses a material of type implementation.
	/*!
	\param material Object conatining the material data.
	\param implementation Object containing the implementation.
	\param materialIndex Index to the material.
	\param materialDesc Material description where the results will be stored.
	*/
	void parseMaterialImplementation(FbxSurfaceMaterial* material, const FbxImplementation* implementation, int materialIndex, LoaderFbxMaterialDesc* materialDesc);
	//! Parses a material of type Phong.
	/*!
	\param material Object conatining the material data.
	\param materialIndex Index to the material.
	\param materialDesc Material description where the results will be stored.
	*/
	void parseMaterialPhong(FbxSurfaceMaterial* material, int materialIndex, LoaderFbxMaterialDesc* materialDesc);
	//! Parses a material of type Lambert.
	/*!
	\param material Object conatining the material data.
	\param materialIndex Index to the material.
	\param materialDesc Material description where the results will be stored.
	*/
	void parseMaterialLambert(FbxSurfaceMaterial* material, int materialIndex, LoaderFbxMaterialDesc* materialDesc);
	//! This function is called if the material can not be Identified. It currently does nothing.
	void parseMaterialUnknown();

	//! Extracts the ambient component of a phong material.
	/*!
	\param material Object conatining the material data.
	\param materialDesc Material description where the results will be stored.
	*/
	void extractPhongAmbient(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc);
	//! Extracts the diffuse component of a phong material.
	/*!
	\param material Object conatining the material data.
	\param materialDesc Material description where the results will be stored.
	*/
	void extractPhongDiffuse(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc);
	//! Extracts the specular component of a phong material.
	/*!
	\param material Object conatining the material data.
	\param materialDesc Material description where the results will be stored.
	*/
	void extractPhongSpecular(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc);
	//! Extracts the emissive component of a phong material.
	/*!
	\param material Object conatining the material data.
	\param materialDesc Material description where the results will be stored.
	*/
	void extractPhongEmissive(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc);
	//! Extracts the opacity component of a phong material.
	/*!
	\param material Object conatining the material data.
	\param materialDesc Material description where the results will be stored.
	*/
	void extractPhongOpacity(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc);
	//! Extracts the shininess component of a phong material.
	/*!
	\param material Object conatining the material data.
	\param materialDesc Material description where the results will be stored.
	*/
	void extractPhongShininess(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc);
	//! Extracts the reflectivity component of a phong material.
	/*!
	\param material Object conatining the material data.
	\param materialDesc Material description where the results will be stored.
	*/
	void extractPhongReflectivity(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc);

	//! Extracts the ambient component of a lambert material.
	/*!
	\param material Object conatining the material data.
	\param materialDesc Material description where the results will be stored.
	*/
	void extractLambertAmbient(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc);
	//! Extracts the diffuse component of a lambert material.
	/*!
	\param material Object conatining the material data.
	\param materialDesc Material description where the results will be stored.
	*/
	void extractLambertDiffuse(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc);
	//! Extracts the emissive component of a lambert material.
	/*!
	\param material Object conatining the material data.
	\param materialDesc Material description where the results will be stored.
	*/
	void extractLambertEmissive(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc);
	//! Extracts the opacity component of a lambert material.
	/*!
	\param material Object conatining the material data.
	\param materialDesc Material description where the results will be stored.
	*/
	void extractLambertOpacity(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc);

};

#endif //XKILL_IO_LOADERFBXMATERIAL_H