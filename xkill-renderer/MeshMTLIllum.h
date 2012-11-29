#ifndef XKILL_RENDERER_MESHMTLILLUM_H
#define XKILL_RENDERER_MESHMTLILLUM_H

enum IlluminationModel 
{
	ILLUM_COLOR_ON_AMBIENT_OFF,
	ILLUM_COLOR_ON_AMBIENT_ON,
	ILLUM_HIGHLIGHT_ON,
	ILLUM_REFLECTION_ON_RAYTRACE_ON,
	ILLUM_TRANSPARENCY_GLASS_ON_REFLECTION_RAYTRACE_ON,
	ILLUM_REFLECTION_FRESNEL_ON_RAYTRACE_ON,
	ILLUM_TRANSPARANCY_REFRACTION_ON_REFLECTION_FRESNEL_OFF_RAYTRACE_ON,
	ILLUM_TRANSPARANCY_REFRACTION_ON_REFLECTION_FRESNEL_ON_RAYTRACE_ON,
	ILLUM_REFLECTION_ON_RAYTRACE_OFF,
	ILLUM_TRANSPARENCY_GLASS_ON_REFLECTION_RAYTRACE_OFF,
	ILLUM_SHADOWS,

	ILLUM_INVALID
};

static const IlluminationModel integerToIllum(unsigned int integer)
{
	IlluminationModel illum = ILLUM_INVALID;

	switch(integer)
	{
	case 0:
		illum = ILLUM_COLOR_ON_AMBIENT_OFF;
		break;
	case 1:
		illum = ILLUM_COLOR_ON_AMBIENT_ON;
		break;
	case 2:
		illum = ILLUM_HIGHLIGHT_ON;
		break;
	case 3:
		illum = ILLUM_REFLECTION_ON_RAYTRACE_ON;
		break;
	case 4:
		illum = ILLUM_TRANSPARENCY_GLASS_ON_REFLECTION_RAYTRACE_ON;
		break;
	case 5:
		illum = ILLUM_REFLECTION_FRESNEL_ON_RAYTRACE_ON;
		break;
	case 6:
		illum = ILLUM_TRANSPARANCY_REFRACTION_ON_REFLECTION_FRESNEL_OFF_RAYTRACE_ON;
		break;
	case 7:
		illum = ILLUM_TRANSPARANCY_REFRACTION_ON_REFLECTION_FRESNEL_ON_RAYTRACE_ON;
		break;
	case 8:
		illum = ILLUM_REFLECTION_ON_RAYTRACE_OFF;
		break;
	case 9:
		illum = ILLUM_TRANSPARENCY_GLASS_ON_REFLECTION_RAYTRACE_OFF;
		break;
	case 10:
		illum = ILLUM_TRANSPARENCY_GLASS_ON_REFLECTION_RAYTRACE_OFF;
		break;
	}

	return illum;
}

/*
0. Color on and Ambient off
1. Color on and Ambient on
2. Highlight on
3. Reflection on and Ray trace on
4. Transparency: Glass on, Reflection: Ray trace on
5. Reflection: Fresnel on and Ray trace on
6. Transparency: Refraction on, Reflection: Fresnel off and Ray trace on
7. Transparency: Refraction on, Reflection: Fresnel on and Ray trace on
8. Reflection on and Ray trace off
9. Transparency: Glass on, Reflection: Ray trace off
10. Casts shadows onto invisible surfaces
*/

#endif //XKILL_RENDERER_MESHMTLILLUM_H