#ifndef XKILL_IO_MDLDESCMODEL_H
#define XKILL_IO_MDLDESCMODEL_H

#include <string>

struct MdlDescModel
{
	unsigned int	modelID_;
	std::string		modelFileName_;
	bool			dynamic_;

	MdlDescModel() {}
	MdlDescModel(
		unsigned int	modelID,
		std::string		modelFileName,
		bool			dynamic)
	{
		modelID_		= modelID;
		modelFileName_	= modelFileName;
		dynamic_		= dynamic;
	}
	~MdlDescModel() {}
};

#endif //XKILL_IO_MDLDESC_MODEL_H