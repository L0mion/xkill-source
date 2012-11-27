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
	char				splitter_;
	std::string			splitting_;

	std::stringstream	ss_;

	std::vector<std::string> split;
};

#endif //XKILL_RENDERER_SIMPLESTRINGSPLITTER_H

/*Splitsample:

std::vector<std::string>   result;

	std::string str = "1/2/3/4/ 6 /fda";

	std::stringstream  data(str);

	std::string line;
	while(std::getline(data,line,'/'))
	{
		result.push_back(line); // Note: You may get a couple of blank lines
		// When multiple underscores are beside each other.
	}


*/