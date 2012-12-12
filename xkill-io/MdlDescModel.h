#ifndef XKILL_IO_MDLDESCMODEL_H
#define XKILL_IO_MDLDESCMODEL_H

#include <string>

//! Describes data used to load a model for use in engine.
struct MdlDescModel
{
	unsigned int	modelID_;		//!< ID of model.
	std::string		modelFileName_;	//!< Filename of model.
	bool			dynamic_;		//!< Specifies whether or not object is to be interpreted as dynamic by physics-component.

	//! Does nothing.
	MdlDescModel() {}
	//! Initializes MdlDescModel.
	MdlDescModel(
		unsigned int	modelID,
		std::string		modelFileName,
		bool			dynamic)
	{
		modelID_		= modelID;
		modelFileName_	= modelFileName;
		dynamic_		= dynamic;
	}
	//! Does nothing.
	~MdlDescModel() {}
};

#endif //XKILL_IO_MDLDESC_MODEL_H