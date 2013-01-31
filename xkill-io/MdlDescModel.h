#ifndef XKILL_IO_MDLDESCMODEL_H
#define XKILL_IO_MDLDESCMODEL_H

#include <string>

//! Describes data used to load a model for use in engine.
/*!
\ingroup xkill-io
*/
struct MdlDescModel
{
	unsigned int	modelID_;		//!< ID of model.
	std::string		modelFileName_;	//!< Filename of model.
	bool			dynamic_;		//!< Specifies whether or not object is to be interpreted as dynamic by physics-component.
	unsigned int	vertexType_;	//!< Specifies the type of vertex the models uses. 

	//! Does nothing.
	MdlDescModel() {}
	//! Initializes MdlDescModel.
	MdlDescModel(
		unsigned int	modelID,
		std::string		modelFileName,
		bool			dynamic,
		unsigned int	vertexType)
	{
		modelID_		= modelID;
		modelFileName_	= modelFileName;
		dynamic_		= dynamic;
		vertexType_		= vertexType;
	}
	//! Does nothing.
	~MdlDescModel() {}
};

#endif //XKILL_IO_MDLDESC_MODEL_H