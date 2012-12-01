#ifndef XKILL_RENDERER_PGYFORMAT_H
#define XKILL_RENDERER_PGYFORMAT_H

#include <string>

static const std::string PGY_SUFFIX = ".pgy";

struct PGYHeader
{
	char	fileType[4];
	float	versionNum;
};

#endif //XKILL_RENDERER_PGYFORMAT_H