#ifndef XKILL_IO_LOADER_H
#define XKILL_IO_LOADER_H

/*! \defgroup xkill-mesh-io xkill-mesh-io
	Loading, writing and management of meshes.
	\ingroup xkill-mesh
*/

#include <fstream>
#include <string>

//! Base class for a Mesh-loading component.
/*!
\ingroup xkill-mesh-io
*/
class Loader
{
public:
	//! Passes path of desired file to the loader.
	/*!
	\param filePath Path up to desired file.
	\param fileName Name of desired file.
	*/
	Loader(
		const std::string filePath,
		const std::string fileName);
	//! Closes ifstream if open.
	~Loader();

	//! Purely abstract functions to be used by all loaders to load respective files.
	virtual bool init()	= 0;
protected:
	//! Checks whether or not a string contains an numeric value.
	/*! \return True if entirely numeric, False if not. */
	bool isNumeric(const std::string value);
	//! Used to, for each read line, increment a counter so that an error may report a line number.
	void getLine(std::string& line);

	const std::string getFilePath();
	const std::string getFileName();
	const std::string getFullPath();

	unsigned int	lineNum_;	//!< Line number previously read from file.
	std::ifstream	ifstream_;	//!< ifstream used to load files.
private:
	std::string	filePath_;	//!< Path up to desired file.
	std::string	fileName_;	//!< Name of desired file.
};

#endif //XKILL_IO_LOADER_H