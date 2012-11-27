#ifndef XKILL_RENDERER_SIMPLESTRINGSPLITTER_H
#define XKILL_RENDERER_SIMPLESTRINGSPLITTER_H

#include <string>
#include <sstream>
#include <vector>

class SimpleStringSplitter
{
public:
	SimpleStringSplitter();
	~SimpleStringSplitter();

	std::vector<std::string> splitString(
		char		splitter,
		std::string	splitting);
protected:
private:
	std::stringstream	ss_;
	std::string			intermediate_;

	std::vector<std::string> split_;
};

#endif //XKILL_RENDERER_SIMPLESTRINGSPLITTER_H