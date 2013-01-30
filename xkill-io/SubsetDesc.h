#ifndef XKILL_IO_SUBSETDESC_H
#define XKILL_IO_SUBSETDESC_H

#include <vector>

struct SubsetDesc
{
	SubsetDesc();
	~SubsetDesc();

	std::vector<unsigned int> indices_;
	unsigned int materialIndex_;
};

#endif //XKILL_IO_SUBSETDESC_H