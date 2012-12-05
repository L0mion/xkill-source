#ifndef XKILL_IO_WRITER_H
#define XKILL_IO_WRITER_H

#include <fstream>
#include <string>

//! Base class for a Mesh-writing component.
/*!
\ingroup xkill-mesh-io
*/
class Writer
{
public:
	//! Passes path of desired file to Writer.
	/*!
	\param filePath Path up to desired file.
	\param fileName Name of desired file.
	*/
	Writer(
		const std::string filePath,
		const std::string fileName);
	//! Closes ifstream if open.
	~Writer();

	//! Purely abstract functions to be used by all loaders to write to respective files.
	virtual bool init() = 0;
protected:
	const std::string getFilePath();
	const std::string getFileName();
	const std::string getFullPath();

	std::ofstream ofstream_; //!< ofstream used to load files.
private:
	std::string filePath_;	//!< Path up to desired file.
	std::string fileName_;	//!< Name of desired file.
};

#endif //XKILL_IO_WRITER_H