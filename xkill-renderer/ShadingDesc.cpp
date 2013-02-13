#include "ShadingDesc.h"

ShadingDesc::ShadingDesc()
{
	vsID_ = SHADERID_VS_DEFAULT;
	psID_ = SHADERID_PS_DEFAULT;

	layoutID_ = LAYOUTID_POS_NORM_TEX;

	stride_[0] = 0;
	stride_[1] = 0;
}
ShadingDesc::~ShadingDesc()
{
	//Do nothing.
}