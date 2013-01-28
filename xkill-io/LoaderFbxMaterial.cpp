#include <fbxsdk.h>

#include <xkill-utilities/EventManager.h>
#include <xkill-utilities/MeshVertices.h>

#include "LoaderFbxMaterial.h"
#include "LoaderFbxMaterialDesc.h"

LoaderFbxMaterial::LoaderFbxMaterial()
{
}
LoaderFbxMaterial::~LoaderFbxMaterial()
{
}
void LoaderFbxMaterial::reset()
{
}

void LoaderFbxMaterial::parseMaterial(FbxGeometry* geometry, LoaderFbxMaterialDesc* materialDesc)
{
	int numMaterials = 0;
	FbxNode* node = NULL;
	if(geometry)
	{
		node = geometry->GetNode();
		if(node)
			numMaterials = node->GetMaterialCount();
	}
	if(numMaterials > 0)
	{
		FbxColor					color;
		FbxPropertyT<FbxDouble3>	kFbxDouble3;
		FbxPropertyT<FbxDouble>		kFbxDouble;
		
		for(int materialIndex=0; materialIndex<numMaterials; materialIndex++)
		{
			FbxSurfaceMaterial* material = node->GetMaterial(materialIndex);
			
			FbxString name = material->GetName();
//			printf("Material:  %s \n", name.Buffer());
			
			const FbxImplementation* implementation = GetImplementation(material, FBXSDK_IMPLEMENTATION_HLSL);
			FbxString implementationType = "HLSL";
			if(!implementation)
			{
				implementation = GetImplementation(material, FBXSDK_IMPLEMENTATION_CGFX);
				implementationType = "CGFX";
			}
			if(implementation)
				parseMaterialImplementation(material, implementation, materialIndex, materialDesc);
			else if(material->GetClassId().Is(FbxSurfacePhong::ClassId))
				parseMaterialPhong(material, materialIndex, materialDesc);
			else if(material->GetClassId().Is(FbxSurfaceLambert::ClassId))
				parseMaterialLambert(material, materialIndex, materialDesc);
			else
				parseMaterialUnknown();
		}
	}
}
void LoaderFbxMaterial::parseMaterialImplementation(FbxSurfaceMaterial* material, const FbxImplementation* implementation, int materialIndex, LoaderFbxMaterialDesc* materialDesc)
{
	FbxBindingTable const* rootTable = implementation->GetRootTable();
	FbxString fileName = rootTable->DescAbsoluteURL.Get();
	FbxString techniqueName = rootTable->DescTAG.Get();

	FbxBindingTable const* table = implementation->GetRootTable();
	size_t entryNum = table->GetEntryCount();

	for(int entryIndex=0; entryIndex<(int)entryNum; entryIndex++)
	{
		const FbxBindingTableEntry& entry = table->GetEntry(entryIndex);
		const char* entrySrcType = entry.GetEntryType(true);
		FbxProperty fbxProperty;

		FbxString test = entry.GetSource();
//		printf("Entry: %s \n", test.Buffer());

		if(strcmp(FbxPropertyEntryView::sEntryType, entrySrcType) == 0)
		{
			fbxProperty = material->FindPropertyHierarchical(entry.GetSource());
			if(!fbxProperty.IsValid())
					fbxProperty = material->RootProperty.FindHierarchical(entry.GetSource());
		}
		else if(strcmp(FbxConstantEntryView::sEntryType, entrySrcType) == 0)
		{
			fbxProperty = implementation->GetConstants().FindHierarchical(entry.GetSource());
		}

		if(fbxProperty.IsValid())
		{
			if(fbxProperty.GetSrcObjectCount<FbxTexture>() > 0)
			{
				for(int i=0; i<fbxProperty.GetSrcObjectCount<FbxFileTexture>(); i++)
					materialDesc->addFbxFileTexture(fbxProperty.GetSrcObject<FbxFileTexture>(i));
				
				for(int i=0; i<fbxProperty.GetSrcObjectCount<FbxLayeredTexture>(); i++)
					materialDesc->addFbxLayeredTexture(fbxProperty.GetSrcObject<FbxLayeredTexture>(i));
				
				for(int i=0; i<fbxProperty.GetSrcObjectCount<FbxProceduralTexture>(); i++)
					materialDesc->addFbxProceduralTexture(fbxProperty.GetSrcObject<FbxProceduralTexture>(i));
			}
			else
			{
				FbxDataType fbxType = fbxProperty.GetPropertyDataType();
				
				/*************************************************************
				 *In the SDK example they extract various data types from the*
				 *property object here without saying what they mean and thus*
				 *I don't know what to do with it							 *
				 *************************************************************/ 
			}
		}
	}
}
void LoaderFbxMaterial::parseMaterialPhong(FbxSurfaceMaterial* material, int materialIndex, LoaderFbxMaterialDesc* materialDesc)
{
	extractPhongAmbient(material, materialDesc);
	extractPhongDiffuse(material, materialDesc);
	extractPhongSpecular(material, materialDesc);
	extractPhongEmissive(material, materialDesc);
	extractPhongOpacity(material, materialDesc);
	extractPhongShininess(material, materialDesc);
	extractPhongReflectivity(material, materialDesc);
}
void LoaderFbxMaterial::parseMaterialLambert(FbxSurfaceMaterial* material, int materialIndex, LoaderFbxMaterialDesc* materialDesc)
{
	extractLambertAmbient(material, materialDesc);
	extractLambertDiffuse(material, materialDesc);
	extractLambertEmissive(material, materialDesc);
	extractLambertOpacity(material, materialDesc);
}
void LoaderFbxMaterial::parseMaterialUnknown()
{
}

void LoaderFbxMaterial::extractPhongAmbient(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc)
{
	FbxPropertyT<FbxDouble3> fbxAmbient;
	fbxAmbient = static_cast<FbxSurfacePhong*>(material)->Ambient;
	Float3 ambient;
	ambient.x = static_cast<float>(fbxAmbient.Get()[0]);
	ambient.y = static_cast<float>(fbxAmbient.Get()[1]);
	ambient.z = static_cast<float>(fbxAmbient.Get()[2]);
	materialDesc->setAmbient(ambient);
}
void LoaderFbxMaterial::extractPhongDiffuse(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc)
{
	FbxPropertyT<FbxDouble3> fbxDiffuse;
	fbxDiffuse = static_cast<FbxSurfacePhong*>(material)->Diffuse;
	Float3 diffuse;
	diffuse.x = static_cast<float>(fbxDiffuse.Get()[0]);
	diffuse.y = static_cast<float>(fbxDiffuse.Get()[1]);
	diffuse.z = static_cast<float>(fbxDiffuse.Get()[2]);
	materialDesc->setDiffuse(diffuse);
}
void LoaderFbxMaterial::extractPhongSpecular(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc)
{
	FbxPropertyT<FbxDouble3> fbxSpecular;
	fbxSpecular = static_cast<FbxSurfacePhong*>(material)->Specular;
	Float3 specular;
	specular.x = static_cast<float>(fbxSpecular.Get()[0]);
	specular.y = static_cast<float>(fbxSpecular.Get()[1]);
	specular.z = static_cast<float>(fbxSpecular.Get()[2]);
	materialDesc->setSpecular(specular);
}
void LoaderFbxMaterial::extractPhongEmissive(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc)
{
	FbxPropertyT<FbxDouble3> fbxEmissive;
	fbxEmissive = static_cast<FbxSurfacePhong*>(material)->Emissive;
	Float3 emissive;
	emissive.x = static_cast<float>(fbxEmissive.Get()[0]);
	emissive.y = static_cast<float>(fbxEmissive.Get()[1]);
	emissive.z = static_cast<float>(fbxEmissive.Get()[2]);
	materialDesc->setEmissive(emissive);
}
void LoaderFbxMaterial::extractPhongOpacity(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc)
{
	FbxPropertyT<FbxDouble> fbxOpacity;
	fbxOpacity = static_cast<FbxSurfacePhong*>(material)->TransparencyFactor;
	float opacity = static_cast<float>(fbxOpacity.Get());
	materialDesc->setOpacity(opacity);
}
void LoaderFbxMaterial::extractPhongShininess(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc)
{
	FbxPropertyT<FbxDouble> fbxShininess;
	fbxShininess = static_cast<FbxSurfacePhong*>(material)->Shininess;
	float shininess = static_cast<float>(fbxShininess.Get());
	materialDesc->setShininess(shininess);
}
void LoaderFbxMaterial::extractPhongReflectivity(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc)
{
	FbxPropertyT<FbxDouble> fbxReflectivity;
	fbxReflectivity = static_cast<FbxSurfacePhong*>(material)->ReflectionFactor;
	float reflectivity = static_cast<float>(fbxReflectivity.Get());
	materialDesc->setReflectivity(reflectivity);
}

void LoaderFbxMaterial::extractLambertAmbient(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc)
{
	FbxPropertyT<FbxDouble3> fbxAmbient;
	fbxAmbient = static_cast<FbxSurfaceLambert*>(material)->Ambient;
	Float3 ambient;
	ambient.x = static_cast<float>(fbxAmbient.Get()[0]);
	ambient.y = static_cast<float>(fbxAmbient.Get()[1]);
	ambient.z = static_cast<float>(fbxAmbient.Get()[2]);
	materialDesc->setAmbient(ambient);
}
void LoaderFbxMaterial::extractLambertDiffuse(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc)
{
	FbxPropertyT<FbxDouble3> fbxDiffuse;
	fbxDiffuse = static_cast<FbxSurfaceLambert*>(material)->Diffuse;
	Float3 diffuse;
	diffuse.x = static_cast<float>(fbxDiffuse.Get()[0]);
	diffuse.y = static_cast<float>(fbxDiffuse.Get()[1]);
	diffuse.z = static_cast<float>(fbxDiffuse.Get()[2]);
	materialDesc->setDiffuse(diffuse);
}
void LoaderFbxMaterial::extractLambertEmissive(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc)
{
	FbxPropertyT<FbxDouble3> fbxEmissive;
	fbxEmissive = static_cast<FbxSurfaceLambert*>(material)->Emissive;
	Float3 emissive;
	emissive.x = static_cast<float>(fbxEmissive.Get()[0]);
	emissive.y = static_cast<float>(fbxEmissive.Get()[1]);
	emissive.z = static_cast<float>(fbxEmissive.Get()[2]);
	materialDesc->setEmissive(emissive);
}
void LoaderFbxMaterial::extractLambertOpacity(FbxSurfaceMaterial* material, LoaderFbxMaterialDesc* materialDesc)
{
	FbxPropertyT<FbxDouble> fbxOpacity;
	fbxOpacity = static_cast<FbxSurfaceLambert*>(material)->TransparencyFactor;
	float opacity = static_cast<float>(fbxOpacity.Get());
	materialDesc->setOpacity(opacity);
}