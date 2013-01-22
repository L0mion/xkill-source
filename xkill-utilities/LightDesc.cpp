#include "LightDesc.h"

//LightDesc::LightDesc()
//{
//	ambient		= Float4(0.0f, 0.0f, 0.0f, 0.0f);
//	diffuse		= Float4(0.0f, 0.0f, 0.0f, 0.0f);
//	specular	= Float4(0.0f, 0.0f, 0.0f, 0.0f);
//}
//LightDesc::~LightDesc()
//{
//	//Do nothing.
//}

LightDescDir::LightDescDir() //: LightDesc()
{
	ambient		= Float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse		= Float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular	= Float4(0.0f, 0.0f, 0.0f, 0.0f);

	direction = Float3(0.0f, 0.0f, 0.0f);
}
LightDescDir::~LightDescDir()
{
	//Do nothing.
}

LightDescPoint::LightDescPoint() //: LightDesc()
{
	ambient		= Float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse		= Float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular	= Float4(0.0f, 0.0f, 0.0f, 0.0f);

	//pos			= Float3(0.0f, 0.0f, 0.0f);
	range		= 0.0f;
	attenuation	= Float3(0.0f, 0.0f, 0.0f);
}
LightDescPoint::~LightDescPoint()
{
	//Do nothing.
}

LightDescSpot::LightDescSpot() //: LightDesc()
{
	ambient		= Float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse		= Float4(0.0f, 0.0f, 0.0f, 0.0f);
	specular	= Float4(0.0f, 0.0f, 0.0f, 0.0f);

	//pos			= Float3(0.0f, 0.0f, 0.0f);
	range		= 0.0f;
	direction	= Float3(0.0f, 0.0f, 0.0f);
	spotPow		= 0.0f;
	attenuation	= Float3(0.0f, 0.0f, 0.0f);
}
LightDescSpot::~LightDescSpot()
{
	//Do nothing.
}