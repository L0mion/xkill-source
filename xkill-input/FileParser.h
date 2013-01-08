#pragma once

#include <queue>
#include <string>

//! Handles files
/*!
Reads from and writes to a file.

Should be removed as the functionality already exists in xkill-io.
*/

class FileParser
{
public:
	/*!
	\param message Message to be printed a the top of a newly created file
	*/
	FileParser(std::string message = "");
	~FileParser();

	void setFileName(std::string filename); //! Sets the file that should be handled
	void setFilePath(std::string filepath);	//! Sets the file path

	//! Start reading from a file
	/*!
	Will read the contents of a file, split it into rows and fill the rows_ queue.
	*/
	bool startReading(); 					
	std::string getNextRow();				//! Returns the next row in the rows_ queue
	bool isEmpty();							//! Returns if the rows_ queue is empty

	void startWriting();					//! Prepares the function to start writing
	void writeRow(std::string row);			//! Writes a row to the rows_ queue
	//! Finish writing to the file
	/*!
	Writes all the rows in the rows_ queue to the file.
	*/
	bool doneWriting(bool append = false);

	void deleteFile();						//! Deletes the file

private:
	std::queue<std::string> rows_;			//! Stores rows to read or written
	std::string filename_;					//! The name of the file to be handled
	std::string filepath_;					//! The path to the file
	std::string message_;					//! Message displayed at the top of a newly created file
	bool endOfFile_;	
	bool reading_;

	bool startReading(unsigned int startRowNumber);	//Start reading from a file
	void createFile();						//! Create the file
};