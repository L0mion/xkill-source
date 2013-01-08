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
	FileParser(std::string message = "");
	~FileParser();

	void setFileName(std::string filename);
	void setFilePath(std::string filepath);

	bool startReading();
	std::string getNextRow();
	bool isEmpty();

	void startWriting();
	void writeRow(std::string row);
	bool doneWriting(bool append = false);

private:
	static const int MAX_NR_OF_ROWS_LOADED = 100;

	std::queue<std::string> rows_;
	std::string filename_;
	std::string filepath_;
	std::string message_;
	bool endOfFile_;
	bool reading_;

	bool startReading(unsigned int startRowNumber);
	void createFile();
};