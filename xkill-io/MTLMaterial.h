#ifndef XKILL_IO_MTLMATERIAL_H
#define XKILL_IO_MTLMATERIAL_H

#include <vector>
#include <string>

#include <xkill-utilities/Math.h>

#include "MTLIllum.h"

//! Describes a single material read from .mtl.
/*!
\ingroup xkill-mesh-io-mtl
*/
class MTLMaterial
{
public:
	//! Initializes material with passed name.
	MTLMaterial(const std::string name);
	//! Does nothing.
	~MTLMaterial();

	//! Adds an illumination model-type to material. This type is pushed to a vector as a material may have several.
	void addIllum(const IlluminationModel im);

	/*sets*/
	void setAmbientColor(const Float3 ambientColor);	
	void setDiffuseColor(const Float3 diffuseColor);	
	void setSpecularColor(const Float3 specularColor);
	void setTF(const Float3 tf);
	
	void setSpecularPow(const float specularPow);				
	void setAlpha(const float alpha);		
	void setOpticalDensity(const float opticalDensity);

	void setTexAmbient(const std::string texAmbient);		
	void setTexDiffuse(const std::string texDiffuse);				
	void setTexSpecular(const std::string texSpecular);			
	void setTexSpecularPow(const std::string texSpecularPow);				
	void setTexAlpha(const std::string texAlpha);			
	void setTexBump(const std::string texBump);			
	void setTexDisplacement(const std::string texDisplacement);			
	void setTexStencil(const std::string texStencil);	

	/*gets*/
	const std::string getName();

	const Float3 getAmbientColor();	
	const Float3 getDiffuseColor();	
	const Float3 getSpecularColor();		
	const Float3 getTF();
	
	const float getSpecularPow();				
	const float getAlpha();			
	const float getOpticalDensity();

	const std::vector<IlluminationModel> getIllums();

	const std::string getTexAmbient();		
	const std::string getTexDiffuse();				
	const std::string getTexSpecular();			
	const std::string getTexSpecularPow();				
	const std::string getTexAlpha();			
	const std::string getTexBump();			
	const std::string getTexDisplacement();			
	const std::string getTexStencil();		
protected:
private:
	std::string name_;

	Float3 ambientColor_;		//!< Ka
	Float3 diffuseColor_;		//!< Kd
	Float3 specularColor_;		//!< Ks
	Float3 tf_;					//!< Tf
	/*^ Transmission Filter*/

	float specularPow_;						//!< Ns
	float alpha_;							//!< d/Tr
	float opticalDensity_;					//!< Ni

	std::vector<IlluminationModel> illums_;	//!< illum

	/*Textures*/
	std::string texAmbient_;				//!< map_Ka
	std::string texDiffuse_;				//!< map_Kd
	std::string texSpecular_;				//!< map_Ks
	std::string texSpecularPow_;			//!< map_Ns
	std::string texAlpha_;					//!< map_d / map_Tr
	std::string texBump_;					//!< bump / map_bump
	std::string texDisplacement_;			//!< disp
	std::string texStencil_;				//!< decal
};

#endif //XKILL_IO_MTLMATERIAL_H