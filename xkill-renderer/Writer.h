#ifndef XKILL_RENDERER_WRITER_H
#define XKILL_RENDERER_WRITER_H

#include <fstream>
#include <string>

class Writer
{
public:
	Writer(
		const std::string filePath,
		const std::string fileName);
	~Writer();

	virtual bool init() = 0;
protected:
	const std::string getFilePath();
	const std::string getFileName();
	const std::string getFullPath();

	std::ofstream ofstream_;
private:
	std::string filePath_;
	std::string fileName_;
};

#endif //XKILL_RENDERER_WRITER_H