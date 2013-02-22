#include "FileParser.h"

#include <fstream>
#include <stdio.h>

FileParser::FileParser(std::string message)
{
	filename_ = "";
	filepath_ = "";
	message_ = message;
	reading_ = true;
	endOfFile_ = true;
}

FileParser::~FileParser()
{

}

void FileParser::setFileName(std::string filename)
{
	filename_ = filename;
}

void FileParser::setFilePath(std::string filepath)
{
	filepath_ = filepath;
}

bool FileParser::startReading()
{
	bool couldRead = true;
	endOfFile_ = false;
	reading_ = true;

	if(!startReading(0))
	{
		couldRead = false;
		endOfFile_ = true;
	}

	return couldRead;
}

bool FileParser::startReading(unsigned int startRowNumber)
{
	std::string completeFileName = filepath_ + filename_;

	std::ifstream file(completeFileName.c_str());
	std::string row = "";
	bool continueReading = true;

	try
	{
		if(!file.is_open())
		{
			endOfFile_ = true;
			createFile();
			return false;
		}

		for(unsigned int i = 0; i < startRowNumber; i++)
		{
			if(!file.eof())
			{
				std::getline(file, row);
			}
			else
			{
				continueReading = false;
				endOfFile_ = true;
				break;
			}
		}

		while(continueReading)
		{
			if(file.eof())
			{
				continueReading = false;
				endOfFile_ = true;
			}
			//else if(rows_.size() >= MAX_NR_OF_ROWS_LOADED)	//Keep only a set amount of rows loaded at any time
			//{
			//	continueReading = false;
			//}													//For now we'll just read all rows at the same time
			else
			{
				std::getline(file, row);
				rows_.push(row);
			}
		}

		file.close();
	}
	catch(std::exception e)
	{
		if(file.is_open())
			file.close();

		return false;
	}

	return true;
}

std::string FileParser::getNextRow()
{
	std::string row = "";

	if(rows_.size() > 0)
	{
		row = rows_.front();
		rows_.pop();
	}

	return row;
}

bool FileParser::isEmpty()
{
	return (rows_.size() <= 0);
}

void FileParser::startWriting()
{
	reading_ = false;
}

void FileParser::writeRow(std::string row)
{
	rows_.push(row);
}

bool FileParser::doneWriting(bool append)
{
	std::string completeFileName = filepath_ + filename_;
	std::ofstream file;

	if(append)
		file.open(completeFileName.c_str(), file.app);
	else
		file.open(completeFileName.c_str(), file.trunc);

	std::string row = "";

	try
	{
		if(!file.is_open())
		{
			return false;
		}

		while(rows_.size())
		{
			row = rows_.front();
			rows_.pop();
			row += "\n";
			file << row;
		}

		file.close();
	}
	catch(std::exception e)
	{
		if(file.is_open())
			file.close();

		return false;
	}

	return true;
}

void FileParser::deleteFile()
{
	std::remove(std::string(filepath_ + filename_).c_str());
}

void FileParser::createFile()
{
	std::string completeFileName = filepath_ + filename_;

	std::ofstream file;
	file.open(completeFileName, file.app);
	if(file.is_open())
	{	
		file << message_;

		file.close();
	}
}

void FileParser::clean()
{
	while(!rows_.empty())
		rows_.pop();
}
