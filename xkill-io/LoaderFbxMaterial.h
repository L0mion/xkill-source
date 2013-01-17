#ifndef XKILL_IO_LOADERFBXMATERIAL_H
#define XKILL_IO_LOADERFBXMATERIAL_H



namespace fbxsdk_2014_0_beta2
{
	class FbxGeometry;
	class FbxSurfaceMaterial;
	class FbxImplementation;
}

class LoaderFbxMaterialDesc;

class LoaderFbxMaterial
{
public:
	LoaderFbxMaterial();
	~LoaderFbxMaterial();
	void reset();

	void parseMaterial(FbxGeometry* geometry);

private:
	void parseMaterialImplementation(FbxSurfaceMaterial* material, const FbxImplementation* implementation, int materialIndex, LoaderFbxMaterialDesc* materialDesc);
	void parseMaterialPhong(FbxSurfaceMaterial* material, int materialIndex, LoaderFbxMaterialDesc* materialDesc);
	void parseMaterialLambert(FbxSurfaceMaterial* material, int materialIndex, LoaderFbxMaterialDesc* materialDesc);
	void parseMaterialUnknown();

	void extractPhongAmbient(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc);
	void extractPhongDiffuse(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc);
	void extractPhongSpecular(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc);
	void extractPhongEmissive(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc);
	void extractPhongOpacity(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc);
	void extractPhongShininess(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc);
	void extractPhongReflectivity(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc);

	void extractLambertAmbient(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc);
	void extractLambertDiffuse(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc);
	void extractLambertEmissive(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc);
	void extractLambertOpacity(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc);

};

#endif //XKILL_IO_LOADERFBXMATERIAL_H