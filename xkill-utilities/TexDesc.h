#ifndef XKILL_UTILITIES_TEXDESC_H
#define XKILL_UTILITIES_TEXDESC_H

#include <vector>

#include "dllUtilities.h"
#include "TexDescHeader.h"
#include "TexDescTex.h"

class DLL_U TexDesc
{
public:
	TexDesc(
		TexDescHeader header,
		std::vector<TexDescTex> texDescs);
	~TexDesc();

	TexDescHeader& getHeader();
	std::vector<TexDescTex>& getTexDescs();
protected:
private:
	TexDescHeader			header_;
	std::vector<TexDescTex>	texDescs_;
};

#endif //XKILL_IO_TEXDESC_H